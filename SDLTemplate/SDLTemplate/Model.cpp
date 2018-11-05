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

	return true;
}

/* TODO carry on https://www.youtube.com/watch?v=4Q3KDZMTaTw&feature=youtu.be from 30:30*/