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
	Input::InputSetup(windu);

	G::player.spawn(glm::vec3((gamesettings::SPAWNX),(gamesettings::SPAWNY),(gamesettings::SPAWNZ)));

	Render triangolo;
	Render quaddo;
	Render cuban;
	Render cubanlazy;
	Render cubanlamp;
	glm::vec3 cubanlampPos;

	Shader shades("shaders/colorsVert.glsl", "shaders/colorsFrag.glsl");
	Shader lampo("shaders/lampVert.glsl", "shaders/lampFrag.glsl");

	triangolo.bind(verts::triangle1, "textures/illuminati.png");
	quaddo.bind(verts::quad1, "textures/cooldog.jpg", verts::quad1indices);
	cuban.bind(verts::cube1, "textures/cromulon.png");
	cubanlazy.bind(verts::cube1, "textures/cromulon.png");
	cubanlamp.bind(verts::cube1, "textures/cooldog.jpg");

	while (!glfwWindowShouldClose(windu))
	{
		Input::setDelta(glfwGetTime());

		glfwPollEvents();
		G::player.processKeyboard(Input::_keys, Input::_deltaTime);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLint modelLoc, viewLoc, projLoc;


		//use lamp shader
		lampo.use();

		modelLoc = glGetUniformLocation(lampo._program, "model");
		viewLoc = glGetUniformLocation(lampo._program, "view");
		projLoc = glGetUniformLocation(lampo._program, "projection");
		//the lamp shader also needs to know the view and proj matrices, although our "camera" is using the shades shader! tldr just send the mvp to every shader (if it supports it)

		//cubanlamp.rotate((GLfloat)glfwGetTime() * 40.0f, glm::vec3(0.7f, 0.2f, 0.8f));
		cubanlamp.translate(glm::vec3(1.0f, 0.4f, 4.0f));

		std::cout << cubanlamp.getPos().x << " " << cubanlamp.getPos().y << " " << cubanlamp.getPos().z << "\n";

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubanlamp.getModel()));
		cubanlamp.drawVBO();

		G::player.setMatrices(); //"translate" the player

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(G::player.getView()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(G::player.getProj()));





		//use shades , btw, doesnt accept textures atm
		shades.use();

		//specific uniforms to send to ^shades, besides the usual mvp
		GLint objectColorLoc = glGetUniformLocation(shades._program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shades._program, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shades._program, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shades._program, "viewPos");
		glUniform3f(objectColorLoc, 0.8f, 0.5f, 0.7f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, cubanlamp.getPos().x, cubanlamp.getPos().y, cubanlamp.getPos().z);
		glUniform3f(viewPosLoc, G::player.getPos().x, G::player.getPos().y, G::player.getPos().z);


		modelLoc = glGetUniformLocation(shades._program, "model");
		viewLoc = glGetUniformLocation(shades._program, "view");
		projLoc = glGetUniformLocation(shades._program, "projection");

		cuban.rotate((GLfloat)glfwGetTime() * 30.0f, glm::vec3(0.8f, 0.0f, 3.0f));
		cuban.translate(glm::vec3(0.1f, 0.2f, -5.0f));

		cubanlazy.translate(glm::vec3(1.0f, 2.0f, 2.5f));

		triangolo.translate(glm::vec3(1.0f, 4.0f, 0.2f));
		triangolo.rotate((GLfloat)glfwGetTime() * -80.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		quaddo.rotate((GLfloat)glfwGetTime() * 50.0f, glm::vec3(0.5f, 0.0f, 0.2f));
		quaddo.translate(glm::vec3(0.4f, 0.0f, 0.0f));


		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(triangolo.getModel()));
		triangolo.drawVBO();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(quaddo.getModel()));
		quaddo.drawEBO();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cubanlazy.getModel()));
		cubanlazy.drawVBO();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cuban.getModel()));
		cuban.drawVBO();

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(G::player.getView()));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(G::player.getProj()));






		//reset all these gosh darn matrices
		triangolo.resetModel();
		quaddo.resetModel();
		cuban.resetModel();
		cubanlazy.resetModel();
		cubanlamp.resetModel();

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
