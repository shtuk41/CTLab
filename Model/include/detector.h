#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

#include <modelDefs.h>
#include <source.h>

class Detector
{
private:
	glm::vec3 center;

	using YArray = std::array<glm::vec3, nDetectorResY>;
	using ZArray = std::array<YArray, nDetectorResZ>;

	std::unique_ptr<ZArray> plate;
public:
	Detector(const glm::vec3& center = { 0,0,0 }, const glm::vec3& x_axis = { 1,0,0 }, const glm::vec3& y_axis = { 0,1,0 }, const glm::vec3& z_axis = { 0,0,1 });

	const glm::vec3& getCenter() const { return center; }

	glm::vec3 getPixel(int w, int h);
	cv::Mat getPixels(const Source &src, const std::vector<glm::vec3> &areaPoints);
	cv::Mat getPixelsPyramidMethod(const Source& src, const std::vector<glm::vec3>& areaPoints);
};
