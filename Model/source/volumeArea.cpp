// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <volumeArea.h>
#include <modelExceptions.h>

VolumeArea::VolumeArea(const glm::vec3& center, const glm::vec3& x_axis, const glm::vec3& y_axis, const glm::vec3& z_axis) : boxCenter(center)
{
	scanBox = std::make_unique<ZArray>();

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

				(*scanBox)[h][w][d] = glm::vec3(x, y, z);
			}
		}
	}
}

glm::vec3 VolumeArea::getPointLocation(int x, int y, int z)
{
	if (x >= 0 && x < nVoxelsX &&
		y >= 0 && y < nVoxelsY &&
		z >= 0 && z < nVoxelsZ)
		return (*scanBox)[z][y][x];

	THROW_DETAILED_EXCEPTION("voxel location is outside of range");
}

glm::vec3* getPointsInsideObject(const ScanObject& object)
{
	return nullptr;
}
