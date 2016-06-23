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
	GLuint _VBOid;
	GLuint _VAOid;
	GLboolean _doEBO;
	GLuint _EBOid;

	GLuint _VBOsize;
	GLuint _EBOsize;

	GLuint _vertShader;
	GLuint _fragShader;
	GLuint _shaderProgram;

public:
	Render();
	~Render();

	//if indexData.empty == 1, dont try to use ebo
	void bind(const std::vector<Vertex> &vertData, const std::vector<GLushort> &indexData = misc::dummyVec);

	void shade(const char *vSource, const char *fSource);

	void draw();

protected:


};

#endif