#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "GameObject.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shaders.h"
#include "LightValues.h"
#include "PlayerController.h"

using namespace glm;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void createObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed, Shader * shader);

	void createLightObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed, float scaleFactor, Shader * shader);

	void drawObjects(std::vector<GameObject*> list, mat4 view, mat4 projection, vec3 cameraPosition);

	void loadAllObjects(Shader* objectShader, Shader* lightShader);

	void updateObjectList(std::vector<GameObject*> list, PlayerController player, bool light, float deltaTime);

	std::vector<GameObject*> GetGameObjectList()
	{
		return GameObjectList;
	}

	std::vector<GameObject*> GetLightObjectList()
	{
		return LightObjectList;
	}

	vec3 GetLightObjectPos()
	{
		return lightObjectPos;
	}

	vec3 GetCurrentLightPos()
	{
		return currentLightPos;
	}

private:
	GLuint textureID;
	std::vector<GameObject*> GameObjectList;
	std::vector<GameObject*> LightObjectList;
	LightValues lightValues;

	// lightObject position used to tell where the center of the light is
	vec3 lightObjectPos;
	vec3 currentLightPos;
};

