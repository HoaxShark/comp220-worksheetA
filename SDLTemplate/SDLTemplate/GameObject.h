#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shaders.h"

#define GLM_ENABLE_EXPERIMENTAL

class GameObject
{
public:
	GameObject();
	~GameObject();

	glm::vec3 GetThrownDirection()
	{
		return thrownDirection;
	}
	glm::vec3 GetThrownNormal()
	{
		return thrownNormal;
	}
	glm::vec3 GetRandomNormal()
	{
		return randomNormal;
	}
	glm::vec3 GetScale()
	{
		return scale;
	}
	glm::vec3& GetPosition()
	{
		return position;
	}
	glm::mat4& GetModelTransformation()
	{
		return modelMatrix;
	}
	Shader* GetShader()
	{
		return shaderProgram;
	}
	GLuint GetDiffuseTexture()
	{
		return diffuseTexture;
	}
	bool GetWithPlayer()
	{
		return withPlayer;
	}
	float GetLife()
	{
		return life;
	}

	void SetThrownDirection(glm::vec3 direction)
	{
		thrownDirection = direction;
	}
	void SetThrownNormal(glm::vec3 normal)
	{
		thrownNormal = normal;
	}
	void SetScale(glm::vec3 newScale)
	{
		scale = newScale;
	}
	void SetPosition(float x, float y, float z)
	{
		position = glm::vec3(x, y, z);
	}
	void SetPositionVec3(glm::vec3 newPos)
	{
		position = newPos;
	}
	void SetShader(Shader * shader)
	{
		shaderProgram = shader;
	}
	void SetdiffuseTexture(GLuint texture)
	{
		diffuseTexture = texture;
	}
	void SetWithPlayer(bool isWithPlayer)
	{
		withPlayer = isWithPlayer;
	}
	void SetRotationAxis(glm::vec3 axis)
	{
		rotationAxis = axis;
	}
	void SetRotationSpeed(float speed)
	{
		rotationSpeed = speed;
	}
	void SetScaleFactor(float factor)
	{
		scaleFactor = factor;
	}
	void SetIsParticle(bool particle)
	{
		isParticle = particle;
	}
	void SetMesh(MeshCollection * newMeshes)
	{
		meshes = newMeshes;
	}

	void DecreaseLife(float deltaTime)
	{
		life -= deltaTime;
	}
	void Update(float deltaTime);
	void ResetLife();
	void Render();
	void RandomNormal();

private:
	//Set up positions for position, rotation and scale
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// a random normal for particle direction
	glm::vec3 randomNormal;

	// variables for partivle usage
	bool isParticle = false;
	const int lifeMax = 1000;
	const int lifeMin = 800;
	float life = 1000.0f;

	// variables for throwing the light gameObject
	bool withPlayer = false;
	glm::vec3 thrownDirection;
	glm::vec3 thrownNormal;

	//calculate the translation, rotation and scale matrices using the above vectores
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	//combine the above matrices into the model matrix
	glm::mat4 modelMatrix;

	//Mesh
	MeshCollection * meshes;

	//Shader
	Shader * shaderProgram;

	//Texture
	GLuint diffuseTexture;

	// Variables for rotating and scaling 
	float rotateAmount;
	float rotationSpeed = 0.5f;
	float scaleFactor = 0.0f;
	glm::vec3 rotationAxis; // angle of the axis an object will rotate around
};