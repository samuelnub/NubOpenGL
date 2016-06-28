#pragma once
#ifndef MESH_H
#define MESH_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW\glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

typedef struct
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec3 uv;
}Vertex;

typedef struct
{
	GLuint id;
	std::string type;
}Texture;

class Shader
{
public:
	GLuint _program;
};

class Mesh
{
private:
	GLuint _vaoID;
	GLuint _vboID;
	GLuint _eboID;

public:
	//lord forgive me for having public member variables
	std::vector<Vertex> _vertices;
	std::vector<GLushort> _indices;
	std::vector<Texture> _textures;

	Mesh(std::vector<Vertex> &vertData, std::vector<GLushort> &indexData, std::vector<Texture> &texData);

	void draw(Shader &shader);

protected:
	void setupMesh();

};

#endif
