#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include "game.h"
#include "globals.h"
#include "render.h"
#include <iostream>

//temp includes for basic const char*-based glsl shaders, and the vertices of a triangle more basic than my mother
#include "shadersTest.h"
#include "verticesTest.h"

Game::Game(GLFWwindow *windu)
{

	Render triangolo;

	triangolo.bind(verts::triangle1);
	triangolo.shade(shaders::vertex, shaders::fragment);

	while (!glfwWindowShouldClose(windu))
	{
		glfwPollEvents();
		glfwSwapBuffers(windu);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		triangolo.draw();
	}

	glfwTerminate();
	exit(SUCC);
}

Game::~Game()
{

}