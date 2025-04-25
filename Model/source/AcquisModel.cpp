// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <exception>
#include <iostream>

#include <glm/glm.hpp>

#include <volumeArea.h>


int main()
{
	glm::vec3 center = { 0,0,0 };

	try
	{
		VolumeArea area(center);
		glm::vec3 p = area.getPointLocation(0, 0, 0);
		std::cout << p.x << ' ' << p.y << ' ' << p.z << std::endl;
		glm::vec3 p2 = area.getPointLocation(99, 99, 99);
		std::cout << p2.x << ' ' << p2.y << ' ' << p2.z << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}


