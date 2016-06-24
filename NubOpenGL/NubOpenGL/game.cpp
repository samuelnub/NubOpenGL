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
#include "shader.h"
#include <iostream>

//temp includes for basic const char*-based glsl shaders, and the vertices of a triangle more basic than my mother
#include "shadersTest.h"
#include "verticesTest.h"

Game::Game(GLFWwindow *windu)
{

	Render triangolo;
	Render quaddo;

	Shader shades("shaders/simpleVert.glsl", "shaders/simpleFrag.glsl");

	triangolo.bind(verts::triangle1, "textures/illuminati.png");
	quaddo.bind(verts::quad1, "textures/cooldog.jpg", verts::quad1indices);


	while (!glfwWindowShouldClose(windu))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shades.use();

		triangolo.drawVBO();
		quaddo.drawEBO();
		
		glfwSwapBuffers(windu);
	}

	glfwTerminate();
	exit(SUCC);
}

Game::~Game()
{

}