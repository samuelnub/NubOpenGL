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
#include "shader.h"
#include <GLFW/glfw3.h>
#include <SOIL\SOIL.h>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//TODO: oriented towards static vertex data currently, might add functions to edit subarray info
class Render
{
private:
	GLuint _VBOid;
	GLuint _VAOid;

	GLboolean _doEBO;
	GLuint _EBOid;

	GLboolean _doTex;
	GLuint _texid;

	glm::mat4 _model;

	GLsizei _VBOsize;
	GLsizei _EBOsize;
public:
	Render();
	~Render();

	//if indexData.empty == 1, dont try to use ebo
	void bind(const std::vector<Vertex> &vertData, const GLchar *texPath, const std::vector<GLushort> &indexData = misc::dummyVec);

	void translate(const glm::vec3 &newPos);
	void scale(const glm::vec3 &newScale);
	void rotate(const GLfloat &newDeg, const glm::vec3 &newAxis);

	glm::mat4 getModel();

	void resetModel();

	void drawVBO();
	void drawEBO();

protected:


};

#endif