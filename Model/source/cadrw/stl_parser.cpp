#include <fstream>
#include <glm/glm.hpp>
#include <stl_parser.h>

//#define OUTPUT_TO_FILE

std::vector<triangle> StlParser::ParseToTriangles(const char* stlFileName, unsigned int& number_of_triangles)
{
	std::ifstream stlFile;

#ifdef OUTPUT_TO_FILE
	std::ofstream dataFile;
#endif

	stlFile.open(stlFileName, std::ios::binary);
#ifdef OUTPUT_TO_FILE
	dataFile.open("stlDataFileDebug.txt", std::ios::trunc);
	dataFile.setf(std::ios_base::fixed);
#endif

	std::vector<triangle> triangles;

	number_of_triangles = 0;
	
	unsigned int numberTriangles;

	STL_HEADER	partHolderHeader;

	if (stlFile.is_open() /*&& dataFile.is_open()*/)
	{
		stlFile.seekg(0, std::ios::beg);
		stlFile.read((char*)&partHolderHeader, sizeof(STL_HEADER));
		stlFile.read((char*)&numberTriangles, sizeof(unsigned int));

#ifdef OUTPUT_TO_FILE
		dataFile << "Number of Triangles:	" << numberTriangles << std::endl;
		dataFile << "Size of Triangle:	" << sizeof(triangle) << std::endl;
		dataFile << "Header size" << sizeof(STL_HEADER) << std::endl;

#endif

		number_of_triangles = numberTriangles;

		char* c = strstr((char*)partHolderHeader.data, "COLOR");

		if (c)
		{
#ifdef OUTPUT_TO_FILE_COLOR
			dataFile << "COLOR FOUND" << std::endl;
#endif
		}

		c = strstr((char*)partHolderHeader.data, "MATERIAL");

		if (c)
		{
#ifdef OUTPUT_TO_FILE_COLOR
			dataFile << "MATERIAL FOUND" << std::endl;
#endif
		}

		for (unsigned int ii = 0; ii < numberTriangles; ii++)
		{
			unsigned int index = ii * 3;
			triangle next;
			size_t sizeofTriangle = sizeof(triangle);
			stlFile.read((char*)&next, 50);

			triangles.push_back(next);

#ifdef OUTPUT_TO_FILE
			//dataFile << ii + 1;
			dataFile << next.norm.data[0] << ',' << next.norm.data[1] << ',' << next.norm.data[2] << ',';
			dataFile << next.points[0].data[0] << ',' << next.points[0].data[1] << ',' << next.points[0].data[2] << ',';

			int blue = (next.atribute_byte_count & 0x001f);
			int green = (next.atribute_byte_count & 0x03e0) >> 5;
			int red = (next.atribute_byte_count & 0x7c00) >> 10;
			int valid = (next.atribute_byte_count & 0x8000) >> 15;

			dataFile << "Blue:	" << blue << std::endl;
			dataFile << "Green	" << green << std::endl;
			dataFile << "Red:	" << red << std::endl;
			dataFile << "Valid:	" << valid << std::endl;

			int iRedWrite = 20 << 10;
			int iGreenWrite = 20 << 5;

			next.atribute_byte_count |= iRedWrite;
			next.atribute_byte_count |= iGreenWrite;
			next.atribute_byte_count |= 1 << 15;

			blue = (next.atribute_byte_count & 0x001f);
			green = (next.atribute_byte_count & 0x03e0) >> 5;
			red = (next.atribute_byte_count & 0x7c00) >> 10;
			valid = (next.atribute_byte_count & 0x8000) >> 15;

			dataFile << "Blue:	" << blue << std::endl;
			dataFile << "Green	" << green << std::endl;
			dataFile << "Red:	" << red << std::endl;
			dataFile << "Valid:	" << valid << std::endl;
#endif
		}
	}

	return triangles;
}