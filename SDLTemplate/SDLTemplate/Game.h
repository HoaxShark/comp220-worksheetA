#pragma once


#include <time.h>
#include <vector>

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Initialise.h"
#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include "Texture.h"
#include "PlayerController.h"
#include "Shaders.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "Skybox.h"
#include "ObjectManager.h"

#define GLM_ENABLE_EXPERIMENTAL

class Game
{
public:

	Game();
	~Game();
	void GameLoop();
	

private:
	SDL_Renderer* renderer = NULL;
	SDL_Window* mainWindow = nullptr;
	SDL_GLContext glContext = nullptr;
	bool gameRunning = true;

	Window window;
	Initialise init;
	PlayerController player;
	Skybox * skybox;
	ParticleGenerator * particleGenerator;
	ObjectManager * objectManager;

	Shader * texturedShader;
	Shader * lightOrbShader;
	Shader * skyboxShader;

	// used for the perspective camera
	mat4 proj;
	mat4 view;

	GLuint programID;
	GLuint textureID;

	// camera
	glm::vec3 cameraPosition = player.camera.GetCameraPos();

	// variables for delta time calulations
	float lastTime;
	float tickTime;
	float deltaTime;

	// SDL window size
	int fullWidth;
	int fullHeight;

	void InitialiseGame();
	void GameQuit();
};

