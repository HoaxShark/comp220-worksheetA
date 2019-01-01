#pragma once

#include <string>
#include <vector>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>

#include "vertex.h"
#include "Mesh.h"

bool LoadModelFromFile(const std::string& filename, GLuint VBO, GLuint EBO, unsigned int& numberOfVerts, unsigned int& numberOfIndices);

bool LoadMeshesFromFile(const std::string& filename, MeshCollection * pMeshCollection);