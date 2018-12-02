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
	Camera camera;
	PlayerController player;
	std::vector<GameObject*> GameObjectList;
	std::vector<GameObject*> LightObjectList;
	Skybox * skybox;
	Shader * texturedShader;
	Shader * lightOrbShader;
	Shader * skyboxShader;
	//Shader * particleShader;
	//ParticleGenerator *Particles;

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
	GLuint particleTextureID;

	// Materials for lighting
	glm::vec4 ambientMaterialColour = glm::vec4(0.0f, 0.0f, 0.01f, 1.0f);
	glm::vec4 diffuseMaterialColour = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 specularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularMaterialPower = 50.0f;
	// camera
	glm::vec3 cameraPosition = player.camera.getCameraPos();
	// Light
	glm::vec4 ambientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 diffuseLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	float rotateAngle = 0.0f;
	float lastTime;
	float tickTime;
	float deltaTime;
	int fullWidth;
	int fullHeight;

	void initialiseGame();
	/* Creates a new game object and stores it in the objectList
	takes file and texture locations, x,y,z positions, vec3 for scale, and vec3 for the axis to rotate around, and a rotation speed, set speed to 0 for no rotation*/
	void createObject(const std::string& fileLocation, const std::string& textureLocation, float posX, float posY, float posZ, glm::vec3 scale, 
						glm::vec3 rotationAxis, float speed);
	/* Creates a new game object and stores it in the objectList
takes file and texture locations, x,y,z positions, vec3 for scale, and vec3 for the axis to rotate around, and a rotation speed, set speed to 0 for no rotation*/
	void createLightObject(const std::string& fileLocation, const std::string& textureLocation, float posX, float posY, float posZ, 
							glm::vec3 scale, glm::vec3 rotationAxis, float speed, float scaleFactor);
	void drawObjects(std::vector<GameObject*> list);
	void gameQuit();
};

