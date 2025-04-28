// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <source.h>
#include <modelExceptions.h>

Source::Source(const glm::vec3& center, const glm::vec3& direction) : center(center), direction(direction)
{
}

const glm::vec3& Source::getCenter() const
{
	return center;
}

const glm::vec3& Source::getDirection() const
{
	return direction;
}
