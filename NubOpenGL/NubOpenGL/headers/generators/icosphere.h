#pragma once
#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <mesh.h>
#include <map>

class Icosphere
{
private:
	std::vector<Vertex> _verts;
	std::vector<GLuint> _indices;
	GLuint _curIndex;
	std::map<GLint64, GLint> _midPointIndexCache;

public:
	Icosphere(GLuint subdivides);

	std::vector<Vertex> getVerts();
	std::vector<GLuint> getIndices();

protected:
	void create(GLuint subdivides);

	GLint getMid(GLint point1, GLint point2); //hand over ref to the 2 indices

	GLint addVert(Vertex &vert);

};

#endif