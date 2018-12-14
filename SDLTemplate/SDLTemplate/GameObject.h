#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Shaders.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update(float deltaTime);

	void SetPosition(float x, float y, float z)
	{
		Position = glm::vec3(x, y, z);
	};

	void SetPositionVec3(glm::vec3 newPos)
	{
		Position = newPos;
	};

	glm::vec3& GetPosition()
	{
		return Position;
	};

	glm::mat4& GetModelTransformation()
	{
		return ModelMatrix;
	};

	void SetMesh(MeshCollection * meshes)
	{
		Meshes = meshes;
	};

	void SetShader(Shader * shader)
	{
		ShaderProgram = shader;
	};

	Shader * GetShader()
	{
		return ShaderProgram;
	};

	void SetDiffuseTexture(GLuint texture)
	{
		DiffuseTexture = texture;
	};

	GLuint GetDiffuseTexture()
	{
		return DiffuseTexture;
	};

	void SetScale(glm::vec3 scale)
	{
		Scale = scale;
	}

	glm::vec3 getScale()
	{
		return Scale;
	}

	void setRotationAxis(glm::vec3 axis)
	{
		rotationAxis = axis;
	}

	void setRotationSpeed(float speed)
	{
		rotationSpeed = speed;
	}

	void SetScaleFactor(float factor)
	{
		scaleFactor = factor;
	}

	void setIsParticle(bool particle)
	{
		isParticle = particle;
	}

	bool getWithPlayer()
	{
		return withPlayer;
	}

	void setWithPlayer(bool isWithPlayer)
	{
		withPlayer = isWithPlayer;
	}

	void setThrownDirection(glm::vec3 direction)
	{
		thrownDirection = direction;
	}

	glm::vec3 getThrownDirection()
	{
		return thrownDirection;
	}

	void setThrownNormal(glm::vec3 normal)
	{
		thrownNormal = normal;
	}

	glm::vec3 getThrownNormal()
	{
		return thrownNormal;
	}

	float GetLife()
	{
		return life;
	}

	void DecreaseLife(float deltaTime)
	{
		life -= deltaTime;
	}

	glm::vec3 GetRandomNormal()
	{
		return randomNormal;
	}

	void ResetLife();
	void Render();
	void RandomNormal();

private:
	//Set up positions for position, rotation and scale
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

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
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScaleMatrix;
	//combine the above matrices into the model matrix
	glm::mat4 ModelMatrix;

	//Mesh
	MeshCollection * Meshes;

	//Shader
	Shader * ShaderProgram;

	//Texture
	GLuint DiffuseTexture;

	// Variables for rotating and scaling 
	float rotateAmount;
	float rotationSpeed = 0.5f;
	float scaleFactor = 0.0f;
	glm::vec3 rotationAxis; // angle of the axis an object will rotate around
};