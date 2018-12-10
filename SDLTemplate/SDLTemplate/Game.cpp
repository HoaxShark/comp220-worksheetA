#include "Game.h"

/* TODO:
	Object Creation Class
	work off my feedback
	joystick controls?
	create light with a base attached to camera with offset
	throwable light
	shadow casting from light?
	
	consider orbiting planets/ moons
	check out factory patterns for gameobject*/

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

	// Initalise Skybox 
	skybox = new Skybox();
}


void Game::gameLoop()
{
	initialiseGame();

	// Mouse setup
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// create shaders
	texturedShader = new Shader();
	texturedShader->Load("blinnPhongVert.glsl", "blinnPhongFrag.glsl");

	lightOrbShader = new Shader();
	lightOrbShader->Load("vertTextured.glsl", "fragTextured.glsl");

	/*particleShader = new Shader();
	particleShader->Load("vertParticle.glsl", "fragParticle.glsl");

	// load particle texture
	particleTextureID = loadTextureFromFile("Model/spark.png");

	Particles = new ParticleGenerator(particleShader , particleTextureID, 10);
	*/

	// Load in objects
	objectManager.loadAllObjects(texturedShader, lightOrbShader);

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

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					for (GameObject * obj : objectManager.GetLightObjectList())
					{
						obj->setWithPlayer(false);
						obj->setThrownDirection(currentPos);
						obj->setThrownNormal(player.camera.getCameraFront());
					}
				}
				else if (event.button.button == SDL_BUTTON_RIGHT)
				{
					for (GameObject * obj : objectManager.GetLightObjectList())
					{
						obj->setWithPlayer(true);
					}
				}

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

		// update objects
		for (GameObject * obj : objectManager.GetGameObjectList())
		{
			obj->Update(deltaTime);
		}

		// update light
		for (GameObject * obj : objectManager.GetLightObjectList())
		{
			// trying to make the light stay with the camera
			vec3 offset = vec3(0.2f, -0.2f, 0.0f);
			vec3 pos = player.camera.getCameraFront();
			vec3 playerPos = player.camera.getCameraPos();
			std::cout << pos.x << pos.y << pos.z << std::endl;
			// add the offset to the normalised vector for where we are looking
			pos = pos + offset;
			// increase the vector
			pos = pos * 10.f;
			currentPos = pos + playerPos;

			if (obj->getWithPlayer())
			{
				// add player pos to vector
				lightObjectPos = currentPos;
			}
			else if (!obj->getWithPlayer())
			{
				vec3 currentDirection = obj->getThrownDirection();
				currentDirection = currentDirection + obj->getThrownNormal();
				obj->setThrownDirection(currentDirection);
				lightObjectPos = currentDirection;
			}

			obj->SetPosition(lightObjectPos.x, lightObjectPos.y, lightObjectPos.z);
			
			obj->Update(deltaTime);
		}

		/*
		// update particles
		Particles->Update(deltaTime, *GameObjectList[0], 2, glm::vec2(0.0f,0.0f));
		*/

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
		view = player.camera.getViewMatrix();
		// close near clip for the skybox
		proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.8f, 1500.0f);

		// draw skybox
		skybox->renderSkybox(player.camera.getViewMatrix(), proj);

		// change the near clip for other objects
		proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 5.0f, 1500.0f);

		/*
		// Draw particles	
		Particles->Draw(proj);
		*/

		// draw objects
		objectManager.drawObjects(objectManager.GetGameObjectList(), view, proj, cameraPosition, lightObjectPos);

		// draw lights
		objectManager.drawObjects(objectManager.GetLightObjectList(), view, proj, cameraPosition, lightObjectPos);

		SDL_GL_SwapWindow(mainWindow);
	}
	// Call all quit functions
	gameQuit();
}

// Clean up resources when the game is exited
void Game::gameQuit()
{
	delete skybox;
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
/* https://learnopengl.com/Advanced-OpenGL/Cubemaps */

