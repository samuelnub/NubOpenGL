#include "model.h"
#include "globals.h"

#include <SOIL\SOIL.h>

Model::Model(GLchar *path)
{
	this->loadModel(path);
}

void Model::draw(Shader &shader)
{
	glUniformMatrix4fv(glGetUniformLocation(shader._program, "model"), 1, GL_FALSE, glm::value_ptr(this->_modelMat));

	for (GLuint i = 0; i < this->_meshes.size(); i++)
	{
		this->_meshes[i].draw(shader);
	}

	this->_modelMat = glm::mat4();
}

void Model::loadModel(std::string path)
{
	Assimp::Importer impotent;

	const aiScene *scene = impotent.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "couldn't use assimp to import model :(((\n" << impotent.GetErrorString() << "\n";
		exit(GENERIC_ERROR);
	}
	this->_directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	//recur me buddy
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		this->_meshes.push_back(this->processMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		//Pos
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;
		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.norm = vector;

		//check if it has uv coords (not compulsory)
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 uv;

			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;

			vertex.uv = uv;
		}
		else
		{
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		//assume all proper shaders follow this uniform naming convention
		std::vector<Texture> diffMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffMaps.begin(), diffMaps.end());

		std::vector<Texture> specMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specMaps.begin(), specMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;
		for (GLuint j = 0; j < this->_texturesLoaded.size(); j++)
		{
			if (this->_texturesLoaded[j].path == str)
			{
				textures.push_back(this->_texturesLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture tex;
			tex.id = textureFromFile(str.C_Str(), this->_directory);
			tex.type = typeName;
			tex.path = str;
			textures.push_back(tex);
			this->_texturesLoaded.push_back(tex);
		}
	}

	return textures;
}


GLuint Model::textureFromFile(const char * path, std::string directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	std::cout << filename << "\n";

	GLuint texID;
	glGenTextures(1, &texID);

	int width;
	int height;
	unsigned char *img = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(img);

	return texID;
}

void Model::calcApproxPos()
{
	GLfloat maxX = 0.0f;
	GLfloat maxY = 0.0f;
	GLfloat maxZ = 0.0f;

	for (GLuint i = 0; i < this->_meshes.size(); i++)
	{
		for (GLuint j = 0; j < this->_meshes[i]._vertices.size(); j++)
		{
			if (this->_meshes[i]._vertices[j].pos.x > maxX)
				maxX = this->_meshes[i]._vertices[j].pos.x;
			if (this->_meshes[i]._vertices[j].pos.y > maxY)
				maxY = this->_meshes[i]._vertices[j].pos.y;
			if (this->_meshes[i]._vertices[j].pos.z > maxZ)
				maxZ = this->_meshes[i]._vertices[j].pos.z;
		}
	}

	GLfloat minX = maxX;
	GLfloat minY = maxY;
	GLfloat minZ = maxZ;

	for (GLuint i = 0; i < this->_meshes.size(); i++)
	{
		for (GLuint j = 0; j < this->_meshes[i]._vertices.size(); j++)
		{
			if (this->_meshes[i]._vertices[j].pos.x < minX)
				minX = this->_meshes[i]._vertices[j].pos.x;
			if (this->_meshes[i]._vertices[j].pos.y < minY)
				minY = this->_meshes[i]._vertices[j].pos.y;
			if (this->_meshes[i]._vertices[j].pos.z < minZ)
				minZ = this->_meshes[i]._vertices[j].pos.z;
		}
	}

	this->_approxPos.x = maxX - minX;
	this->_approxPos.y = maxY - minY;
	this->_approxPos.z = maxZ - minZ;
}

glm::vec3 Model::getApproxPos()
{
	return this->_approxPos;
}

void Model::translate(glm::vec3 & translation)
{
	this->_modelMat = glm::translate(this->_modelMat, translation);
}

void Model::scale(glm::vec3 & scale)
{
	this->_modelMat = glm::scale(this->_modelMat, scale);
}

void Model::rotate(GLfloat & degrees, glm::vec3 & axis)
{
	this->_modelMat = glm::rotate(this->_modelMat, glm::radians(degrees), axis);
}

void Model::sendUniforms(Shader &shader)
{
	if (!this->_uniformVec3.empty())
	{
		std::map<const GLchar *, glm::vec3>::const_iterator iter;
		iter = this->_uniformVec3.begin();

		while (iter != _uniformVec3.end())
		{
			glUniform3f(glGetUniformLocation(shader._program, iter->first), iter->second.x, iter->second.y, iter->second.z);
		}
	}

	if (!this->_uniformMat4.empty())
	{
		std::map<const GLchar *, glm::mat4>::const_iterator iter;
		iter = this->_uniformMat4.begin();

		while (iter != _uniformMat4.end())
		{
			glUniformMatrix4fv(glGetUniformLocation(shader._program, iter->first), 1, GL_FALSE, glm::value_ptr(iter->second));
		}
	}

	if (!this->_uniformFloat.empty())
	{
		std::map<const GLchar *, GLfloat>::const_iterator iter;
		iter = this->_uniformFloat.begin();

		while (iter != _uniformFloat.end())
		{
			glUniform1f(shader._program, iter->second), iter->first);
			//swapped around cause iterator chose the glfloats to be the iterating key
		}
	}
}
