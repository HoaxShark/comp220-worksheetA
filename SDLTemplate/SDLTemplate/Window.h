#pragma once

#include <iostream>

#include <SDL.h>

class Window
{
public:
	Window();
	~Window();

	// if the window is currently fullscreen
	bool isFullscreen;

	//Dimensions of SDL window.
	int screenWidth;
	int screenHeight;

	//Window Title
	char* windowTitle;

	// get and set isFullscreen
	bool GetIsFullscreen();
	void SetIsFullscreen();

};

