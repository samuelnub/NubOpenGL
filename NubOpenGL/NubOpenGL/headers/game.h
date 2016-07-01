#pragma once
#ifndef GAME_H
#define GAME_H

#include <player.h>
#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

//game objects that will init on startup
namespace G
{
	Player player;
	//maybe an empty vector that can fill with "enemy" objects, idk
}


//forward declaring "static" input class
class Input
{
private:


public:
	static void InputSetup(GLFWwindow *window);

	static void setDelta(const GLfloat &curFrame);

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);


	//lord forgive me for having public member variables
	static std::vector<bool> _keys;
	static GLfloat _lastX;
	static GLfloat _lastY;
	static bool _firstMouse;
	static GLfloat _deltaTime;
	static GLfloat _lastFrame;
protected:

};

class Game
{
private:

public:
	Game(GLFWwindow *windu);
	~Game();

protected:


};

#endif