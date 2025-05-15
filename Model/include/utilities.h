#pragma once

#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void saveMatToFIle(const cv::Mat& mat, const std::string &fileName)
{
	cv::imwrite(fileName, mat);
}
