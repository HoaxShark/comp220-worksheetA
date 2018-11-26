#pragma once
#include "stdafx.h"
#include "SDL.h"
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
//#include "Shader.h"
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

using namespace glm;

class Game
{
public:

	Game();
	~Game();
	void gameLoop();
	void createObject(const std::string& fileLocation, const std::string& textureLocation);

private:
	SDL_Renderer* renderer = NULL;
	SDL_Window* mainWindow = nullptr;
	SDL_GLContext gl_Context = nullptr;
	bool gameRunning = true;

	Window window;
	Initialise init;
	Camera camera;
	PlayerController player;
	std::vector<GameObject*> GameObjectList;
	Shader * texturedShader;

	vec3 position = vec3(0.0f);
	vec3 shapeScale = vec3(1.0f);
	// set up perspective camera
	mat4 proj;
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
	GLuint textureID;

	/*// mouse movement variables
	float lastX = window.screenWidth / 2;
	float lastY = window.screenHeight / 2;
	float sensitivity = 0.05f;
	float pitch = 0.0f;
	float yaw = 270.0f;
	bool firstMouse = true;*/

	float rotateAngle = 0.0f;
	float lastTime;
	float tickTime;
	float deltaTime;
	int fullWidth;
	int fullHeight;

	void initialiseGame();
	void gameQuit();

	/*// updates mouse movement
	void mouseUpdate(float xPos, float yPos);
	void moveCamera();*/
};

