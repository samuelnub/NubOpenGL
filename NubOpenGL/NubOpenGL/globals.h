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
	//initial player spawn coords
	const GLfloat SPAWNX = 0.0f;
	const GLfloat SPAWNY = 0.0f;
	const GLfloat SPAWNZ = 3.0f;

	// Default camera values
	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 4.0f;
	const GLfloat SENSITIVTY = 0.35f;
	const GLfloat ZOOM = 90.0f;

	namespace controls
	{
		const GLint FORWARD = GLFW_KEY_W;
		const GLint BACKWARD = GLFW_KEY_S;
		const GLint LEFT = GLFW_KEY_A;
		const GLint RIGHT = GLFW_KEY_D;

		const GLint HOME = GLFW_KEY_H;
	}
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