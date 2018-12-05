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

	void setScale(glm::vec3 scale)
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

	void setScaleFactor(float factor)
	{
		scaleFactor = factor;
	}

	void setIsParticle(bool particle)
	{
		isParticle = particle;
	}

	float getLife()
	{
		return life;
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

	void Render();
	void RandomNormal();

private:
	//Set up positions for position, rotation and scale
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	// a random normal for particle direction
	glm::vec3 randomNormal;
	float normalIncrease = 1.001f;

	bool withPlayer = false;
	bool isParticle = false;
	float life = 20000.0f;
	glm::vec3 thrownDirection;
	glm::vec3 thrownNormal;

	//calculate the translation, rotation and scale matrices using the above vectores
	glm::mat4 TranslationMatrix;
	glm::mat4 RotationMatrix;
	glm::mat4 ScaleMatrix;
	//combine the above matrices into the model matrix (order is important!!!! - TRS)
	glm::mat4 ModelMatrix;

	//Mesh
	MeshCollection * Meshes;

	//Shader
	Shader * ShaderProgram;

	//Texture
	GLuint DiffuseTexture;

	float rotateAmount;
	float rotationSpeed = 0.2f;
	float scaleFactor = 0.0f;
	glm::vec3 rotationAxis; // angle of the axis an object will rotate around
};