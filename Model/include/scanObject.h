#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include <cad_common.h>
#include <modelDefs.h>

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

	void Init(const std::string& cadPath, const glm::vec3 &offset = { 0,0,0 });
	unsigned int GetNumberOfTriangles() const { return numberOfTriangles; }
	glm::vec4* GetTrianglesWithOffset() const { return m_data.output; }
	std::vector<glm::vec3> GetMeshPoints() const;
};

using XArray = std::vector<glm::vec3>;
using YArray = std::vector<XArray>;
using ZArray = std::vector<YArray>;
