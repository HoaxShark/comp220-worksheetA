#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(GLuint Amount, ObjectManager* manager, Shader* shader)
{
	amount = Amount;
	objectManager = manager;
	lightShader = shader;
	init();
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::Update(GLfloat deltaTime, GameObject & object, GLuint newParticles)
{
	//ParticleObjectList = objectManager.GetParticleObjectList();
	// Add new particles
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(objectManager->GetParticleObjectList()[unusedParticle], object);
	}
	// Update all particles
	for (GLuint i = 0; i < this->amount; ++i)
	{
		GameObject * p = objectManager->GetParticleObjectList()[i];
		p->DecreaseLife(deltaTime); // reduce life
		if (p->GetLife() > 0.0f)
		{	// particle is alive, thus update
			vec3 currentPos = p->GetPosition();
			vec3 objectsDirection = p->GetRandomNormal();
			currentPos = currentPos + objectsDirection;
			p->SetPositionVec3(currentPos);
		}
	}
}

void ParticleGenerator::init()
{
	// Create this->amount default particle instances
	for (GLuint i = 0; i < this->amount; ++i)
	{
		objectManager->createParticleObject("Model/star.obj", "Model/light3.png", 15.0f, 15.0f, -20.0f, vec3(0.085f, 0.085f, 0.085f), vec3(1.0f, 1.0f, 0.0f), 0.5f, 0.0f, lightShader);
	}
}

// Stores the index of the last particle used to save on computing time
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (objectManager->GetParticleObjectList()[i]->GetLife() <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (objectManager->GetParticleObjectList()[i]->GetLife() <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(GameObject * particle, GameObject & object)
{
	particle->SetPositionVec3(object.GetPosition());
	particle->ResetLife();
	particle->RandomNormal();
}
