#include "game.h"
#include "globals.h"
#include <iostream>

Game::Game(GLFWwindow *windu)
{
	while (!glfwWindowShouldClose(windu))
	{
		glfwPollEvents();
		glfwSwapBuffers(windu);
	}

	glfwTerminate();
	exit(SUCC);
}

Game::~Game()
{

}