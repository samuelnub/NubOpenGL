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
#include <iostream>

Game::Game(GLFWwindow *windu)
{
	Input::InputSetup(windu);

	G::player.spawn(glm::vec3((gamesettings::SPAWNX),(gamesettings::SPAWNY),(gamesettings::SPAWNZ)));


	Shader shades("shaders/colorsVert.glsl", "shaders/colorsFrag.glsl");
	Shader lampo("shaders/lampVert.glsl", "shaders/lampFrag.glsl");

	Model nanoguy("models/nanosuit/nanosuit.obj");
	Model cuban("models/sphere/sphere.obj");

	nanoguy._uniformFloat["material.shininess"] = 64.0f;
	nanoguy._uniformVec3["light.ambient"] = glm::vec3(0.1f, 0.1f, 0.1f);
	nanoguy._uniformVec3["light.diffuse"] = glm::vec3(1.0f, 1.0f, 1.0f);
	nanoguy._uniformVec3["light.specular"] = glm::vec3(1.0f, 1.0f, 1.0f);
	nanoguy._uniformFloat["light.constant"] = 0.5f;
	nanoguy._uniformFloat["light.linear"] = 0.001f;
	nanoguy._uniformFloat["light.quadratic"] = 0.0002f;


	while (!glfwWindowShouldClose(windu))
	{
		Input::setDelta(glfwGetTime());

		glfwPollEvents();
		G::player.processKeyboard(Input::_keys, Input::_deltaTime);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		G::player.setMatrices(shades);

		//set lamp shader first you dumbo
		lampo.use();

		cuban.translate(glm::vec3(0.0f, 5.0f, 0.0f));
		cuban.rotate((GLfloat)glfwGetTime()*50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		cuban.translate(glm::vec3(5.0f, 0.0f, 0.0f));

		G::player.sendMatrices(lampo);

		cuban.draw(lampo);

		//use shades shaders
		shades.use();

		cuban.setApproxPos();

		glUniform3f(glGetUniformLocation(shades._program, "viewPos"), G::player.getPos().x, G::player.getPos().y, G::player.getPos().z);
		nanoguy._uniformVec3["light.position"] = glm::vec3(cuban.getApproxPos().x, cuban.getApproxPos().y, cuban.getApproxPos().z);

		nanoguy.scale(glm::vec3(0.3, 0.3, 0.3));
		nanoguy.rotate((GLfloat)glfwGetTime()*-20.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		nanoguy.sendUniforms(shades);

		nanoguy.draw(shades);

		G::player.sendMatrices(shades);

		nanoguy.resetModel();
		cuban.resetModel();
		cuban.resetApproxPos();
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
