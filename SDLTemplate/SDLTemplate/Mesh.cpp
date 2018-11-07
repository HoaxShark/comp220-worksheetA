#include "Mesh.h"

Mesh::Mesh()
{
	m_VBO = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_NumberOfIndices = 0;
	m_NumberOfVerts = 0;
}


Mesh::~Mesh()
{
	destroy();
}

void Mesh::copyMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(m_VAO);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),     // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer
	(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(3 * sizeof(float))
	);

	// 3rd attribute buffer : texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer
	(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(7 * sizeof(float))
	);

	m_NumberOfVerts = vertices.size();
	m_NumberOfIndices = indices.size();
}

void Mesh::init()
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

void Mesh::destroy()
{
	//Delete Buffer
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	//Delete Vertex Array
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::render()
{
	// bind vertex arrays
	glBindVertexArray(m_VAO);
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// bind the element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	glDrawElements(GL_TRIANGLES, m_NumberOfIndices, GL_UNSIGNED_INT, 0);
}
