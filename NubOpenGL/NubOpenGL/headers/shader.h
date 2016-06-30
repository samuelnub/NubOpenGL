#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Shader
{
private:

public:
	GLuint _program;
	Shader(const GLchar *vertPath, const GLchar *fragPath);

	//these functions dont gosh darn work
	void send(GLuint location, GLfloat value);
	void send(GLuint location, glm::mat4 const &matrix);
	template<typename T> 
	Shader &send(std::string const &name, T&& value)
	{
		int location = glGetUniformLocation(this->_program, name.c_str());
		if (location == -1)
			std::cout << "missing uniform " << name.c_str() << ":(((\n";
		else
			this->send(location, std::forward<T>(value));
		return *this;
	}



	void use();

};

#endif