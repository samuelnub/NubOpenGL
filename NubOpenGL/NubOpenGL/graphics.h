#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

class Game 
{
public:
	Game(GLFWwindow *windu);
};

class Graphics
{
private:
	GLFWwindow *_windu;

public:
	Graphics();
	~Graphics();

	

protected:


};

#endif