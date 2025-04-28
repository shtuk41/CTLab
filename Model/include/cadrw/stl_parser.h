#pragma once

#include <vector>
#include <cad_common.h>

typedef struct 
{
	unsigned char data[80];
} STL_HEADER;

typedef struct
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
} color;

typedef struct
{
	color	diffuse;
	color	specular;
	color	ambient;
} material;

class StlParser
{
public:
	static output_data ParseToTriangleMesh(const char *stlFileName, unsigned int &number_of_triangles, float size_coef);
	static std::vector<triangle> ParseToTriangles(const char* stlFileName, unsigned int& number_of_triangles);
};