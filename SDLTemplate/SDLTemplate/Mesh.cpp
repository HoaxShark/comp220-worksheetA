#include "Mesh.h"

Mesh::Mesh()
{
	m_VBO = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_NumberOfIndices = 0;
	m_NumberOfVertices = 0;
}


Mesh::~Mesh()
{
	Destroy();
}

void Mesh::CopyBufferData(Vertex * pVerts, unsigned int numberOfVerts, unsigned int * pIndices, unsigned int numberOfIndices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfVerts * sizeof(Vertex), pVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);

	m_NumberOfIndices = numberOfIndices;
	m_NumberOfVertices = numberOfVerts;
	glBindVertexArray(m_VAO);
	// enable vertex attribute arrays
	// vertext locations
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	// normals
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12 * sizeof(float)));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(15 * sizeof(float)));
}

void Mesh::Init()
{
	// set vertex array memory location
	glGenVertexArrays(1, &m_VAO);
	// bind vertex arrays
	glBindVertexArray(m_VAO);

	// set vertex buffer memory location
	glGenBuffers(1, &m_VBO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// set element buffer memory location
	glGenBuffers(1, &m_EBO);
	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void Mesh::Destroy()
{
	//Delete Buffer
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	//Delete Vertex Array
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Render()
{
	// bind vertex arrays
	glBindVertexArray(m_VAO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glDrawElements(GL_TRIANGLES, m_NumberOfIndices, GL_UNSIGNED_INT, (void*)0);
}

MeshCollection::MeshCollection()
{
}

MeshCollection::~MeshCollection()
{
	Destroy();
}

// add new mesh to the collection
void MeshCollection::AddMesh(Mesh * pMesh)
{
	m_Meshes.push_back(pMesh);
}

// loop through all meshes in the collection and call render on them
void MeshCollection::Render()
{
	for (Mesh *pMesh : m_Meshes)
	{
		pMesh->Render();
	}
}

void MeshCollection::Destroy()
{
	// Delete the data in meshes - can't use the standard for loop as you can't modifiy anything instide the loop
	auto iter = m_Meshes.begin();
	// while iter isn't the end of meshes
	while (iter != m_Meshes.end())
	{
		// check iter has memory
		if ((*iter))
		{
			// destroy the mesh
			(*iter)->Destroy();
			// delete the memory
			delete (*iter);
			(*iter) = nullptr;
			// erase the slot in the vector and return new iter
			iter = m_Meshes.erase(iter);
		}
		else
		{
			// if no memory move on
			iter++;
		}
	}
	// final flush of vector
	m_Meshes.clear();
}
