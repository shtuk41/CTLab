#include <algorithm>
#include <vector>

#include <cad_parser.h>
#include <stl_parser.h>
#include <stp_parser.h>

static std::string getFileExtension(std::string filepath)
{
	size_t dotLocation = filepath.rfind('.');

	if (dotLocation != std::string::npos) {

		auto extension = filepath.substr(dotLocation + 1);

		std::transform(extension.begin(),
			extension.end(),
			extension.begin(),
						[](unsigned char c) { return std::tolower(c); });

		return extension;

	}
	else {
		return "";
	}
}


output_data CadParser::ParseToTriangleMesh(const char *filePath, unsigned int &numberOfTriangles, const double stpLinearDeflection, const float sizeCoef)
{
	// offset file
	float xoffset = 0.0f;
	float yoffset = 0.0f;
	float zoffset = 0.0f;

	glm::vec3 mi_x_axis = { 1,0,0 };
	glm::vec3 mi_y_axis = { 0,1,0 };
	glm::vec3 mi_z_axis = { 0,0,1 };

	//base coordinate system file

	float xoffset2 = 0.0f;
	float yoffset2 = 0.0f;
	float zoffset2 = 0.0f;

	glm::vec3 mi_x_axis2 = { 1,0,0 };
	glm::vec3 mi_y_axis2 = { 0,1,0 };
	glm::vec3 mi_z_axis2 = { 0,0,1 };

	std::ifstream stlFile;

#ifdef OUTPUT_TO_FILE
	std::ofstream dataFile;
#endif

	output_data data;
	data.output = nullptr;
	data.normal = nullptr;
	numberOfTriangles = 0;
	unsigned int numberTriangles;

	std::vector<triangle> triangles;

	std::string extension = getFileExtension(std::string(filePath));

	//stl or stp?
	if (extension.compare("stl") == 0)
	{
		triangles = StlParser::ParseToTriangles(filePath, numberTriangles);
		numberOfTriangles = numberTriangles;
	}
	else if (extension.compare("stp") == 0)
	{
		triangles = StpParser::ParseToTriangles(filePath, numberTriangles, stpLinearDeflection);
		numberOfTriangles = numberTriangles;
	}
	else
	{
		return data;
	}

#ifdef OUTPUT_TO_FILE
	dataFile << "Number of Triangles:	" << numberTriangles << std::endl;
#endif

	data.output = new glm::vec4[numberTriangles * 3];
	data.normal = new glm::vec3[numberTriangles * 3];
	numberOfTriangles = numberTriangles;

	for (unsigned int ii = 0; ii < numberTriangles; ii++)
	{
		unsigned int index = ii * 3;
		triangle next = triangles[ii];

#ifdef OUTPUT_TO_FILE
			//dataFile << ii + 1;
			dataFile << next.norm.data[0] << ',' << next.norm.data[1] << ',' << next.norm.data[2] << ',';
#endif

		glm::vec3 normal1 =  translateVec3ToNewCoordinateSystem(mi_x_axis, mi_y_axis, mi_z_axis, glm::vec3(next.norm.data[0], next.norm.data[1], next.norm.data[2]));

		data.normal[index] =
		data.normal[index + 1] =
		data.normal[index + 2] = translateVec3ToNewCoordinateSystem(mi_x_axis2, mi_y_axis2, mi_z_axis2, normal1);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[0].data[0] << ',' << next.points[0].data[1] << ',' << next.points[0].data[2] << ',';
#endif	

		glm::vec3 tri1 = glm::vec3(next.points[0].data[0] * sizeCoef, next.points[0].data[1] * sizeCoef, next.points[0].data[2] * sizeCoef);
		glm::vec3 tri1_new = translateVec3ToNewCoordinateSystem(mi_x_axis, mi_y_axis, mi_z_axis, tri1);
		glm::vec3 tri1_offset1 = glm::vec3(tri1_new.x + xoffset, tri1_new.y + yoffset, tri1_new.z + zoffset);

		glm::vec3 tri1_off2_new = translateVec3ToNewCoordinateSystem(mi_x_axis2, mi_y_axis2, mi_z_axis2, tri1_offset1);
		data.output[index] = glm::vec4(tri1_off2_new.x + xoffset2, tri1_off2_new.y + yoffset2, tri1_off2_new.z + zoffset2, 1.0f);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[1].data[0] << ',' << next.points[1].data[1] << ',' << next.points[1].data[2] << ',';
#endif		
		glm::vec3 tri2 = glm::vec3(next.points[1].data[0] * sizeCoef, next.points[1].data[1] * sizeCoef, next.points[1].data[2] * sizeCoef);
		glm::vec3 tri2_new = translateVec3ToNewCoordinateSystem(mi_x_axis, mi_y_axis, mi_z_axis, tri2);
		glm::vec3 tri2_offset1 = glm::vec3(tri2_new.x + xoffset, tri2_new.y + yoffset, tri2_new.z + zoffset);

		glm::vec3 tri2_off2_new = translateVec3ToNewCoordinateSystem(mi_x_axis2, mi_y_axis2, mi_z_axis2, tri2_offset1);
		data.output[index + 1] = glm::vec4(tri2_off2_new.x + xoffset2, tri2_off2_new.y + yoffset2, tri2_off2_new.z + zoffset2, 1.0f);

#ifdef OUTPUT_TO_FILE
			dataFile << next.points[2].data[0] << ',' << next.points[2].data[1] << ',' << next.points[2].data[2] << ",\n";
#endif
		glm::vec3 tri3 = glm::vec3(next.points[2].data[0] * sizeCoef, next.points[2].data[1] * sizeCoef, next.points[2].data[2] * sizeCoef);
		glm::vec3 tri3_new = translateVec3ToNewCoordinateSystem(mi_x_axis, mi_y_axis, mi_z_axis, tri3);
		glm::vec3 tri3_offset1 = glm::vec3(tri3_new.x + xoffset, tri3_new.y + yoffset, tri3_new.z + zoffset);

		glm::vec3 tri3_off2_new = translateVec3ToNewCoordinateSystem(mi_x_axis2, mi_y_axis2, mi_z_axis2, tri3_offset1);
		data.output[index + 2] = glm::vec4(tri3_offset1.x + xoffset2, tri3_offset1.y + yoffset2, tri3_offset1.z + zoffset2, 1.0f);
	}

	return data;
}