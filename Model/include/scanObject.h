#pragma once

#include <array>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include <modelDefs.h>

typedef struct
{
	glm::vec4* output;
	glm::vec3* normal;
} output_data;

class ScanObject
{
private:
	unsigned int numberOfTriangles;
	output_data m_data;


public:
	ScanObject() { numberOfTriangles = 0; }

	~ScanObject()
	{
		if (m_data.normal != nullptr)
		{
			delete[] m_data.normal;
		}

		if (m_data.output != nullptr)
		{
			delete[] m_data.output;
		}
	}

	void Init(const std::string& cadPath, const glm::vec3 &offset);
	unsigned int GetNumberOfTriangles() const { return numberOfTriangles; }
	glm::vec4* GetTrianglesWithOffset() const { return m_data.output; }


};
