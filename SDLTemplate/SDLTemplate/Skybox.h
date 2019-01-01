#pragma once

#include <vector>

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <SDL_image.h>
#include <glm\gtc\type_ptr.hpp>

#include "Shaders.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int GetCubemapTexture()
	{
		return cubemapTexture;
	}
	GLuint GetSkyboxVAO()
	{
		return skyboxVAO;
	}
	Shader* GetShader()
	{
		return skyboxShader;
	}

	void RenderSkybox(glm::mat4 playerViewMatrix, glm::mat4 proj);
	void InitSkybox();

private: 
	unsigned int cubemapTexture;
	GLuint skyboxVAO, skyboxVBO;
	Shader * skyboxShader;
	glm::mat4 view;

	unsigned int LoadCubemap(std::vector<std::string> faces);
};

