// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <detector.h>
#include <modelExceptions.h>

Detector::Detector(const glm::vec3& center, const glm::vec3& x_axis, const glm::vec3& y_axis, const glm::vec3& z_axis) : center(center)
{
	plate = std::make_unique<ZArray>();

	double halfWidth = (nVoxelsY - 1) * yDetectorPitch / 2.0;
	double halfHeight = (nVoxelsZ - 1) * zDetectorPitch / 2.0;

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
