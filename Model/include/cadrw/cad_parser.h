#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <cad_common.h>

class CadParser
{
public:
	static output_data ParseToTriangleMesh(const char *filePath, unsigned int & numberOfTriangles, const double stpLinearDeflection = 0.01, const float sizeCoef = 1.0f);
};