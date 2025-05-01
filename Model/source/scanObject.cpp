// Model.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <exception>
#include <scanObject.h>
#include <modelExceptions.h>
#include <cad_parser.h>

void ScanObject::Init(const std::string& cadPath, const glm::vec3& offset)
{
	m_data = CadParser::ParseToTriangleMesh(cadPath.c_str(), numberOfTriangles);
}

std::vector<glm::vec3> ScanObject::GetMeshPoints() const
{
	glm::vec4* ptr = GetTrianglesWithOffset();
	std::vector<glm::vec3> vec;

	for (unsigned int ii = 0; ii < GetNumberOfTriangles(); ii++)
	{
		vec.push_back(glm::vec3(*ptr++));
		vec.push_back(glm::vec3(*ptr++));
		vec.push_back(glm::vec3(*ptr++));
	}

	return vec;
}

