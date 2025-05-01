#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <modelDefs.h>

#include <scanObject.h>

class VolumeArea
{
private:

	glm::vec3 boxCenter;

	using XArray = std::vector<glm::vec3>;
	using YArray = std::vector<XArray>;
	using ZArray = std::vector<YArray>;

	std::unique_ptr<ZArray> scanBox;

public:
	VolumeArea(const glm::vec3& center = { 0,0,0 }, const glm::vec3& x_axis = { 1,0,0 }, const glm::vec3& y_axis = { 0,1,0 }, const glm::vec3& z_axis = { 0,0,1 });

	std::vector<glm::vec3> getPointsInsideObject(const ScanObject& object);
	std::vector<glm::vec3> getAllPoints() const;


	glm::vec3 getPointLocation(int w, int h, int d);
	
};
