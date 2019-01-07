#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(GLuint amountOfParticles, ObjectManager* manager, Shader* shader)
{
	amount = amountOfParticles;
	objectManager = manager;
	lightShader = shader;
	Init();
}

ParticleGenerator::~ParticleGenerator()
{
}

// Gets unused particles and respawns new ones, then decreases particle life by delta time and updates position
void ParticleGenerator::Update(GLfloat deltaTime, GameObject & object, GLuint newParticles)
{
	// Add new particles
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(objectManager->GetParticleObjectList()[unusedParticle], object);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		GameObject * p = objectManager->GetParticleObjectList()[i];
		p->DecreaseLife(deltaTime); // reduce life
		if (p->GetLife() > 0.0f)
		{	// particle is alive, thus update
			glm::vec3 currentPos = p->GetPosition();
			glm::vec3 objectsDirection = p->GetRandomNormal();
			currentPos = currentPos + (objectsDirection / speed); // increase the division to slow down the movement
			p->SetPositionVec3(currentPos);
		}
		p->Update(deltaTime);
	}
}

void ParticleGenerator::Init()
{
	// Create the default amount of particle instances
	for (GLuint i = 0; i < amount; ++i)
	{
		objectManager->CreateObject("particle", "Model/star.obj", "Model/light3.png", 15.0f, 15.0f, -20.0f, glm::vec3(0.0085f, 0.0085f, 0.0085f), glm::vec3(1.0f, 1.0f, 0.0f), 0.5f, 0.0f, lightShader);
	}
}

// Stores the index of the last particle
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::FirstUnusedParticle()
{
	// Search from last used particle
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (objectManager->GetParticleObjectList()[i]->GetLife() <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise serach from the begining
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (objectManager->GetParticleObjectList()[i]->GetLife() <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// If there are no dead particles use the first one in the list
	lastUsedParticle = 0;
	return 0;
}

// Respawn the particle and give it a new random direction
void ParticleGenerator::RespawnParticle(GameObject * particle, GameObject & object)
{
	particle->SetPositionVec3(object.GetPosition());
	particle->ResetLife();
	particle->RandomNormal();
}
