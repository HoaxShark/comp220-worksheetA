#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <glm\glm.hpp>

#include "Window.h"

class Initialise
{
public:

	SDL_Window* InitaliseSDLWindow();
	SDL_Renderer* InitaliseSDLRenderer();
	SDL_GLContext InitialiseContext(SDL_Window* mainWindow);
	int InitaliseSDL();
	int InitaliseGlew(SDL_Window* mainWindow);
	bool SetOpenGLAttributes();
	
	Initialise();
	~Initialise();

private:
	Window window;
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* renderer = NULL;
	SDL_GLContext glContext = nullptr;
};

