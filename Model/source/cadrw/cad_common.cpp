#include <glm/glm.hpp>

/// <summary>
/// Translates a vector to a new coordinate system described by unit vectors ex, ey, ez
/// </summary>
/// <param name="ex">new x unit vector</param>
/// <param name="ey">new y unit vector</param>
/// <param name="ez">new z unit vector</param>
/// <param name="initial_vector"></param>
/// <returns>a vector in new coordinate system</returns>
glm::vec3 translateVec3ToNewCoordinateSystem(const glm::vec3& ex, const glm::vec3& ey, const glm::vec3& ez, const glm::vec3& initial_vector)
{
	glm::vec3 output;

	output.x = ex.x * initial_vector.x + ey.x * initial_vector.y + ez.x * initial_vector.z;
	output.y = ex.y * initial_vector.x + ey.y * initial_vector.y + ez.y * initial_vector.z;
	output.z = ex.z * initial_vector.x + ey.z * initial_vector.y + ez.z * initial_vector.z;

	return output;
}