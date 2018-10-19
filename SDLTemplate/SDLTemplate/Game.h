#pragma once
#include "stdafx.h"
#include "SDL.h"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include "Shader.h"

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

using namespace glm;

class Game
{
public:

	SDL_Renderer* renderer = NULL;
	SDL_Window* mainWindow = nullptr;
	SDL_GLContext gl_Context = nullptr;
	bool gameRunning = true;

	Window window;
	Initialise init;

	vec3 position = vec3(0.0f);
	vec3 shapeScale = vec3(1.0f);
	float rotateAngle = 0.0f;
	// set up perspective camera
	mat4 proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.1f, 100.0f);
	mat4 view;
	GLuint programID;
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// create elemenet buffer variable
	GLuint elementbuffer;
	//Create Vertex Array
	GLuint VertexArrayID;
	GLuint modelMatrixLocation;
	GLuint viewLocation;
	GLuint projLocation;

	float lastTime;
	float tickTime;
	float deltaTime;

	int fullWidth;
	int fullHeight;

	void initialiseGame();
	void gameLoop();
	void gameQuit();

	Game();
	~Game();
};

