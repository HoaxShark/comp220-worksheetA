#pragma once
#include "stdafx.h"
#include "Window.h"

class Initialise
{
public:

	SDL_Window* initaliseSDLWindow();
	SDL_Renderer* initaliseSDLRenderer();
	int initaliseSDL();

	Initialise();
	~Initialise();

private:
	Window window;
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* renderer = NULL;
	SDL_GLContext gl_Context = nullptr;
};

