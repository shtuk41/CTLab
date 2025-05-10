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

	for (int h = 0; h < nVoxelsZ; h++)
	{
		for (int w = 0; w < nVoxelsY; w++)
		{
			double y = center.y + w * yDetectorPitch - halfWidth;
			double z = center.z + h * zDetectorPitch - halfHeight;
			double x = center.x;

			(*plate)[h][w] = glm::vec3(x, y, z);
		}
	}
}

glm::vec3 Detector::getPixel(int y, int z)
{
	if (y >= 0 && y < nDetectorResY && z >= 0.0 && z < std::abs(nVoxelsZ))
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

				glm::length_t distance = shortestVector.length();

				if (distance < 0.1)
					detectorValue.at<uchar>(jj, ii) = 255;
			}
		}
	}

	return detectorValue;
}
