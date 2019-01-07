#pragma once

#include <map>
#include <iostream>

#include <SDL.h>

#include "Camera.h"
#include "Window.h"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	Camera camera;

	// checks pressed keys and changes camera position
	void HandleKeyboard(float deltaTime);

	//Put keyboard events into map
	void ManageKeyboardEvents(SDL_Event event); 

	//Check if key is released
	bool IsReleased(SDL_Keycode key); 

	//Check if key is pressed
	bool IsPressed(SDL_Keycode key); 

	//Clear map of events
	void ClearEvents(); 

	// updates mouse movement
	void MouseUpdate(float xPos, float yPos);


private:
	// window used to get the height and width of base window
	Window window;

	// camera used to control the camera view
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	
	//Map containing the key, and its state (pressed/released)
	std::map<SDL_Keycode, Uint8> keyStates;

	// mouse movement variables
	float lastX = window.screenWidth / 2.0f;
	float lastY = window.screenHeight / 2.0f;
	float sensitivity = 0.05f;
	float pitch = 0.0f;
	float yaw = 270.0f;
	bool firstMouse = true;

	// player movement speed
	float moveSpeed;

	// moves the camera during mouse movement
	void MoveCamera();	
};

