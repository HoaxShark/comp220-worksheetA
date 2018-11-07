#include "Model.h"

bool loadModelFromFile(const std::string & filename, GLuint VBO, GLuint EBO, unsigned int & numberOfVerts, unsigned int & numberOfIndices)
{
	// create importer
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

	// if no scene return error message
	if (scene == nullptr)
	{
		printf("Error loading model %s", importer.GetErrorString());
		return false;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// loop through all meshes in scene
	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		// get the scene mesh
		const aiMesh* currentAIMesh = scene->mMeshes[m];
		// loop through the verts in the current mesh
		for (unsigned int v = 0; v < currentAIMesh->mNumVertices; v++)
		{
			const aiVector3D currentAIPosition = currentAIMesh->mVertices[v];

			Vertex ourVertex;
			ourVertex.x = currentAIPosition.x;
			ourVertex.y = currentAIPosition.y;
			ourVertex.z = currentAIPosition.z;
			ourVertex.r = 1.0f; ourVertex.b = 1.0f; ourVertex.g = 1.0f; ourVertex.r = 1.0f;
			ourVertex.tu = 0.0f; ourVertex.tv = 0.0f;

			if (currentAIMesh->HasTextureCoords(0))
			{
				// get current texture coords
				const aiVector3D currentTextureCoords = currentAIMesh->mTextureCoords[0][v];
				ourVertex.tu = currentTextureCoords.x;
				ourVertex.tv = currentTextureCoords.y;
			}
			if (currentAIMesh->HasVertexColors(0))
			{
				const aiColor4D currentColour = currentAIMesh->mColors[0][v];
				ourVertex.r = currentColour.r;
				ourVertex.g = currentColour.g;
				ourVertex.b = currentColour.b;
				ourVertex.a = currentColour.a;
			}

			// push ourVertec into the vertices
			vertices.push_back(ourVertex);

		}
		// loop through the faces of the current mesh
		for (unsigned int f = 0; f < currentAIMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAIMesh->mFaces[f];
			
			// push the 3 indices of the face
			indices.push_back(currentFace.mIndices[0]);
			indices.push_back(currentFace.mIndices[1]);
			indices.push_back(currentFace.mIndices[2]);
		}
	}

	// set the number of verts
	numberOfVerts = vertices.size();
	// set number of indices
	numberOfIndices = indices.size();

	// bind VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy across to the buffer
	glBufferData(GL_ARRAY_BUFFER, numberOfVerts * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// bind EBO buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// copy across to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	return true;
}

bool loadMeshesFromFile(const std::string & filename, std::vector<Mesh*>& meshes)
{
	// create importer
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast | aiProcess_FlipUVs);

	// if no scene return error message
	if (scene == nullptr)
	{
		printf("Error loading model %s", importer.GetErrorString());
		return false;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// loop through all meshes in scene
	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		// get the scene mesh
		const aiMesh* currentAIMesh = scene->mMeshes[m];
		// create Mesh
		Mesh* ourCurrentMesh = new Mesh();
		// Initialise Mesh
		ourCurrentMesh->init();
		// loop through the verts in the current mesh
		for (unsigned int v = 0; v < currentAIMesh->mNumVertices; v++)
		{
			const aiVector3D currentAIPosition = currentAIMesh->mVertices[v];

			Vertex ourVertex;
			ourVertex.x = currentAIPosition.x;
			ourVertex.y = currentAIPosition.y;
			ourVertex.z = currentAIPosition.z;
			ourVertex.r = 1.0f; ourVertex.b = 1.0f; ourVertex.g = 1.0f; ourVertex.r = 1.0f;
			ourVertex.tu = 0.0f; ourVertex.tv = 0.0f;

			if (currentAIMesh->HasTextureCoords(0))
			{
				// get current texture coords
				const aiVector3D currentTextureCoords = currentAIMesh->mTextureCoords[0][v];
				ourVertex.tu = currentTextureCoords.x;
				ourVertex.tv = currentTextureCoords.y;
			}
			if (currentAIMesh->HasVertexColors(0))
			{
				const aiColor4D currentColour = currentAIMesh->mColors[0][v];
				ourVertex.r = currentColour.r;
				ourVertex.g = currentColour.g;
				ourVertex.b = currentColour.b;
				ourVertex.a = currentColour.a;
			}

			// push ourVertec into the vertices
			vertices.push_back(ourVertex);

		}
		// loop through the faces of the current mesh
		for (unsigned int f = 0; f < currentAIMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAIMesh->mFaces[f];

			// push the 3 indices of the face
			indices.push_back(currentFace.mIndices[0]);
			indices.push_back(currentFace.mIndices[1]);
			indices.push_back(currentFace.mIndices[2]);
		}

		// send vertices and indices 
		ourCurrentMesh->copyMeshData(vertices, indices);
		meshes.push_back(ourCurrentMesh);

		// clear verts and indices ready for next mesh
		vertices.clear();
		indices.clear();
	}
	return true;
}
