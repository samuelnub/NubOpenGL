#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <GLFW\glfw3.h>
#include <vector>

enum Errors
{
	SUCC = 0,
	GENERIC_ERROR,
	SUPER_BAD_ERROR
};

namespace gamesettings
{
	enum CamMovement 
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 4.0f;
	const GLfloat SENSITIVTY = 0.35f;
	const GLfloat ZOOM = 90.0f;
}

namespace glsettings
{
	const int majorVer = 3;
	const int minorVer = 3;
}

//TODO: probably gonna read this from a file eventually lol
namespace windowsettings
{
	const int width = 1200;
	const int height = 800;
	const char name[] = "ayy lmao";
}

//TODO: cheap workarounds for not having null references
namespace misc
{
	const short dummyShort = 69;
	const std::vector<GLushort> dummyVec = {};
}


typedef struct
{
	GLfloat x, y, z;
	//GLfloat cx, cy, cz; //dont need colors, pfft, we've got textures now, what is this? the dark ages?
	GLfloat ux, uy;
}Vertex;


#endif