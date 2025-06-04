// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <detector.h>
#include <source.h>
#include <volumeArea.h>
#include <utilities.h>

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

void rotateZ(std::vector<glm::vec3>& insidePoints, glm::vec3 center, float zDegAngle)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f); 
	glm::vec3 axis(0.0f, 1.0f, 0.0f); 

	// Convert angle to radians
	float angleInRadians = glm::radians(zDegAngle);

	// Create the rotation matrix
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleInRadians, axis);

	// Apply the rotation to the model matrix
	modelMatrix = modelMatrix * rotationMatrix;

	for (auto& ii : insidePoints)
	{
		glm::vec4 tTranslatedOrigin = glm::vec4(ii - center, 1.0);
		glm::vec4 tRotatedOrigin = modelMatrix * tTranslatedOrigin;
		ii = glm::vec3(tRotatedOrigin) + center;
	}
}

int main()
{
	glm::vec3 center = { 200,0,0 };

	try
	{
		VolumeArea area(center);
		ScanObject obj;
		//obj.Init("C:\\Files\\CTLab\\MeshesTest\\wolf10.stl");
		obj.Init("..\\MeshesTest\\wolf10.stl");

		auto scanPoints = obj.GetMeshPoints();
		savePointsToAFile(scanPoints, "object.csv");

		//std::vector<glm::vec3> areaPoints = area.getAllPoints();
		//savePointsToAFile(areaPoints, "area.csv");

		//
		std::vector<glm::vec3> insidePoints = area.getPointsInsideObject(obj);

		std::cout << insidePoints.size() << std::endl;
		savePointsToAFile(insidePoints, "insidePoints.csv");

		glm::vec3 sourceCenter(0, 0, 0);
		glm::vec3 sourceDirection(1, 0, 0);

		Source source(sourceCenter, sourceDirection);

		glm::vec3 detectorCenter(700, 0, 0);
		Detector detector(detectorCenter);

		//scanning & reconstructing
		for (uint ii = 0; ii < numberOfSlices; ii++)
		{
			rotateZ(insidePoints, center, float(360 / numberOfSlices));
			cv::Mat detData = detector.getPixelsPyramidMethod(source, insidePoints);
			std::ostringstream oss;
			oss << "..\\Slices\\Slice" << numberOfSlices << "\\slice" << ii << ".png";
			
			saveMatToFile(detData, oss.str());

			float rotation = 360.0f * ii / numberOfSlices;

			std::cout << "Slice " << ii << " saved\nBackprojection started. Rotation " << std::fixed << std::setprecision(5) << rotation << std::endl;

			if (ii == 0 || ii == 45 || ii == 90)
			{
				area.backprojectSlice(detector, source, detData, rotation);
				std::cout << "Slice " << ii << " backprojection finished.\n";
			}

			//break;
		}

		area.writeFile("area360.raw");
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}


