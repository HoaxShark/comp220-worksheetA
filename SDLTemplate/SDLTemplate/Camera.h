#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	glm::vec3 getCameraPos();
	void setCameraPos(glm::vec3 newCameraPos);
	void increaseCameraPos(glm::vec3 newCameraPos);
	glm::vec3 getCameraFront();
	void setCameraFront(glm::vec3 newCameraFront);
	glm::vec3 getCameraUp();
	float getBaseCameraSpeed();
	glm::mat4 getViewMatrix()
	{
		return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
	}
	
private:

	// set camera target
	glm::vec3 cameraTarget;
	// calculate camera direction
	glm::vec3 cameraDirection;
	// set the up baseline
	glm::vec3 up;
	// get the cross product of up and camera direction to be used as right
	glm::vec3 cameraRight;
	// init camera variables
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float baseCameraSpeed = 0.1f;

};

