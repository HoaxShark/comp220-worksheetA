#include "Game.h"

/* TODO:
	move the init to specific classes?
	
	consider orbiting planets/ moons*/

Game::Game()
{
}

Game::~Game()
{
}

void Game::InitialiseGame()
{
	bool inDevelopMode = false;

	// Initalise random seed
	std::srand(time(NULL));

	// Initialise times
	float lastTime = 0.0f;
	float tickTime = 0.0f;
	float deltaTime = 0.0f;

	// Initalise the SDL components
	mainWindow = managerSDL.InitaliseSDLWindow();
	renderer = managerSDL.InitaliseSDLRenderer();

	// Initalise OpenGL 
	managerSDL.SetOpenGLAttributes();
	glContext = managerSDL.InitialiseContext(mainWindow);
	managerSDL.InitaliseGlew(mainWindow);

	// Initalise objectManager
	objectManager = new ObjectManager();

	// Initalise Skybox 
	skybox = new Skybox();
}

void Game::GameLoop()
{
	InitialiseGame();

	// Mouse setup
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Create shaders
	texturedShader = new Shader();
	texturedShader->Load("blinnPhongVert.glsl", "blinnPhongFrag.glsl");

	lightOrbShader = new Shader();
	lightOrbShader->Load("vertTextured.glsl", "fragTextured.glsl");

	// Start particleGenerator
	particleGenerator = new ParticleGenerator(100, objectManager, lightOrbShader);

	// Load in objects
	objectManager->LoadAllObjects(texturedShader, lightOrbShader);

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
				// Window close
			case SDL_QUIT:
				gameRunning = false;
				break;

			case SDL_MOUSEMOTION:
				// pass mouse movements
				player.MouseUpdate(event.motion.xrel, event.motion.yrel);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					for (GameObject * obj : objectManager->GetLightObjectList())
					{
						obj->SetWithPlayer(false);
						obj->SetThrownDirection(objectManager->GetCurrentLightPos());
						obj->SetThrownNormal(player.camera.GetCameraFront());
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					for (GameObject * obj : objectManager->GetLightObjectList())
					{
						obj->SetWithPlayer(true);
					}
				}

			case SDL_KEYDOWN:
				// Update key map
				player.ManageKeyboardEvents(event);

				// Check individual keys by code
				switch (event.key.keysym.sym)
				{
				// Exit the game
				case SDLK_ESCAPE:
					gameRunning = false;
					break;
					
				case SDLK_F11:

					// switch between fullscreen and window
					if (window.GetIsFullscreen()) {
						window.SetIsFullscreen();
						SDL_SetWindowFullscreen(mainWindow, 0);
						glViewport(0, 0, window.screenWidth, window.screenHeight);
					}
					else {
						window.SetIsFullscreen();
						SDL_SetWindowFullscreen(mainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
						SDL_GetWindowSize(mainWindow, &fullWidth, &fullHeight);
						glViewport(0, 0, fullWidth, fullHeight);
					}
					break;
				}
				break;

			case SDL_KEYUP:
				// Update key map
				player.ManageKeyboardEvents(event);
				break;
			}
		}

		// move depending on pressed keys
		player.HandleKeyboard(deltaTime);

		// update objects
		objectManager->UpdateObjectList(objectManager->GetGameObjectList(), player, false, deltaTime);

		// update light
		objectManager->UpdateObjectList(objectManager->GetLightObjectList(), player, true, deltaTime);

		// update particles
		particleGenerator->Update(deltaTime, *objectManager->GetLightObjectList()[0], 2);

		// Update game and render with openGL
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Bind program
		glUseProgram(programID);
	   
		// note that we're translating the scene in the reverse direction of where we want to move
		view = player.camera.GetViewMatrix();
		// close near clip for the skybox
		proj = glm::perspective(glm::radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.8f, 1500.0f);

		// draw skybox
		skybox->RenderSkybox(player.camera.GetViewMatrix(), proj);

		// change the near clip for other objects
		proj = glm::perspective(glm::radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 5.0f, 1500.0f);

		// draw objects
		objectManager->DrawObjects(objectManager->GetGameObjectList(), view, proj, cameraPosition);

		// draw lights
		objectManager->DrawObjects(objectManager->GetLightObjectList(), view, proj, cameraPosition);

		// Draw particles	
		objectManager->DrawObjects(objectManager->GetParticleObjectList(), view, proj, cameraPosition);

		SDL_GL_SwapWindow(mainWindow);
	}
	// Call all quit functions
	GameQuit();
}

// Clean up resources when the game is exited
void Game::GameQuit()
{
	// clear key events
	player.ClearEvents();
	// delete textures
	glDeleteTextures(1, &textureID);
	// delete Program
	glDeleteProgram(programID);
	// delete context
	SDL_GL_DeleteContext(glContext);
	// close window
	SDL_DestroyWindow(mainWindow);
	delete particleGenerator;
	delete objectManager;
	delete skybox;
	delete texturedShader;
	delete lightOrbShader;
	delete skyboxShader;
	SDL_Quit();
}

