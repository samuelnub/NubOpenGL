#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <mesh.h>
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

	void setApproxPos();
	glm::vec3 getApproxPos();
	void resetApproxPos();

	void translate(const glm::vec3 &translation);
	void scale(const glm::vec3 &scale);
	void rotate(const GLfloat &degrees, const glm::vec3 &axis);

	void sendUniforms(Shader &shader);
	void resetModel();

protected:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif