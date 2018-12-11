#include "GameObject.h"

GameObject::GameObject()
{
	Position = glm::vec3(0.0f);
	Scale = glm::vec3(1.0f);
	Rotation = glm::vec3(0.0f);

	TranslationMatrix = glm::mat4(0.0f);
	RotationMatrix = glm::mat4(0.0f);
	ScaleMatrix = glm::mat4(0.0f);
	ModelMatrix = glm::mat4(0.0f);

	Meshes = nullptr;
	ShaderProgram = nullptr;
	DiffuseTexture = 0;

	RandomNormal();
}

GameObject::~GameObject()
{
	glDeleteTextures(1, &DiffuseTexture);

	if (Meshes) {
		delete Meshes;
	}
	if (ShaderProgram)
	{
		delete ShaderProgram;
	}
}

void GameObject::Update(float deltaTime)
{
	
	if (isParticle)
	{
		/*
		std::cout << deltaTime << std::endl;
		glm::vec3 directionVec = randomNormal * normalIncrease;
		Position = Position * directionVec;
		normalIncrease += 0.01f;
		life = -0.1f;*/
		// if at max size, flip scale factor so it gets smaller
		if (Scale.x >= 0.8f)
		{
			scaleFactor = -scaleFactor;
		}
		// if at min size, flip scale factor so it gets bigger
		else if (Scale.x <= 0.005f)
		{
			scaleFactor = -scaleFactor;
		}

		Scale += scaleFactor;
	}
	TranslationMatrix = glm::translate(Position);
	RotationMatrix = glm::rotate(Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
		*glm::rotate(Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
		*glm::rotate(Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMatrix = glm::scale(Scale);

	//combine the above matrices into the model matrix (order is important!!!! - TRS)
	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	// rotate the ModelMatrix
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotateAmount), rotationAxis);
	// increase rotation by rotationSpeed
	rotateAmount += rotationSpeed;
}

void GameObject::ResetLife()
{
	life = lifeDefault;
}

void GameObject::Render()
{
	if (Meshes)
	{
		Meshes->render();
	}
}

void GameObject::RandomNormal()
{
	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	randomNormal.x = x;
	randomNormal.y = y;
	randomNormal.z = z;
}
