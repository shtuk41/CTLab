#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include <modelDefs.h>

class VolumeArea
{
private:

	glm::vec3 boxCenter;

	using XArray = std::array<glm::vec3, nVoxelsX>;
	using YArray = std::array<XArray, nVoxelsY>;
	using ZArray = std::array<YArray, nVoxelsZ>;

	std::unique_ptr<ZArray> scanBox;

public:
	VolumeArea(const glm::vec3& center = { 0,0,0 }, const glm::vec3& x_axis = { 1,0,0 }, const glm::vec3& y_axis = { 0,1,0 }, const glm::vec3& z_axis = { 0,0,1 });


	glm::vec3 getPointLocation(int w, int h, int d);
};
