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

	glm::vec3& GetCameraPos()
	{
		return cameraPos;
	}
	glm::vec3& GetCameraUp()
	{
		return cameraUp;
	}
	glm::vec3& GetCameraDirection()
	{
		return cameraDirection;
	}
	glm::vec3& GetCameraFront()
	{
		return cameraFront;
	}
	glm::mat4& GetViewMatrix()
	{
		lookAt = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
		return lookAt;
	}
	float& GetBaseCameraSpeed()
	{
		return baseCameraSpeed;
	}

	void SetCameraPos(glm::vec3 newCameraPos)
	{
		cameraPos = newCameraPos;
	}
	void SetCameraFront(glm::vec3 newCameraFront)
	{
		cameraFront = newCameraFront;
	}
	void IncreaseCameraPos(glm::vec3 newCameraPos)
	{
		cameraPos += newCameraPos;
	}

private:

	glm::mat4 lookAt;
	glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;
	// up baseline for camera calculations
	glm::vec3 up;
	glm::vec3 cameraRight;

	// init camera variables
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float baseCameraSpeed = 0.1f;
};

