#pragma once

#include <vector>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CopyBufferData(Vertex *pVerts, unsigned int numberOfVerts, unsigned int *pIndices, unsigned int numberOfIndices);
	void Init();
	void Destroy();
	void Render();

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

	void AddMesh(Mesh *pMesh);
	void Render();
	void Destroy();

private:
	std::vector<Mesh*> m_Meshes;
};