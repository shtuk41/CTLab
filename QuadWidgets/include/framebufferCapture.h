#pragma once
#include <memory>
#include <string>

#include <SOIL.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class FramebufferCapture
{
private:
	std::string screenShotName;
public:

	void Setup() { }
	void SaveFrameToImageColor(std::string filename);
	void SaveFrameToImageColorResized(std::string filename, float scale_percent);
	void SaveFrameToImageColorResizedToSize(std::string filename, int saveWidth, int saveHeight);
	void SaveFrameToImageBW(std::string filename);
	void SaveFrameToImageBWResized(std::string filename, float scale_percent);
	void SaveFrameToImageBWResizedToSize(std::string filename, int saveWidth, int saveHeight);
	cv::Mat GetCurrentFrameAsMat();

	std::string& GetScreenshotName();
};

