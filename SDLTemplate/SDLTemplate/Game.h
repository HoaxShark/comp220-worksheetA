#pragma once
#include "stdafx.h"
#include "SDL.h"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include "Model.h"

#include <time.h>
#include "Window.h"
#include "Initialise.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <time.h>
#include "Vertex.h"
#include "Texture.h"
#include "PlayerController.h"
#include "Shaders.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "Skybox.h"
#include "ObjectManager.h"

using namespace glm;

class Game
{
public:

	Game();
	~Game();
	void gameLoop();
	

private:
	SDL_Renderer* renderer = NULL;
	SDL_Window* mainWindow = nullptr;
	SDL_GLContext gl_Context = nullptr;
	bool gameRunning = true;

	Window window;
	Initialise init;
	PlayerController player;
	Skybox * skybox;
	Shader * texturedShader;
	Shader * lightOrbShader;
	Shader * skyboxShader;

	//ParticleGenerator *Particles;

	ObjectManager objectManager;

	// used for the perspective camera
	mat4 proj;
	mat4 view;
	GLuint programID;

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// create elemenet buffer variable
	GLuint elementbuffer;
	// Create Vertex Array
	GLuint VertexArrayID;
	GLuint modelMatrixLocation;
	GLuint viewLocation;
	GLuint projLocation;
	GLuint textureID;
	//GLuint particleTextureID;

	// camera
	glm::vec3 cameraPosition = player.camera.getCameraPos();

	// variables for delta time calulations
	float lastTime;
	float tickTime;
	float deltaTime;

	// SDL window size
	int fullWidth;
	int fullHeight;

	void initialiseGame();
	void gameQuit();
};

