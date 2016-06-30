#include "player.h"
#include <math.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::spawn(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
{
	this->_front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->_moveSpeed = gamesettings::SPEED;
	this->_mouseSensitivity = gamesettings::SENSITIVTY;
	this->_zoom = gamesettings::ZOOM;

	this->_pos = position;
	this->_worldUp = up;
	this->_yaw = yaw;
	this->_pitch = pitch;
	this->updateVectors();
}

void Player::setMatrices(Shader &shader)
{
	this->_view = glm::lookAt(this->_pos, this->_pos + this->_front, this->_up);

	this->_projection = glm::perspective(this->_zoom, (GLfloat)windowsettings::width / (GLfloat)windowsettings::height, 0.1f, 1000.0f);

	//assume your shader names it "view" and "projection" too
	glUniformMatrix4fv(glGetUniformLocation(shader._program, "view"), 1, GL_FALSE, glm::value_ptr(this->_view));
	glUniformMatrix4fv(glGetUniformLocation(shader._program, "projection"), 1, GL_FALSE, glm::value_ptr(this->_projection));
}

glm::vec3 Player::getPos()
{
	return this->_pos;
}

glm::mat4 Player::getView()
{
	return this->_view;
}

glm::mat4 Player::getProj()
{
	return this->_projection;
}

void Player::resetView()
{
	this->_view = glm::mat4();
}

void Player::resetProj()
{
	this->_projection = glm::mat4();
}

void Player::processKeyboard(const std::vector<bool> &keys, GLfloat &deltaTime)
{
	GLfloat vel = this->_moveSpeed * deltaTime;
	if (keys.at(gamesettings::controls::FORWARD))
		this->_pos += this->_front * vel;
	if (keys.at(gamesettings::controls::BACKWARD))
		this->_pos -= this->_front * vel;
	if (keys.at(gamesettings::controls::LEFT))
		this->_pos -= this->_right * vel;
	if (keys.at(gamesettings::controls::RIGHT))
		this->_pos += this->_right * vel;

	if (keys.at(gamesettings::controls::HOME))
		this->_pos = glm::vec3((gamesettings::SPAWNX), (gamesettings::SPAWNY), (gamesettings::SPAWNZ));
}

void Player::processMouseMove(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->_mouseSensitivity;
	yoffset *= this->_mouseSensitivity;

	this->_yaw = std::fmod((this->_yaw + xoffset), (GLfloat)360.0f);
	this->_pitch += yoffset;

	//cant look too high or low
	if (constrainPitch)
	{
		if (this->_pitch > 89.0f)
			this->_pitch = 89.0f;
		if (this->_pitch < -89.0f)
			this->_pitch = -89.0f;
	}

	//update front xyz vectors, "which way is 'front' now?"
	this->updateVectors();
}

void Player::processMouseScroll(GLfloat yoffset)
{
	if (this->_zoom >= 1.0f && this->_zoom <= gamesettings::ZOOM)
		this->_zoom -= yoffset - (yoffset/2);
	if (this->_zoom <= 1.0f)
		this->_zoom = 1.0f;
	if (this->_zoom >= gamesettings::ZOOM)
		this->_zoom = gamesettings::ZOOM;
}

void Player::sendMatrices(Shader & shader)
{
	//although setmatrices sends them, if you use multiple shaders, you gotta do this

	glUniformMatrix4fv(glGetUniformLocation(shader._program, "view"), 1, GL_FALSE, glm::value_ptr(this->_view));
	glUniformMatrix4fv(glGetUniformLocation(shader._program, "projection"), 1, GL_FALSE, glm::value_ptr(this->_projection));
}

void Player::updateVectors()
{
	//calc the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	front.y = sin(glm::radians(this->_pitch));
	front.z = sin(glm::radians(this->_yaw)) * cos(glm::radians(this->_pitch));
	this->_front = glm::normalize(front);
	
	//recalculate the Right and Up vector
	this->_right = glm::normalize(glm::cross(this->_front, this->_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->_up = glm::normalize(glm::cross(this->_right, this->_front));
}
