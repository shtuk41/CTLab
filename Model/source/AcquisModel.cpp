// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <detector.h>
#include <source.h>
#include <volumeArea.h>

void savePointsToAFile(const std::vector<glm::vec3>& points, const std::string fileName)
{
	if (points.size() == 0)
		return;

	std::ofstream file(fileName.c_str());

	if (file.is_open())
	{
		for (auto a : points)
		{
			file << a.x << ',' << a.y << ',' << a.z << '\n';
		}

		file.close();
	}
}

int main()
{
	glm::vec3 center = { 0,0,0 };

	try
	{
		VolumeArea area(center);
		ScanObject obj;
		obj.Init("C:\\Files\\CTLab\\MeshesTest\\sphere500.stl");

		auto scanPoints = obj.GetMeshPoints();
		savePointsToAFile(scanPoints, "object.csv");

		std::vector<glm::vec3> areaPoints = area.getAllPoints();
		savePointsToAFile(areaPoints, "area.csv");

		std::vector<glm::vec3> insidePoints = area.getPointsInsideObject(obj);

		std::cout << insidePoints.size() << std::endl;
		savePointsToAFile(insidePoints, "insidePoints.csv");

		
		Source source;
		Detector detetector(glm::vec3(1000,0,0));
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}


