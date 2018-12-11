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
	ParticleGenerator(GLuint Amount, ObjectManager* manager, Shader* shader);

	~ParticleGenerator();
	// Update particles
	void Update(GLfloat dt, GameObject &object, GLuint newParticles);

private:
	// Setup particles
	std::vector<GameObject*> particles;
	GLuint amount;
	void init();
	ObjectManager * objectManager;
	Shader* lightShader;
	std::vector<GameObject*> ParticleObjectList;
	// Returns the first Particle index that's currently unused
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(GameObject * particle, GameObject & object);
};

