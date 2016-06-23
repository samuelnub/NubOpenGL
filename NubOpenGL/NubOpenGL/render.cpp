#include "render.h"
#include <iostream>

Render::Render()
{
}

Render::~Render()
{
	glDeleteVertexArrays(1, &_VAOid);
	glDeleteBuffers(1, &_VBOid);
}


void Render::bind(const std::vector<Vertex> &vertData)
{
	glGenVertexArrays(1, &this->_VAOid);
	glBindVertexArray(this->_VAOid);

	glGenBuffers(1, &this->_VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBOid);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertData.size() * sizeof(Vertex),
		&vertData[0],
		GL_STATIC_DRAW
		);

	//just coord data for now, every 3 which Vertex encapsulates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Render::shade(const char *vSource, const char *fSource)
{
	this->_vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->_vertShader, 1, &vSource, NULL);
	glCompileShader(this->_vertShader);

	GLint succ;
	glGetShaderiv(this->_vertShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt compile vertex shader :(((\n";
	}

	this->_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->_fragShader, 1, &fSource, NULL);
	glCompileShader(this->_fragShader);

	glGetShaderiv(this->_fragShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt compile fragment shader :(((\n";
	}

	this->_shaderProgram = glCreateProgram();
	glAttachShader(this->_shaderProgram, this->_vertShader);
	glAttachShader(this->_shaderProgram, this->_fragShader);
	glLinkProgram(this->_shaderProgram);

	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt link shaders into program :(((\n";
	}

	glDeleteShader(this->_vertShader);
	glDeleteShader(this->_fragShader);
}

void Render::index()
{
}


void Render::draw()
{
	glUseProgram(this->_shaderProgram);
	glBindVertexArray(_VAOid);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
