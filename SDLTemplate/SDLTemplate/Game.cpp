#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}

void Game::initialiseGame()
{
	bool inDevelopMode = false;

	// Initalise random seed
	std::srand(time(NULL));

	// Initialise times
	float lastTime = 0.0f;
	float tickTime = 0.0f;
	float deltaTime = 0.0f;

	// Initalise the SDL components
	mainWindow = init.initaliseSDLWindow();
	renderer = init.initaliseSDLRenderer();

	// Initalise OpenGL 
	init.SetOpenGLAttributes();
	gl_Context = init.initialiseContext(mainWindow);
	init.initaliseGlew(mainWindow);
}

void Game::gameLoop()
{
	initialiseGame();

	// Mouse setup
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vertTextured.glsl", "fragTextured.glsl");

	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	std::vector<Mesh*> meshes;
	loadMeshesFromFile("Tank1.fbx", meshes);

	// set MVP matrix locations
	GLuint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLuint viewLocation = glGetUniformLocation(programID, "view");
	GLuint projLocation = glGetUniformLocation(programID, "proj");
	GLuint textureUniformLocation = glGetUniformLocation(programID, "textureSampler");

	// Current sdl event
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
		while (SDL_PollEvent(&event))
		{
			// Events found
			switch (event.type)
			{
				// Window closed
			case SDL_QUIT:
				gameRunning = false;
				break;

			case SDL_MOUSEMOTION:
				// pass event.motion.xrel and event.motion.yrel here
				player.mouseUpdate(event.motion.xrel, event.motion.yrel);
				break;

			case SDL_KEYDOWN:
				// Update key map
				player.manageKeyboardEvents(event);

				// Check individual keys by code
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
						glViewport(0, 0, window.screenWidth, window.screenHeight);
					}
					else {
						window.setIsFullscreen();
						SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
						SDL_GetWindowSize(mainWindow, &fullWidth, &fullHeight);
						glViewport(0, 0, fullWidth, fullHeight);
					}
					break;
				}
				break;

			case SDL_KEYUP:
				// Update key map
				player.manageKeyboardEvents(event);
				break;
			}
		}

		// move depending on pressed keys
		player.handleKeyboard(deltaTime);

		// Update game and render with openGL
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Bind program
		glUseProgram(programID);

		mat4 modelMatrix = translate(position);

		// note that we're translating the scene in the reverse direction of where we want to move
		glm::mat4 view;
		view = glm::lookAt(player.camera.getCameraPos(), player.camera.getCameraPos() + player.camera.getCameraFront(), player.camera.getCameraUp());
		// sends across modelMatrix
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(textureUniformLocation, 0);


		// draw loop
		for (Mesh*currentMesh : meshes)
		{
			currentMesh->render();
		}

		SDL_GL_SwapWindow(mainWindow);
	}
	// Call all quit functions
	gameQuit(meshes);
}

// Clean up resources when the game is exited
void Game::gameQuit(std::vector<Mesh*> meshes)
{
	// Delete the data in meshes - can't use the standard for loop as you can't modifiy anything instide the loop
	auto iter = meshes.begin();
	// while iter isn't the end of meshes
	while (iter != meshes.end())
	{
		// check iter has memory
		if ((*iter))
		{
			// destroy the mesh
			(*iter)->destroy();
			// delete the memory
			delete (*iter);
			// erase the slot in the vector and return new iter
			iter = meshes.erase(iter);
		}
		else
		{
			// if no memory move on
			iter++;
		}
	}
	// final flush of vector
	meshes.clear();
	// clear key events
	player.clearEvents();
	// delete textures
	glDeleteTextures(1, &textureID);
	// delete Program
	glDeleteProgram(programID);
	// delete context
	SDL_GL_DeleteContext(gl_Context);
	// close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}
