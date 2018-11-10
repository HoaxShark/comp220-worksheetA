#include "PlayerController.h"



PlayerController::PlayerController()
{
}


PlayerController::~PlayerController()
{
}

void PlayerController::handleKeyboard(float deltaTime)
{
	cameraPosition = camera.getCameraPos();
	cameraTarget = camera.getCameraFront();
	moveSpeed = camera.getBaseCameraSpeed() * deltaTime;

	//Check if inputs are pressed.
	if (isPressed(SDLK_w)) // move forward
	{
		camera.increaseCameraPos(moveSpeed*camera.getCameraFront());
	}

	if (isPressed(SDLK_a)) // move left
	{
		camera.increaseCameraPos(-glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * moveSpeed);
	}

	if (isPressed(SDLK_s)) // move backwards
	{
		camera.increaseCameraPos(-moveSpeed*camera.getCameraFront());
	}

	if (isPressed(SDLK_d)) // move right
	{
		camera.increaseCameraPos(glm::normalize(glm::cross(camera.getCameraFront(), camera.getCameraUp())) * moveSpeed);
	}

	if (isPressed(SDLK_SPACE)) // move up
	{
		camera.increaseCameraPos(glm::vec3(0, 1, 0)*moveSpeed);
	}

	if (isPressed(SDLK_LSHIFT)) // move down
	{
		camera.increaseCameraPos(glm::vec3(0, -1, 0)*moveSpeed);
	}
}

void PlayerController::manageKeyboardEvents(SDL_Event event)
{
	keyStates[event.key.keysym.sym] = event.key.state;
}

bool PlayerController::isReleased(SDL_Keycode key)
{
	return keyStates[key] == SDL_RELEASED;
}

bool PlayerController::isPressed(SDL_Keycode key)
{
	return keyStates[key] == SDL_PRESSED;
}

void PlayerController::clearEvents()
{
	keyStates.clear();
}

void PlayerController::mouseUpdate(float xPos, float yPos)
{
	// multiply by the sensitivity of the mouse set in game.h
	float sensitivity = 0.05f;
	xPos *= sensitivity;
	yPos *= sensitivity;

	// add the offsets to the pitch and yaw
	yaw += xPos;
	pitch += yPos;

	// constrain pitch to stop camera flipping
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	moveCamera();
}

void PlayerController::moveCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = -sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera.setCameraFront(glm::normalize(front));
}