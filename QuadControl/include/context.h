#pragma once

#include <memory>
#include <numbers>
#include <string>
#include <vector>
#include <io/ioData.h>
#include <QOpenGLFunctions_3_3_Core>

struct Context
{
	VolumeData volumeData;
	GLuint tex3D = 0;
	bool initialized = false;
	int width = 0, height = 0, depth = 0;

	Context(const std::string& path);
	void initGL();
};


