#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <qevent.h>

class Camera
{
private:
    glm::vec3 g_position;  
	glm::vec3 g_position_offset_x = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 g_position_offset_y = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 g_direction;
	glm::vec3 up;
    float speed;
	float g_initial_fov;
    glm::mat4 g_view_matrix;
    glm::mat4 g_projection_matrix;
    
public:
	Camera(float speed = 3.0f);

    glm::mat4 getViewMatrix() const
    {
        return g_view_matrix;
    }
    
    glm::mat4 getProjectionMatrix() const
    {
        return g_projection_matrix;
    }
    
    glm::vec3 getPosition() const
    {
        return g_position;
    }

	void setPosition(const glm::vec3 &current_pos)
	{
		g_position = current_pos;
	}

	float getFov() const
	{
		return g_initial_fov;
	}

	glm::vec3 getDirection() const
	{
		return g_direction;
	}

	void setDirection(const glm::vec3 &dir)
	{
		g_direction = dir;
	}

	void SetUp(const glm::vec3 &u)
	{
		up = u;
	}

	void rotate(glm::vec3 a, float angle);
	void rotate2(const float x, const float y);
	void rotateX(const float x);
	void rotateY(const float y);
	void rotateZ(const float z);
	void setOffsetX(const float&);
	void setOffsetY(const float&);
	void computeViewProjectionMatrices(int width, int height, bool moveback, bool moveforward, double deltaTime, QKeyEvent* keyEvent);
	void computeViewProjectionMatrices(float orthoLeft, float orthoRight, float orthoBottom, float orthoTop, float orthoNear, float orthoFar);
};