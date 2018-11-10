#pragma once

#include <SDL.h>
#include <map>
#include <iostream>

#include "Camera.h"
#include "Window.h"

class PlayerController
{
public:
	PlayerController();
	~PlayerController();

	Camera camera;

	// checks pressed keys and changes camera position
	void handleKeyboard(float deltaTime);

	//Put keyboard events into map
	void manageKeyboardEvents(SDL_Event event); 

	//Check if key is released
	bool isReleased(SDL_Keycode key); 

	//Check if key is pressed
	bool isPressed(SDL_Keycode key); 

	//Clear map of events
	void clearEvents(); 

	// updates mouse movement
	void mouseUpdate(float xPos, float yPos);


private:
	// window used to get the height and width of base window
	Window window;

	// camera used to control the camera view
	
	glm::vec3 cameraPosition;
	glm::vec3 cameraTarget;
	
	//Map containing the key, and its state (pressed/released)
	std::map<SDL_Keycode, Uint8> keyStates;

	// mouse movement variables
	float lastX = window.screenWidth / 2;
	float lastY = window.screenHeight / 2;
	float sensitivity = 0.05f;
	float pitch = 0.0f;
	float yaw = 270.0f;
	bool firstMouse = true;

	// player movement speed
	float moveSpeed;

	// moves the camera during mouse movement
	void moveCamera();	
};

