#include "Game.h"

/* TODO:
	use renderDoc for debugging very important for the coursework
	game obejct class
	mesh collection?
	work off my feedback
	joystick controls
	render the game scene
	create light with a base attached to camera with offset
	throwable light
	shadow casting from light
	
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
}

void Game::gameLoop()
{
	initialiseGame();

	// Mouse setup
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Hold shader programme, rename to what the ID does
	//GLuint programID = LoadShaders("vertTextured.glsl", "fragTextured.glsl");

	MeshCollection * tankMeshes = new MeshCollection();
	loadMeshesFromFile("Tank1.fbx", tankMeshes);

	Shader * texturedShader = new Shader();
	texturedShader->Load("vertTextured.glsl", "fragTextured.glsl");

	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	GameObject * tankGO = new GameObject();
	tankGO->SetPosition(0.0f, 0.0f, -10.0f);
	tankGO->SetMesh(tankMeshes);
	tankGO->setScale(vec3(0.5f, 0.5f, 0.5f));
	tankGO->SetShader(texturedShader);
	tankGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(tankGO);

	MeshCollection * egyptMeshes = new MeshCollection();
	loadMeshesFromFile("Model/egypt.fbx", egyptMeshes);

	textureID = loadTextureFromFile("Model/color_webgl.png");

	GameObject * egyptGO = new GameObject();
	egyptGO->SetPosition(0.0f, 0.0f, -30.0f);
	egyptGO->SetMesh(egyptMeshes);
	egyptGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	egyptGO->SetShader(texturedShader);
	egyptGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(egyptGO);

	//std::vector<Mesh*> meshes;
	//loadMeshesFromFile("Tank1.fbx", meshes);



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

		// update objects
		for (GameObject * obj : GameObjectList)
		{
			obj->Update(deltaTime);
		}

		// Update game and render with openGL
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Bind program
		glUseProgram(programID);

		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::lookAt(player.camera.getCameraPos(), player.camera.getCameraPos() + player.camera.getCameraFront(), player.camera.getCameraUp());
		proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.1f, 1000.0f);


		// draw loop
		
		for (GameObject * obj : GameObjectList) {

			Shader * currentShader = obj->GetShader();
			currentShader->Use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, obj->GetDiffuseTexture());

			glUniformMatrix4fv(currentShader->GetUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(obj->GetModelTransformation()));
			glUniformMatrix4fv(currentShader->GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(currentShader->GetUniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
			//glUniform1f(currentShader->GetUniform("morphBlendAlpha"), morphBlendAlpha);
			glUniform1i(currentShader->GetUniform("diffuseTexture"), 0);

			
			obj->Render();
		}

		SDL_GL_SwapWindow(mainWindow);
	}
	// Call all quit functions
	gameQuit();
}

// Clean up resources when the game is exited
void Game::gameQuit()
{
	// call mesh collection destroy?

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
