#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "LightValues.h"
#include "PlayerController.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void CreateObject(std::string objectType, const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, 
					  glm::vec3 scale, glm::vec3 rotationAxis, float speed, float scaleFactor, Shader * shader);
	void DrawObjects(std::vector<GameObject*> list, glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
	void LoadAllObjects(Shader* objectShader, Shader* lightShader);
	void UpdateObjectList(std::vector<GameObject*> list, PlayerController player, bool light, float deltaTime);
	void LoadAsteroids(Shader* objectShader);

	std::vector<GameObject*>& GetGameObjectList()
	{
		return gameObjectList;
	}
	std::vector<GameObject*>& GetLightObjectList()
	{
		return lightObjectList;
	}
	std::vector<GameObject*>& GetParticleObjectList()
	{
		return particleObjectList;
	}
	glm::vec3& GetLightObjectPos()
	{
		return lightObjectPos;
	}
	glm::vec3& GetCurrentLightPos()
	{
		return currentLightPos;
	}

private:
	GLuint textureID;
	std::vector<GameObject*> gameObjectList;
	std::vector<GameObject*> lightObjectList;
	std::vector<GameObject*> particleObjectList;
	LightValues lightValues;

	int numberOfAsteroids = 80;
	glm::vec3 asteroidBeltStart = glm::vec3(400.0f, 400.0f, 300.0f);

	// lightObject position used to tell where the center of the light is
	glm::vec3 lightObjectPos;
	glm::vec3 currentLightPos;

	// a random normal for rotation and new position
	glm::vec3 randomNormal;

	void RandomNormal();
};

