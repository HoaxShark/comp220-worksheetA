#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shaders.h"
#include "Texture.h"
#include "GameObject.h"
#include "ObjectManager.h"

// Renders, spawns, updates and kills particles
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(GLuint amountOfParticles, ObjectManager* manager, Shader* shader);
	~ParticleGenerator();

	// Update particles
	void Update(GLfloat dt, GameObject &object, GLuint newParticles);

private:
	// Will be the default amount of particles
	GLuint amount;
	// Speed at which the particle will move, higher is slower
	float speed = 100.0f;
	// For pointing to the active objectManager and lightShader
	ObjectManager * objectManager;
	Shader* lightShader;
	// List of all particle objects
	std::vector<GameObject*> particleObjectList;

	// Returns the first Particle index that's currently unused
	GLuint FirstUnusedParticle();
	// Respawns particle
	void RespawnParticle(GameObject * particle, GameObject & object);
	void Init();
};

