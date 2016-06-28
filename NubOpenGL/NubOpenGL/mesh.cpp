#include "mesh.h"

#include <sstream>

Mesh::Mesh(std::vector<Vertex>& vertData, std::vector<GLushort>& indexData, std::vector<Texture>& texData)
{
	this->_vertices = vertData;
	this->_indices = indexData;
	this->_textures = texData;

	this->setupMesh();
}

void Mesh::draw(Shader & shader)
{
	//assume the shader and the textures you give have the naming convention "texture_diffuse<n>" in a "material." struct in your shader, etc.

	GLuint diffuseN = 1;
	GLuint specularN = 1;

	for (GLuint i = 0; i < this->_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string number;
		std::string name = this->_textures[i].type;

		if (name == "texture_diffuse")
			ss << diffuseN++;
		else if (name == "texture_specular")
			ss << specularN++;
		number = ss.str();
		
		glUniform1f(glGetUniformLocation(shader._program, ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->_textures[i].id);
	}

	glBindVertexArray(this->_vaoID);
	glDrawElements(GL_TRIANGLES, this->_indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->_vaoID);
	glGenBuffers(1, &this->_vboID);
	glGenBuffers(1, &this->_eboID);

	glBindVertexArray(this->_vaoID);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vboID);
	glBufferData(
		GL_ARRAY_BUFFER,
		this->_vertices.size() * sizeof(Vertex),
		&this->_vertices[0],
		GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, norm)); //offsetof, what a nifty lil function

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, uv));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_eboID);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		this->_indices.size() * sizeof(GLushort),
		&this->_vertices[0],
		GL_STATIC_DRAW);

	glBindVertexArray(0);
}
