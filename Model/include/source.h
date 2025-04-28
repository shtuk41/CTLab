#pragma once

#include <array>
#include <memory>

#include <glm/glm.hpp>

#include <modelDefs.h>

class Source
{
private:
	glm::vec3 center;
	glm::vec3 direction;

public:
	Source(const glm::vec3& center = { 0,0,0 }, const glm::vec3& direction = { 1,0,0 });

	const glm::vec3& getCenter() const;
	const glm::vec3& getDirection() const;

};
