// Main.cpp : Defines the entry point for the console application.

#include "Main.h"

// Create Initialisation class
Initialise init;

// Create window class for managing fullscreen
Window window;

//Game loop runs while true
bool gameRunning = true;

//The SDL renderer. Should be passed to class functions that handle their own rendering.
SDL_Renderer* renderer = NULL;

//The main game window.
SDL_Window* mainWindow = nullptr;

// Create context for openGL
SDL_GLContext gl_Context = nullptr;


int main(int argc, char *argv[])
{
	bool inDevelopMode = false;

	//Initalise random seed
	std::srand(time(NULL));

	//Initialise times
	float lastTime = 0;
	float tickTime = 0;
	float deltaTime = 0;

	//Initalise the SDL components
	mainWindow = init.initaliseSDLWindow();
	renderer = init.initaliseSDLRenderer();

	//Initalise OpenGL 
	init.SetOpenGLAttributes();
	gl_Context = init.initialiseContext(mainWindow);
	init.initaliseGlew(mainWindow);

	//Current sdl event
	SDL_Event event;

	/*------------------------
	Main game loop
	------------------------*/
	while (gameRunning)
	{
		//Calculate deltaTime
		lastTime = tickTime;
		tickTime = SDL_GetTicks();
		deltaTime = (tickTime - lastTime);

		//Check for SDL events
		if (SDL_PollEvent(&event))
		{
			//Events found
			switch (event.type)
			{
					//Window closed
				case SDL_QUIT:
					gameRunning = false;
					break;

				case SDL_KEYDOWN:

					//Check individual keys by code (can be moved out into main switch statement if fewer keys need to be checked.)
					switch (event.key.keysym.sym)
					{
						// Exit the game
						case SDLK_ESCAPE:
							gameRunning = false;
							break;

						case SDLK_F11:
							
							// switch between fullscreen and window
							if (window.getIsFullscreen()) {
								window.setIsFullscreen();
								SDL_SetWindowFullscreen(mainWindow, 0);
							}
							else {
								window.setIsFullscreen();
								SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
							}
							break;
					}

				break;
			}
		}

		//Update game and render with openGL
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		SDL_GL_SwapWindow(mainWindow);
	}
	//Delete context
	SDL_GL_DeleteContext(gl_Context);
	//Close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
	return 0;
}