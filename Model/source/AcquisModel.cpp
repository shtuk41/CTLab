// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <exception>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <volumeArea.h>

int main()
{
	glm::vec3 center = { 0,0,0 };

	try
	{
		VolumeArea area(center);
		ScanObject obj;
		obj.Init("C:\\Files\\CTLab\\MeshesTest\\sphere500.stl");

		std::vector<glm::vec3> inside = area.getPointsInsideObject(obj);

		std::cout << inside.size() << std::endl;




	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}


