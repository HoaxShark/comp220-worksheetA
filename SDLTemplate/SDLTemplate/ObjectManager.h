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

using namespace glm;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void createObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed, Shader * shader);

	void createLightObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed, float scaleFactor, Shader * shader);

	void drawObjects(std::vector<GameObject*> list, mat4 view, mat4 projection, vec3 cameraPosition, vec3 lightPosition);

	void loadAllObjects(Shader* objectShader, Shader* lightShader);

	std::vector<GameObject*> GetGameObjectList()
	{
		return GameObjectList;
	}

	std::vector<GameObject*> GetLightObjectList()
	{
		return LightObjectList;
	}


private:
	GLuint textureID;
	std::vector<GameObject*> GameObjectList;
	std::vector<GameObject*> LightObjectList;
	LightValues lightValues;
};

