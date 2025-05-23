// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <vector>

#include <glm/glm.hpp>  
#include <opencv2/opencv.hpp>

#include <detector.h>
#include <modelExceptions.h>

Detector::Detector(const glm::vec3& center, const glm::vec3& x_axis, const glm::vec3& y_axis, const glm::vec3& z_axis) : center(center)
{
	plate = std::make_unique<ZArray>();

	double halfWidth = (nDetectorResY - 1) * yDetectorPitch / 2.0;
	double halfHeight = (nDetectorResZ - 1) * zDetectorPitch / 2.0;

	for (int h = 0; h < nDetectorResZ; h++)
	{
		for (int w = 0; w < nDetectorResY; w++)
		{
			double y = center.y + w * yDetectorPitch - halfWidth;
			double z = center.z + h * zDetectorPitch - halfHeight;
			double x = center.x;

			(*plate)[h][w] = glm::vec3(x, y, z);
		}
	}
}

glm::vec3 Detector::getPixel(int y, int z) const
{
	if (y >= 0 && y < nDetectorResY && z >= 0 && z < nDetectorResZ)
		return (*plate)[z][y];

	THROW_DETAILED_EXCEPTION("detector pixel location is outside of range");
}

cv::Mat Detector::getPixels(const Source& src, const std::vector<glm::vec3>& areaPoints)
{
	auto srcCenter = src.getCenter();

	cv::Mat detectorValue = cv::Mat::zeros(nDetectorResZ, nDetectorResY, CV_8U);

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			glm::vec3 pos = getPixel(ii, jj);
			glm::vec3 lineDetector = pos - srcCenter;

			for (auto ap : areaPoints)
			{
				glm::vec3 lineScanAreaPoint = ap - srcCenter;

				float detectorLineLengths = glm::length(lineDetector);

				glm::vec3 projOnTolineDetector = glm::dot(lineScanAreaPoint, lineDetector) / (detectorLineLengths * detectorLineLengths) * lineDetector;

				glm::vec3 shortestVector = ap - projOnTolineDetector;

				float distance = glm::length(shortestVector);

				if (distance < 0.5)
					detectorValue.at<uchar>(jj, ii) = 255;
			}
		}
	}

	return detectorValue;
}

cv::Mat Detector::getPixelsPyramidMethod(const Source& src, const std::vector<glm::vec3>& areaPoints)
{
	glm::vec3 apex = src.getCenter();

	cv::Mat detectorValue = cv::Mat::zeros(nDetectorResZ, nDetectorResY, CV_16U);

#ifdef USE_CUDA
	getPixelsPyramidMethodKernelLouncher(this, apex, areaPoints, detectorValue);

#else

	ushort maxValue = 1;

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			glm::vec3 baseCenter = getPixel(ii, jj);
			glm::vec3 xV = baseCenter - apex;
			float h = glm::length(xV);
			if (h < 1e-6f) continue; // avoid zero length

			xV = glm::normalize(xV);

			// Build orthonormal frame
			glm::vec3 arbitrary = (fabs(glm::dot(xV, glm::vec3(0, 0, 1))) < 0.99f) ? glm::vec3(0, 0, 1) : glm::vec3(1, 0, 0);
			glm::vec3 yV = glm::normalize(glm::cross(xV, arbitrary));
			glm::vec3 zV = glm::cross(xV, yV); // right-handed frame

			float half_side = static_cast<float>(xVoxelPitch / 2.0);

			ushort currentValue = 0;

			for (auto ap : areaPoints)
			{
				glm::vec3 apexToPoint = ap - apex;

				float px = glm::dot(apexToPoint, xV);  // along pyramid axis (height)
				float py = glm::dot(apexToPoint, yV);  // lateral
				float pz = glm::dot(apexToPoint, zV);  // lateral

				if (px >= 0 && px <= h &&
					fabs(py) <= half_side &&
					fabs(pz) <= half_side)
				{
					currentValue += 1;
				}
			}

			if (currentValue > maxValue)
			{
				maxValue = currentValue;
			}

			detectorValue.at<ushort>(jj, ii) = currentValue;
		}
	}

	for (int jj = 0; jj < nDetectorResZ; jj++)
	{
		for (int ii = 0; ii < nDetectorResY; ii++)
		{
			detectorValue.at<ushort>(jj, ii) = static_cast<ushort>((1.0f - (float(detectorValue.at<ushort>(jj, ii)) / maxValue)) * 65535);
		}
	}

#endif

	return detectorValue;
}
