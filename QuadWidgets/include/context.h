#pragma once

#include <memory>
#include <numbers>
#include <string>
#include <vector>
#include <io/ioData.h>

struct Context
{
	Context(std::string volumePath);

	VolumeData volumeData;
	//views
	//GLViewQuadAxial* glViewQuadAxial;
	//GLViewQuadCoronal* glViewQuadCoronal;
	//GLViewQuadSagittal* glViewQuadSagittal;
	//GLViewQuad3D* glViewQuad3d;
};

