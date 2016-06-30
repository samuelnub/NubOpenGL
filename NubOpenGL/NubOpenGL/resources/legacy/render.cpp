#include "render.h"
#include <iostream>

Render::Render()
{
}

Render::~Render()
{
	glDeleteVertexArrays(1, &this->_VAOid);
	glDeleteBuffers(1, &this->_VBOid);
	if (this->_doEBO == GL_TRUE)
	{
		glDeleteBuffers(1, &this->_EBOid);
	}
}


void Render::bind(const std::vector<Vertex>& vertData, const GLchar * diffPath, const GLchar * specPath, const std::vector<GLushort>& indexData)
{
	this->_doEBO = (indexData.empty()) ? GL_FALSE : GL_TRUE;
	this->_VBOsize = vertData.size();
	if (this->_doEBO == GL_TRUE)
		this->_EBOsize = sizeof(indexData);

	glGenVertexArrays(1, &this->_VAOid);
	glBindVertexArray(this->_VAOid);

	if (this->_doEBO == GL_TRUE)
	{
		glGenBuffers(1, &this->_EBOid);
		std::cout << "bound ebo\n";
	}

	glGenBuffers(1, &this->_VBOid);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBOid);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->_VBOsize * sizeof(Vertex),
		&vertData[0],
		GL_STATIC_DRAW
		);

	if (this->_doEBO == GL_TRUE)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBOid);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			this->_EBOsize, 
			&indexData[0], 
			GL_STATIC_DRAW
			);
	}

	//coords, first 3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//normals, next 3
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//uv, next 2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); //ps, do not unbind ebo like this, dont unbind it at all, keep it bound to this vao


	//load and bind diffuse map
	glGenTextures(1, &this->_diffMap);
	glBindTexture(GL_TEXTURE_2D, this->_diffMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	int width, height;
	unsigned char *img = SOIL_load_image(diffPath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(img);

	//load and bind specular map, if you're too inept, just give me the diff map or anything lol
	glGenTextures(1, &this->_specMap);
	glBindTexture(GL_TEXTURE_2D, this->_specMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	
	img = SOIL_load_image(specPath, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(img);


	glBindTexture(GL_TEXTURE_2D, 0); //unbind tex when done
}

void Render::translate(const glm::vec3 &newPos)
{
	this->_model = glm::translate(this->_model, newPos);
}

void Render::scale(const glm::vec3 &newScale)
{
	this->_model = glm::scale(this->_model, newScale);
}

void Render::rotate(const GLfloat &newDeg, const glm::vec3 &newAxis)
{
	this->_model = glm::rotate(this->_model, glm::radians(newDeg), newAxis);
}

void Render::setupApproxPos(const std::vector<Vertex> &vertData)
{
	//a very taxing func that should not be used, y'hear me, done once
	GLfloat maxX = 0.0f;
	GLfloat maxY = 0.0f;
	GLfloat maxZ = 0.0f;

	for (int i = 0; i < vertData.size(); i++)
	{
		if (vertData.at(i).x > maxX)
			maxX = vertData.at(i).x;
		if (vertData.at(i).y > maxY)
			maxY = vertData.at(i).y;
		if (vertData.at(i).z > maxZ)
			maxZ = vertData.at(i).z;
	}

	GLfloat minX = maxX;
	GLfloat minY = maxY;
	GLfloat minZ = maxZ;

	for (int i = 0; i < vertData.size(); i++)
	{
		if (vertData.at(i).x < minX)
			minX = vertData.at(i).x;
		if (vertData.at(i).y < minY)
			minY = vertData.at(i).y;
		if (vertData.at(i).z < minZ)
			minZ = vertData.at(i).z;
	}
	
	this->_approxPos.x = maxX - minX;
	this->_approxPos.y = maxY - minY;
	this->_approxPos.z = maxZ - minZ;
}

void Render::setApproxPos()
{
	glm::vec4 tempPos = this->_model * glm::vec4((this->_approxPos), (1.0f));

	this->_approxPos = glm::vec3((tempPos.x), (tempPos.y), (tempPos.z));
}

glm::vec3 Render::getApproxPos()
{
	return glm::vec3(this->_approxPos);
}

glm::mat4 Render::getModel()
{
	return this->_model;
}

void Render::resetApproxPos()
{
	this->_approxPos = glm::vec3();
}

void Render::resetModel()
{
	this->_model = glm::mat4();
}


//this isnt how shaders work you nimbahoon
/*void Render::shade(const char *vSource, const char *fSource)
{
	this->_vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->_vertShader, 1, &vSource, NULL);
	glCompileShader(this->_vertShader);

	GLint succ;
	glGetShaderiv(this->_vertShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt compile vertex shader :(((\n";
	}

	this->_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->_fragShader, 1, &fSource, NULL);
	glCompileShader(this->_fragShader);

	glGetShaderiv(this->_fragShader, GL_COMPILE_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt compile fragment shader :(((\n";
	}

	this->_shaderProgram = glCreateProgram();
	glAttachShader(this->_shaderProgram, this->_vertShader);
	glAttachShader(this->_shaderProgram, this->_fragShader);
	glLinkProgram(this->_shaderProgram);

	glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &succ);
	if (!succ)
	{
		std::cout << "couldnt link shaders into program :(((\n";
	}

	glDeleteShader(this->_vertShader);
	glDeleteShader(this->_fragShader);
}*/

void Render::drawVBO()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->_diffMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->_specMap);

	glBindVertexArray(_VAOid);

	glDrawArrays(GL_TRIANGLES, 0, this->_VBOsize);
	glBindVertexArray(0);
}

void Render::drawEBO()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->_diffMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->_specMap);

	glBindVertexArray(_VAOid);

	glDrawElements(GL_TRIANGLES, this->_EBOsize, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}
