#pragma once

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <SDL_image.h>
#include <glm\gtc\type_ptr.hpp>
#include <vector>

#include "Shaders.h"

class Skybox
{
public:
	Skybox();
	~Skybox();
	void renderSkybox(glm::mat4 playerViewMatrix, glm::mat4 proj);
	void initSkybox();
	unsigned int getCubemapTexture()
	{
		return cubemapTexture;
	}
	GLuint getSkyboxVAO()
	{
		return skyboxVAO;
	}
	Shader* getShader()
	{
		return skyboxShader;
	}

private: 
	unsigned int cubemapTexture;
	GLuint skyboxVAO, skyboxVBO;
	Shader * skyboxShader;
	glm::mat4 view;
	unsigned int loadCubemap(std::vector<std::string> faces);
	
};

