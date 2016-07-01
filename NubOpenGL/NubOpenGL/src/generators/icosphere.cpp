#include <generators\icosphere.h>
#include <math.h>

//courtesy of http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

Icosphere::Icosphere(GLuint subdivides)
{
	this->create(subdivides);
}

std::vector<Vertex> Icosphere::getVerts()
{
	return this->_verts;
}

std::vector<GLuint> Icosphere::getIndices()
{
	return this->_indices;
}

void Icosphere::create(GLuint subdivides)
{
	this->_curIndex = 0;
	
	//make 3 quads for the x y z of this initial icosahedron
	Vertex tempVert;

	GLfloat t = (1.0f + sqrt(5.0f) / 2.0f);

	tempVert.pos = glm::vec3(-1, t, 0);
	addVert(tempVert);
	tempVert.pos = glm::vec3(1, t, 0);
	addVert(tempVert);
	tempVert.pos = glm::vec3(-1, -t, 0);
	addVert(tempVert);
	tempVert.pos = glm::vec3(1, -t, 0);
	addVert(tempVert);

	tempVert.pos = glm::vec3(0, -1, t);
	addVert(tempVert);
	tempVert.pos = glm::vec3(0, 1, t);
	addVert(tempVert);
	tempVert.pos = glm::vec3(0, -1, -t);
	addVert(tempVert);
	tempVert.pos = glm::vec3(0, 1, -t);
	addVert(tempVert);

	tempVert.pos = glm::vec3(t, 0, -1);
	addVert(tempVert);
	tempVert.pos = glm::vec3(t, 0, 1);
	addVert(tempVert);
	tempVert.pos = glm::vec3(-t, 0, -1);
	addVert(tempVert);
	tempVert.pos = glm::vec3(-t, 0, 1);
	addVert(tempVert);

	//create 20 indexes that link to those 3 quads
	struct faceIndex {
		GLint ind1, ind2, ind3;
	};
	std::vector<faceIndex> faces;

	faces.push_back(faceIndex{0,11,5});
	faces.push_back(faceIndex{0,5,1});
	faces.push_back(faceIndex{0,1,7});
	faces.push_back(faceIndex{0,7,10});
	faces.push_back(faceIndex{0,10,11});

	faces.push_back(faceIndex{1,5,9});
	faces.push_back(faceIndex{5,11,4});
	faces.push_back(faceIndex{11,10,2});
	faces.push_back(faceIndex{10,7,6});
	faces.push_back(faceIndex{7,1,8});

	faces.push_back(faceIndex{3,9,4});
	faces.push_back(faceIndex{3,4,2});
	faces.push_back(faceIndex{3,2,6});
	faces.push_back(faceIndex{3,6,8});
	faces.push_back(faceIndex{3,8,9});

	faces.push_back(faceIndex{4,9,5});
	faces.push_back(faceIndex{2,4,11});
	faces.push_back(faceIndex{6,2,10});
	faces.push_back(faceIndex{8,6,7});
	faces.push_back(faceIndex{9,8,1});

	for (GLint i = 0; i < subdivides; i++)
	{
		std::vector<faceIndex> faces2;

		for (GLint i = 0; i < faces.size(); i++)
		{
			//replace unrefined tri with 4 inner tri's
			GLint a = getMid(faces[i].ind1, faces[i].ind2);
			GLint b = getMid(faces[i].ind2, faces[i].ind3);
			GLint c = getMid(faces[i].ind3, faces[i].ind1);
			//"at these 2 indexes, what pos do they correspond to? ok, so whats their midpoint?"

			faces2.push_back(faceIndex{ faces[i].ind1, a, c });
			faces2.push_back(faceIndex{ faces[i].ind2, b, a });
			faces2.push_back(faceIndex{ faces[i].ind3, c, b });
			faces2.push_back(faceIndex{ a, b, c });
		}
		faces = faces2;
	}

	//"we're done subdividing"
	for (GLint i = 0; i < faces.size(); i++)
	{
		this->_indices.push_back(faces[i].ind1);
		this->_indices.push_back(faces[i].ind2);
		this->_indices.push_back(faces[i].ind3);
	}
}

GLint Icosphere::getMid(GLint point1, GLint point2)
{
	//see if we have it already
	bool firstSmaller = point1 < point2;
	GLint64 smallerIndex = (firstSmaller) ? point1 : point2;
	GLint64 largerIndex = (firstSmaller) ? point2 : point1;
	GLint64 key = (smallerIndex << 32) + largerIndex; //shift those bits away

	GLint ret;
	if (this->_midPointIndexCache.count(key)) //if the count of the instance of key is above 1, it returns yes and execs this if statement
	{
		ret = this->_midPointIndexCache.at(key); //just give back the cache'd one, why repeat
		return ret;
	}

	Vertex coord1 = this->_verts[point1];
	Vertex coord2 = this->_verts[point2];

	Vertex coordMid = {
		glm::vec3(
			(coord1.pos.x + coord2.pos.x) / 2.0f,
			(coord1.pos.y + coord2.pos.y) / 2.0f,
			(coord1.pos.z + coord2.pos.z) / 2.0f
			)
	};

	GLint i = addVert(coordMid);

	//cache this^
	this->_midPointIndexCache[key] = i;
	return i;
}

GLint Icosphere::addVert(Vertex & vert)
{
	GLdouble length = sqrt(
		vert.pos.x * vert.pos.x
		+ 
		vert.pos.y * vert.pos.y
		+ 
		vert.pos.z * vert.pos.z);

	Vertex tempVert;
	
	tempVert.pos = glm::vec3(vert.pos.x / length, vert.pos.y / length, vert.pos.z / length);
	this->_verts.push_back(tempVert);
	return this->_curIndex++;
}
