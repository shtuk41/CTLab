#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 texCoord;

out vec2 vTexCoord;

void main()
{
	//position of the vertex in clip space
	gl_Position = vec4(vPosition, 1.0);
	vTexCoord = texCoord;
}