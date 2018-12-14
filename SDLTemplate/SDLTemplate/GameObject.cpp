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
		// if at max size, flip scale factor so it gets smaller
		if (Scale.x >= 0.0085f)
		{
			scaleFactor = -scaleFactor;
		}
		// if at min size, flip scale factor so it gets bigger
		else if (Scale.x <= 0.0015f)
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

	//combine the above matrices into the model matrix
	ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	// rotate the ModelMatrix
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotateAmount), rotationAxis);
	// increase rotation by rotationSpeed
	rotateAmount += rotationSpeed;
}

void GameObject::ResetLife()
{
	life = (rand() % (lifeMax + 1 - lifeMin)) + lifeMin;
}

void GameObject::Render()
{
	if (Meshes)
	{
		Meshes->render();
	}
}

// Creates a random normal for the game object
void GameObject::RandomNormal()
{
	const int maximum_number = 1.0f;
	const int minimum_number = -1.0f;
	double x = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number; 
	double y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
	double z = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
	randomNormal.x = x;
	randomNormal.y = y;
	randomNormal.z = z;
}
