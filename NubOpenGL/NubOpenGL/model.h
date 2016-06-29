#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <string>
#include <vector>

class Model
{
private:
	std::vector<Mesh> _meshes;
	std::string _directory;

	std::vector<Texture> _texturesLoaded;

public:
	Model(GLchar *path);
	void draw(Shader &shader);

	static GLuint textureFromFile(const char *path, std::string directory);

protected:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif