#pragma once

#include <glm/glm.hpp>
#include <cad_common.h>

class StpParser
{
public:
	static std::vector<triangle> ParseToTriangles(const char* stlFileName, unsigned int& number_of_triangles, double meshGenLinearDeflection = 0.08);
};