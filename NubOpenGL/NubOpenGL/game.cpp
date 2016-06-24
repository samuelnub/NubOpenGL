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

//probably temp, will move matrix operations to a separate class eventually
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//temp includes for basic const char*-based glsl shaders, and the vertices of a triangle more basic than my mother
#include "shadersTest.h"
#include "verticesTest.h"

Game::Game(GLFWwindow *windu)
{
	Render triangolo;
	Render quaddo;
	Render cuban;

	Shader shades("shaders/simpleVert.glsl", "shaders/simpleFrag.glsl");

	triangolo.bind(verts::triangle1, "textures/illuminati.png");
	quaddo.bind(verts::quad1, "textures/cooldog.jpg", verts::quad1indices);
	cuban.bind(verts::cube1, "textures/cromulon.png");

	while (!glfwWindowShouldClose(windu))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shades.use();

		//true mvp :'))) will probably abstract to separate class


		glm::mat4 vue;
		glm::mat4 proyek;
		
		vue = glm::translate(vue, glm::vec3(0.0f, 0.0f, -3.0f)); //where we are in the world

		proyek = glm::perspective(glm::radians(40.0f), (GLfloat)windowsettings::width / (GLfloat)windowsettings::height, 0.1f, 100.0f);

		//prepare their uniforms to pass to shaders
		GLint modelLoc = glGetUniformLocation(shades._program, "model");
		GLint viewLoc = glGetUniformLocation(shades._program, "view");
		GLint projLoc = glGetUniformLocation(shades._program, "projection");

		//pass to shaders
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vue));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proyek));

		cuban.rotate((GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.8f, 0.0f, 3.0f));
		cuban.translate(glm::vec3(0.1f, 0.2f, -5.0f));

		triangolo.rotate((GLfloat)glfwGetTime() * -80.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		quaddo.rotate((GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 0.0f, 0.2f));
		quaddo.translate(glm::vec3(0.4f, 0.0f, 0.0f));

		triangolo.drawVBO(modelLoc);
		quaddo.drawEBO(modelLoc);
		cuban.drawVBO(modelLoc);
		
		glfwSwapBuffers(windu);
	}

	glfwTerminate();
	exit(SUCC);
}

Game::~Game()
{

}