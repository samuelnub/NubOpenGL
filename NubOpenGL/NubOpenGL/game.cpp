#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include "game.h"
#include "globals.h"
#include "model.h"
#include "shader.h"
#include <iostream>

//temp includes for basic const char*-based glsl shaders, and the vertices of a triangle more basic than my mother
#include "shadersTest.h"
#include "verticesTest.h"

Game::Game(GLFWwindow *windu)
{
	Input::InputSetup(windu);

	G::player.spawn(glm::vec3((gamesettings::SPAWNX),(gamesettings::SPAWNY),(gamesettings::SPAWNZ)));


	Shader shades("shaders/simpleVert.glsl", "shaders/simpleFrag.glsl");

	Model nanoguy("models/nanosuit/nanosuit.obj");
	Model cuban("models/cube/cube.obj");

	while (!glfwWindowShouldClose(windu))
	{
		Input::setDelta(glfwGetTime());

		glfwPollEvents();
		G::player.processKeyboard(Input::_keys, Input::_deltaTime);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shades shaders
		shades.use();

		G::player.setMatrices();

		glUniformMatrix4fv(glGetUniformLocation(shades._program, "view"), 1, GL_FALSE, glm::value_ptr(G::player.getView()));
		glUniformMatrix4fv(glGetUniformLocation(shades._program, "projection"), 1, GL_FALSE, glm::value_ptr(G::player.getProj()));

		glm::mat4 model;
		model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shades._program, "model"), 1, GL_FALSE, glm::value_ptr(model));



		cuban.draw(shades);
		nanoguy.draw(shades);

		G::player.resetView();
		G::player.resetProj();


		glfwSwapBuffers(windu);
	}

	glfwTerminate();
	exit(SUCC);
}

Game::~Game()
{

}

//input shazam==============================

std::vector<bool> Input::_keys = std::vector<bool>(1024);
GLfloat Input::_lastX = windowsettings::width / 2;
GLfloat Input::_lastY = windowsettings::height / 2;
bool Input::_firstMouse = true;
GLfloat Input::_deltaTime = 0.0f;
GLfloat Input::_lastFrame = 0.0f;

void Input::InputSetup(GLFWwindow *window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::setDelta(const GLfloat &curFrame)
{
	Input::_deltaTime = curFrame - Input::_lastFrame;
	Input::_lastFrame = curFrame;
}

void Input::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	std::cout << key << " key was pressed\n";

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Input::_keys[key] = true;
		else if (action == GLFW_RELEASE)
			Input::_keys[key] = false;
	}

	//All things that use key input==============================
	//G::player.processKeyboard(Input::_keys, Input::_deltaTime);  //handled each frame instead of called here twice
}

void Input::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{

	//All things that use scroll input===========================
	G::player.processMouseScroll(yoffset);
}

void Input::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (Input::_firstMouse)
	{
		Input::_firstMouse = false;
		Input::_lastX = xpos;
		Input::_lastY = ypos;
	}

	GLfloat xoffset = xpos - Input::_lastX;
	GLfloat yoffset = Input::_lastY - ypos;

	Input::_lastX = xpos;
	Input::_lastY = ypos;

	//All things that use mouse movement input===================
	G::player.processMouseMove(xoffset, yoffset);
}
