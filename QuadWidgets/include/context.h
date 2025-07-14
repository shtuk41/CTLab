#pragma once

#include <memory>
#include <numbers>
#include <vector>

#include <glviewquadaxial.h>
#include <glviewquadcoronal.h>
#include <glviewquadsagittal.h>
#include <glviewquad3d.h>

struct Context
{
	Context();
	//views
	GLViewQuadAxial* glViewQuadAxial;
	GLViewQuadCoronal* glViewQuadCoronal;
	GLViewQuadSagittal* glViewQuadSagittal;
	GLViewQuad3D* glViewQuad3d;
};

