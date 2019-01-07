#include "PlayerController.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::HandleKeyboard(float deltaTime)
{
	cameraPosition = camera.GetCameraPos();
	cameraTarget = camera.GetCameraFront();
	moveSpeed = camera.GetBaseCameraSpeed() * deltaTime;

	//Check if inputs are pressed.
	if (IsPressed(SDLK_w)) // move forward
	{
		camera.IncreaseCameraPos(moveSpeed*camera.GetCameraFront());
	}

	if (IsPressed(SDLK_a)) // move left
	{
		camera.IncreaseCameraPos(-glm::normalize(glm::cross(camera.GetCameraFront(), camera.GetCameraUp())) * moveSpeed);
	}

	if (IsPressed(SDLK_s)) // move backwards
	{
		camera.IncreaseCameraPos(-moveSpeed*camera.GetCameraFront());
	}

	if (IsPressed(SDLK_d)) // move right
	{
		camera.IncreaseCameraPos(glm::normalize(glm::cross(camera.GetCameraFront(), camera.GetCameraUp())) * moveSpeed);
	}

	if (IsPressed(SDLK_SPACE)) // move up
	{
		camera.IncreaseCameraPos(glm::vec3(0, 1, 0)*moveSpeed);
	}

	if (IsPressed(SDLK_LSHIFT)) // move down
	{
		camera.IncreaseCameraPos(glm::vec3(0, -1, 0)*moveSpeed);
	}

	if (IsPressed(SDLK_q)) // slow down
	{
		camera.SetBaseCameraSpeed(camera.GetBaseCameraSpeed() - 0.01f);
	}

	if (IsPressed(SDLK_e)) // speed up
	{
		camera.SetBaseCameraSpeed(camera.GetBaseCameraSpeed() + 0.01f);
	}
}

void PlayerController::ManageKeyboardEvents(SDL_Event event)
{
	keyStates[event.key.keysym.sym] = event.key.state;
}

bool PlayerController::IsReleased(SDL_Keycode key)
{
	return keyStates[key] == SDL_RELEASED;
}

bool PlayerController::IsPressed(SDL_Keycode key)
{
	return keyStates[key] == SDL_PRESSED;
}

void PlayerController::ClearEvents()
{
	keyStates.clear();
}

void PlayerController::MouseUpdate(float xPos, float yPos)
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

	MoveCamera();
}

void PlayerController::MoveCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = -sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera.SetCameraFront(glm::normalize(front));
}