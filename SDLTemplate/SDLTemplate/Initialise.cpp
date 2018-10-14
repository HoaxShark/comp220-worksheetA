#include "Initialise.h"

Initialise::Initialise()
{
	initaliseSDL();
}


Initialise::~Initialise()
{
}


int Initialise::initaliseSDL() 
{
	//initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Cannot initalise SDL " << SDL_GetError() << std::endl;
		return -1;
	}
	return 0;
}

SDL_Window * Initialise::initaliseSDLWindow()
{
	//Initalise the main window
	mainWindow = SDL_CreateWindow(window.windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window.screenWidth, window.screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (mainWindow == nullptr)
	{
		std::cout << "Cannot create window " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}

	return mainWindow;
}

SDL_Renderer * Initialise::initaliseSDLRenderer()
{
	//Initalise renderer
	renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL)
	{
		std::cout << "Cannot initalise SDL" << SDL_GetError << std::endl;
		SDL_Quit();
		return nullptr;
	}

	return renderer;
}