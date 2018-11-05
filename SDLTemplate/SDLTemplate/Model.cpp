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
		}
		// loop through the faces of the current mesh
		for (unsigned int f = 0; f < currentAIMesh->mNumFaces; f++)
		{
			const aiFace currentFace = currentAIMesh->mFaces[f];
		}
	}

	return true;
}

/* TODO carry on https://www.youtube.com/watch?v=4Q3KDZMTaTw&feature=youtu.be from 24:30*/