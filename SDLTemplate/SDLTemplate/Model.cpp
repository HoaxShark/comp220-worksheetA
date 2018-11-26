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

bool loadMeshesFromFile(const std::string& filename, MeshCollection * pMeshCollection)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords | aiProcess_CalcTangentSpace);

	if (!scene)
	{
		printf("Model Loading Error - %s\n", importer.GetErrorString());
		return false;
	}

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh *currentMesh = scene->mMeshes[i];
		Mesh *pMesh = new Mesh();
		pMesh->init();

			for (int v = 0; v < currentMesh->mNumVertices; v++)
			{
				aiVector3D currentModelVertex = currentMesh->mVertices[v];
				aiColor4D currentModelColour = aiColor4D(1.0, 1.0, 1.0, 1.0);
				aiVector3D currentTextureCoordinates = aiVector3D(0.0f, 0.0f, 0.0f);
				aiVector3D currentModelNormals = aiVector3D(0.0f, 0.0f, 0.0f);
				aiVector3D currentModelTangents = aiVector3D(0.0f, 0.0f, 0.0f);
				aiVector3D currentModelBitangents = aiVector3D(0.0f, 0.0f, 0.0f);

				if (currentMesh->HasVertexColors(0))
				{
					currentModelColour = currentMesh->mColors[0][v];
				}
				if (currentMesh->HasTextureCoords(0))
				{
					currentTextureCoordinates = currentMesh->mTextureCoords[0][v];
				}
				if (currentMesh->HasNormals())
				{
					currentModelNormals = currentMesh->mNormals[v];
				}
				if (currentMesh->HasTangentsAndBitangents())
				{
					currentModelTangents = currentMesh->mTangents[v];
					currentModelBitangents = currentMesh->mBitangents[v];
				}

				Vertex currentVertex = { currentModelVertex.x,currentModelVertex.y,currentModelVertex.z,
					currentModelColour.r,currentModelColour.g,currentModelColour.b,currentModelColour.a,
					currentTextureCoordinates.x,currentTextureCoordinates.y,
					currentModelNormals.x,currentModelNormals.y,currentModelNormals.z,
					currentModelTangents.x,currentModelTangents.y,currentModelTangents.z,
					currentModelBitangents.x,currentModelBitangents.y,currentModelBitangents.z };

				vertices.push_back(currentVertex);
			}

		for (int f = 0; f < currentMesh->mNumFaces; f++)
		{
			aiFace currentModelFace = currentMesh->mFaces[f];
			indices.push_back(currentModelFace.mIndices[0]);
			indices.push_back(currentModelFace.mIndices[1]);
			indices.push_back(currentModelFace.mIndices[2]);
		}

		pMesh->copyBufferData(vertices.data(), vertices.size(), indices.data(), indices.size());

		pMeshCollection->addMesh(pMesh);
		vertices.clear();
		indices.clear();
	}

	return true;
}