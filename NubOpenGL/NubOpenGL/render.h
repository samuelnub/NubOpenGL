#pragma once
#ifndef RENDER_H
#define RENDER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include "globals.h"
#include <GLFW/glfw3.h>
#include <vector>

//TODO: oriented towards static vertex data currently, might add functions to edit subarray info
class Render
{
private:

	std::vector<Vertex> _vertices;

	GLuint _VBOid;
	GLuint _VAOid;
	GLuint _EBOid;

	GLuint _vertShader;
	GLuint _fragShader;
	GLuint _shaderProgram;

public:
	//when an instance of this is made, take a reference of the external vertex data and put it in this->_vertices array
	Render(const std::vector<Vertex> &vertData);
	~Render();

	void bind();

	void shade(const char *vSource, const char *fSource);

	void index();

	void draw();

protected:


};

#endif