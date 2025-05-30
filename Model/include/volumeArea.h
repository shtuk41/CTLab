#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <detector.h>
#include <modelDefs.h>
#include <scanObject.h>


struct Vec3
{
	float x, y, z;
	Vec3(float x_ = 0, float y_ = 0, float z_ = 0) : x(x_), y(y_), z(z_) {}
};



class VolumeArea
{
private:

	glm::vec3 boxCenter;
	std::unique_ptr<ZArray> scanBox;

public:
	VolumeArea(const glm::vec3& center = { 0,0,0 }, const glm::vec3& x_axis = { 1,0,0 }, const glm::vec3& y_axis = { 0,1,0 }, const glm::vec3& z_axis = { 0,0,1 });

	std::vector<glm::vec3> getPointsInsideObject(const ScanObject& object);
	std::vector<glm::vec3> getAllPoints() const;


	glm::vec3 getPointLocation(int w, int h, int d);
	void backprojectSlice(const Detector& detector, const Source& src, const cv::Mat& data, float angle);
	
};

void getPointsInsideObjectCudaKernelLouncher(const ScanObject& obj, ZArray *scanBox, std::vector<glm::vec3>& innerPoints);
