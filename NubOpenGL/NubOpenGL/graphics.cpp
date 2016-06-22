#include "graphics.h"
#include "globals.h"
#include "game.h"
#include <iostream>


Graphics::Graphics()
{


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glsettings::majorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glsettings::minorVer);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	this->_windu = glfwCreateWindow(windowsettings::width, windowsettings::height, "ayy", nullptr, nullptr);
	if (this->_windu == nullptr)
	{
		std::cout << "couldnt create glfw window :(((\n";
		glfwTerminate();
		exit(GENERIC_ERROR);
	}

	glfwMakeContextCurrent(this->_windu);

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "GLEW couldnt init :((((\n" << std::endl;
		exit(GENERIC_ERROR);
	}
#endif

	int width, height;
	glfwGetFramebufferSize(this->_windu, &width, &height);
	glViewport(0, 0, width, height);

	Game gammond(this->_windu);
}

Graphics::~Graphics()
{

}