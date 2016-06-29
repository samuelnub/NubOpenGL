#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <string>
#include <vector>
#include <map>

class Model
{
private:
	std::vector<Mesh> _meshes;
	std::string _directory;

	std::vector<Texture> _texturesLoaded;

	glm::vec3 _approxPos;
	glm::mat4 _modelMat;

public:
	//lord forgive my public member variables
	std::map<const GLchar *, glm::vec3> _uniformVec3;
	std::map<const GLchar *, glm::mat4> _uniformMat4;
	std::map<const GLchar *, GLfloat> _uniformFloat;



	Model(GLchar *path);
	void draw(Shader &shader);

	static GLuint textureFromFile(const char *path, std::string directory);

	void calcApproxPos(); //calcs center based on min. bounding box
	glm::vec3 getApproxPos();

	void translate(glm::vec3 &translation);
	void scale(glm::vec3 &scale);
	void rotate(GLfloat &degrees, glm::vec3 &axis);

	void sendUniforms(Shader &shader);

protected:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif