#pragma once

#include <glm/glm.hpp>

typedef struct
{
	float data[3];
} normal_vector;

typedef struct
{ 
	float data[3];
} vertex;

typedef struct
{
	normal_vector	norm;
	vertex			points[3];
	unsigned short	atribute_byte_count;
} triangle;

typedef struct 
{
	glm::vec4* output;
	glm::vec3* normal;
} output_data;

/// <summary>
/// Translates a vector to a new coordinate system described by unit vectors ex, ey, ez
/// </summary>
/// <param name="ex">new x unit vector</param>
/// <param name="ey">new y unit vector</param>
/// <param name="ez">new z unit vector</param>
/// <param name="initial_vector"></param>
/// <returns>a vector in new coordinate system</returns>
glm::vec3 translateVec3ToNewCoordinateSystem(const glm::vec3& ex, const glm::vec3& ey, const glm::vec3& ez, const glm::vec3& initial_vector);