#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shaders.h"
#include "Texture.h"
#include "GameObject.h"

// struct for a single particle
struct Particle 
{
	glm::vec2 Position, Rotation;
	glm::vec4 Colour;
	GLfloat Life;

	Particle() : Position(0.0f), Rotation(0.0f), Colour(1.0f), Life(200000.0f) { }
};

// Renders, spawns, updates and kills particles
class ParticleGenerator
{
public:
	// Constructor
	ParticleGenerator(Shader* shader, GLuint texture, GLuint amount);

	~ParticleGenerator();
	// Update particles
	void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// Render particles
	void Draw(glm::mat4 proj);

private:
	// Setup particles
	std::vector<Particle> particles;
	GLuint amount;
	// Render state
	Shader shader;
	GLuint texture;
	GLuint VAO;
	// Initializes buffer and vertex attributes
	void init();
	// Returns the first Particle index that's currently unused
	GLuint firstUnusedParticle();
	// Respawns particle
	void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

