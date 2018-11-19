#pragma once
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void copyBufferData(Vertex *pVerts, unsigned int numberOfVerts, unsigned int *pIndices, unsigned int numberOfIndices);

	void init();

	void destroy();

	void render();

private:
	GLuint m_VBO; // vertex buffer object
	GLuint m_EBO; // element buffer object
	GLuint m_VAO; // vertex array buffer

	unsigned int m_NumberOfVertices;
	unsigned int m_NumberOfIndices;
};

class MeshCollection
{
public:
	MeshCollection();
	~MeshCollection();

	void addMesh(Mesh *pMesh);

	void render();
	void destroy();

private:
	std::vector<Mesh*> m_Meshes;
};