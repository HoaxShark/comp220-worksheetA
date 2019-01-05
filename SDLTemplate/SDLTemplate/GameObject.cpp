#include "GameObject.h"

GameObject::GameObject()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);

	translationMatrix = glm::mat4(0.0f);
	rotationMatrix = glm::mat4(0.0f);
	scaleMatrix = glm::mat4(0.0f);
	modelMatrix = glm::mat4(0.0f);

	meshes = nullptr;
	shaderProgram = nullptr;
	diffuseTexture = 0;

	RandomNormal();
}

GameObject::~GameObject()
{
	glDeleteTextures(1, &diffuseTexture);

	if (meshes) {
		delete meshes;
	}
	if (shaderProgram)
	{
		delete shaderProgram;
	}
}

void GameObject::Update(float deltaTime)
{
	if (isParticle)
	{
		// if at max size, flip scale factor so it gets smaller
		if (scale.x >= 0.0085f)
		{
			scaleFactor = -scaleFactor;
		}
		// if at min size, flip scale factor so it gets bigger
		else if (scale.x <= 0.0015f)
		{
			scaleFactor = -scaleFactor;
		}

		scale += scaleFactor;
	}
	translationMatrix = glm::translate(position);
	rotationMatrix = glm::rotate(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))
					*glm::rotate(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))
					*glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(scale);

	//combine the above matrices into the model matrix
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	// rotate the ModelMatrix
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAmount), rotationAxis);
	// increase rotation by rotationSpeed
	rotateAmount += rotationSpeed;
}

void GameObject::ResetLife()
{
	life = (rand() % (lifeMax + 1 - lifeMin)) + lifeMin;
}

void GameObject::Render()
{
	if (meshes)
	{
		meshes->Render();
	}
}

// Creates a random normal for the game object
void GameObject::RandomNormal()
{
	const int maximum_number = 1;
	const int minimum_number = -1;
	float x = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number; 
	float y = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
	float z = (rand() % (maximum_number + 1 - minimum_number)) + minimum_number;
	randomNormal.x = x;
	randomNormal.y = y;
	randomNormal.z = z;
}
