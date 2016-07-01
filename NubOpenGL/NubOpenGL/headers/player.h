#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>         /// remove the "3" for OpenGL versions < 3
#include <OpenGL/gl3ext.h>      /// ditto
#else 
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <shader.h>
#include <globals.h>
#include <vector>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Player
{
private:
	glm::vec3 _pos;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	GLfloat _yaw;
	GLfloat _pitch;

	GLfloat _moveSpeed;
	GLfloat _mouseSensitivity;
	GLfloat _zoom;

	glm::mat4 _view;
	glm::mat4 _projection;

public:
	Player();
	~Player();

	void spawn(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = gamesettings::YAW, GLfloat pitch = gamesettings::PITCH);

	void setMatrices(Shader &shader);

	glm::vec3 getPos();
	glm::mat4 getView();
	glm::mat4 getProj();

	void resetView();
	void resetProj();

	void processKeyboard(const std::vector<bool> &keys, GLfloat &deltaTime);
	void processMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(GLfloat yoffset);

	void sendMatrices(Shader &shader);

protected:
	void updateVectors();

};


#endif