#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <memory>
#include <sstream>

#include <context.h>

Context::Context()
{
	volumeData = nullptr;
}

Context::Context(std::string volumePath)
{
	volumeData = std::make_unique<VolumeData>(volumePath);

	volumeData->saveHeaderToFile("headerfile.txt");
	volumeData->fillBuffer();
}

void Context::Init(std::string volumePath)
{
	volumeData = std::make_unique<VolumeData>(volumePath);
}

void Context::setXDistance(float xD)
{
	xDistance = xD;

	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}

void Context::setYDistance(float yD)
{
	yDistance = yD;

	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}

void Context::setZDistance(float zD) 
{ 
	zDistance = zD;
	
	if (onDistanceChanged)
	{
		onDistanceChanged();
	}
}


