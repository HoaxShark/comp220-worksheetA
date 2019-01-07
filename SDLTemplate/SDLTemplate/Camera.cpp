#include "Camera.h"

Camera::Camera()
{
	// set camera position
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	// set camera target
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// calculate camera direction
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	// set the up baseline
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	// get the cross product of up and camera direction to be used as right
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	// get the cross product of up and right to get the up direction
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
}

Camera::~Camera()
{
}