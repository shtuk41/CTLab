#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <memory>
#include <sstream>

#include <context.h>

Context::Context(std::string volumePath) : volumeData(volumePath)
{
	
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


