#pragma once

#include <memory>
#include <numbers>
#include <string>
#include <vector>
#include <io/ioData.h>
#include <QOpenGLFunctions_3_3_Core>

struct Context
{
	std::function<void()> onDistanceChanged;
	std::function<void()> onVolumeChanged;

	void setXDistance(float xD);
	void setYDistance(float yD);
	void setZDistance(float zD);

	VolumeData volumeData;
	GLuint tex3D = 0;
	bool initialized = false;
	int width = 0, height = 0, depth = 0;
	float xDistance = 0.0f, yDistance = 0.0f, zDistance = 0.0f;

	Context(const std::string& path);
	void initGL();

	bool setVolume(const std::string& path);
};