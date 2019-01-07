#pragma once

#include <glm/glm.hpp>

class LightValues
{
public:
	// Materials for lighting
	glm::vec4 ambientMaterialColour = glm::vec4(0.0f, 0.0f, 0.01f, 1.0f);
	glm::vec4 diffuseMaterialColour = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	glm::vec4 specularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float specularMaterialPower = 50.0f;

	// Light colours and direction
	glm::vec4 ambientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 diffuseLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
};
