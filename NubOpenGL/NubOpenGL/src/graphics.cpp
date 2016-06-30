#include "graphics.h"
#include "globals.h"
#include <iostream>


Graphics::Graphics()
{


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glsettings::majorVer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glsettings::minorVer);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	this->_windu = glfwCreateWindow(windowsettings::width, windowsettings::height, windowsettings::name, nullptr, nullptr);
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_STENCIL_TEST);
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	Game gammond(this->_windu);
}

Graphics::~Graphics()
{

}