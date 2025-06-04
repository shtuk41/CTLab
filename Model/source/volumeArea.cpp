// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <volumeArea.h>
#include <modelExceptions.h>

VolumeArea::VolumeArea(const glm::vec3& center, const glm::vec3& x_axis, const glm::vec3& y_axis, const glm::vec3& z_axis) : boxCenter(center)
{
	scanBox = std::make_unique<ZArray>(nVoxelsZ);

	for (auto& y : *scanBox)
	{
		y.resize(nVoxelsY);
		for (auto& x : y)
		{
			x.resize(nVoxelsX);
		}
	}

	double halfWidth = (nVoxelsY - 1) * yVoxelPitch / 2.0;
	double halfHeight = (nVoxelsZ - 1) * zVoxelPitch / 2.0;
	double halfDepth = (nVoxelsX - 1) * xVoxelPitch / 2.0;

	for (int h = 0; h < nVoxelsZ; h++)
	{
		for (int w = 0; w < nVoxelsY; w++)
		{
			for (int d = 0; d < nVoxelsX; d++)
			{
				double x = center.x + w * yVoxelPitch - halfWidth;
				double y = center.y + h * zVoxelPitch - halfHeight;
				double z = center.z + d * xVoxelPitch - halfDepth;

				(*scanBox)[h][w][d] = std::pair<glm::vec3, int>(glm::vec3(x, y, z), 0);
			}
		}
	}
}

glm::vec3 VolumeArea::getPointLocation(int x, int y, int z)
{
	if (x >= 0 && x < nVoxelsX &&
		y >= 0 && y < nVoxelsY &&
		z >= 0 && z < nVoxelsZ)
		return (*scanBox)[z][y][x].first;

	THROW_DETAILED_EXCEPTION("voxel location is outside of range");
}

std::vector<glm::vec3> VolumeArea::getAllPoints() const
{
	std::vector<glm::vec3> points;

	for (int h = 0; h < nVoxelsZ; h++)
	{
		for (int w = 0; w < nVoxelsY; w++)
		{
			for (int d = 0; d < nVoxelsX; d++)
			{
				points.push_back((*scanBox)[h][w][d].first);
			}
		}
	}

	return points;
}

std::vector<glm::vec3> VolumeArea::getPointsInsideObject(const ScanObject& object)
{
	//winding number procedure to find out if the point is inside mesh

	std::vector<glm::vec3> innerPoints;

#ifdef USE_CUDA
	
	getPointsInsideObjectCudaKernelLouncher(object, scanBox.get(), innerPoints);

#else

	for (int h = 0; h < nVoxelsZ; h++)
	{
		for (int w = 0; w < nVoxelsY; w++)
		{
			for (int d = 0; d < nVoxelsX; d++)
			{
				glm::vec4* ptr = object.GetTrianglesWithOffset();

				glm::vec3 p = (*scanBox)[h][w][d].first;
				//std::cout << "Point: " << p.x << " : " << p.y << " : " << p.z << '\n';

				float totalSolidAngle = 0.0f;

				for (unsigned int ii = 0; ii < object.GetNumberOfTriangles(); ii++)
				{
					if (ptr != nullptr)
					{
						//std::cout << "Triangle: " << ii << " : " << ptr->x << " : " << ptr->y << " : " << ptr->z << '\n';

						glm::vec3 a = glm::vec3(*ptr++) - p;
						glm::vec3 b = glm::vec3(*ptr++) - p;
						glm::vec3 c = glm::vec3(*ptr++) - p;

						float la = glm::length(a);
						float lb = glm::length(b);
						float lc = glm::length(c);

						//float numerator = fabs(glm::dot(a, glm::cross(b, c)));
						float numerator = glm::dot(a, glm::cross(b, c));

						float denominator = (la * lb * lc +
											glm::dot(a, b) * lc +
											glm::dot(b, c) * la +
											glm::dot(c, a) * lb);

						float omega = 2.0f * atan2f(numerator, denominator);

						totalSolidAngle += omega;
					}
				}

				//std::cout << "Solid angle: " << totalSolidAngle << '\n';

				//const float epsilon = 1e-3f;
				const float epsilon = 1e-2f;
				const float four_pi = 4.0f * glm::pi<float>();

				bool isInside = glm::abs(totalSolidAngle - four_pi) < epsilon;

				if (isInside)
				{
					innerPoints.push_back(p);
				}
			}
		}
	}

#endif
		
	return innerPoints;
}

void VolumeArea::backprojectSlice(const Detector& detector, const Source& src, const cv::Mat& data, float angle)
{
	glm::vec3 apex = src.getCenter();

	float angleRadians = glm::radians(angle);
	glm::mat4 rot = glm::rotate(angleRadians, glm::vec3(0.0f, 1.0f, 0.0));

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			glm::vec3 baseCenter = detector.getPixel(jj, ii);
			glm::vec3 xV = baseCenter - apex;
			float xVLength = glm::length(xV);
			if (xVLength < 1e-6f) continue; // avoid zero length

			xV = glm::normalize(xV);

			// Build orthonormal frame
			glm::vec3 arbitrary = (fabs(glm::dot(xV, glm::vec3(0, 0, 1))) < 0.99f) ? glm::vec3(0, 0, 1) : glm::vec3(1, 0, 0);
			glm::vec3 yV = glm::normalize(glm::cross(xV, arbitrary));
			glm::vec3 zV = glm::cross(xV, yV); // right-handed frame

			float half_side = static_cast<float>(xVoxelPitch / 2.0);

			for (int h = 0; h < nVoxelsZ; h++)
			{
				for (int w = 0; w < nVoxelsY; w++)
				{
					for (int d = 0; d < nVoxelsX; d++)
					{
						auto& p = (*scanBox)[h][w][d];

						glm::vec3 areaPoint = p.first;
						glm::vec3 areaPointTSource = areaPoint - boxCenter;
						glm::vec4 rotated_point = rot * glm::vec4(areaPointTSource, 1.0f);
						glm::vec3 rotatedPoint = glm::vec3(rotated_point) + boxCenter;

						glm::vec3 apexToPoint = rotatedPoint - apex;

						float px = glm::dot(apexToPoint, xV);  // along pyramid axis (height)
						float py = glm::dot(apexToPoint, yV);  // lateral
						float pz = glm::dot(apexToPoint, zV);  // lateral

						if (px >= 0 && px <= xVLength &&
							fabs(py) <= half_side &&
							fabs(pz) <= half_side)
						{
							p.second += static_cast<int>(65535 - data.at<ushort>(jj, ii));
						}
					}
				}
			}
		}
	}
}

void VolumeArea::writeFile(const std::string fileName)
{
	std::ofstream saveFile(fileName.c_str(), std::ios::binary);

	try
	{
		if (saveFile.is_open())
		{
			for (int h = 0; h < nVoxelsZ; h++)
			{
				for (int w = 0; w < nVoxelsY; w++)
				{
					for (int d = 0; d < nVoxelsX; d++)
					{
						int p = (*scanBox)[h][w][d].second;
						saveFile.write(reinterpret_cast<const char*>(&p), sizeof(p));
					}
				}
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << "Error writing file: " << e.what() << std::endl;
		return;
	}

}
