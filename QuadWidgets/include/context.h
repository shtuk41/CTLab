#pragma once

#include <memory>
#include <numbers>
#include <string>
#include <vector>
#include <io/ioData.h>

struct Context
{
	std::function<void()> onDistanceChanged;

	Context(std::string volumePath);
	void setXDistance(float xD);
	void setYDistance(float yD);
	void setZDistance(float zD);

	VolumeData volumeData;
	//views
	//GLViewQuadAxial* glViewQuadAxial;
	//GLViewQuadCoronal* glViewQuadCoronal;
	//GLViewQuadSagittal* glViewQuadSagittal;
	//GLViewQuad3D* glViewQuad3d;

	float xDistance = 0.0f, yDistance = 0.0f, zDistance = 0.0f;
};

