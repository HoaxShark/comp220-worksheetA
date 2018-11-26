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

	texturedShader = new Shader();
	texturedShader->Load("vertTextured.glsl", "fragTextured.glsl");

	/*MeshCollection * tankMeshes = new MeshCollection();
	loadMeshesFromFile("Tank1.fbx", tankMeshes);

	textureID = loadTextureFromFile("Tank1DF.png");

	GameObject * tankGO = new GameObject();
	tankGO->SetPosition(0.0f, 0.0f, -10.0f);
	tankGO->SetMesh(tankMeshes);
	tankGO->setScale(vec3(0.5f, 0.5f, 0.5f));
	tankGO->SetShader(texturedShader);
	tankGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(tankGO);*/
	//////////////////////
	MeshCollection * egyptMeshes = new MeshCollection();
	loadMeshesFromFile("Model/egypt.fbx", egyptMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * egyptGO = new GameObject();
	egyptGO->SetPosition(0.0f, 30.0f, -100.0f);
	egyptGO->SetMesh(egyptMeshes);
	egyptGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	egyptGO->SetShader(texturedShader);
	egyptGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(egyptGO);
	//////////////////
	MeshCollection * forestMeshes = new MeshCollection();
	loadMeshesFromFile("Model/forest.fbx", forestMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * forestGO = new GameObject();
	forestGO->SetPosition(250.0f, 0.0f, -40.0f);
	forestGO->SetMesh(forestMeshes);
	forestGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	forestGO->SetShader(texturedShader);
	forestGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(forestGO);
	////////////////////////
	MeshCollection * havayMeshes = new MeshCollection();
	loadMeshesFromFile("Model/havay.fbx", havayMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * havayGO = new GameObject();
	havayGO->SetPosition(150.0f, 0.0f, -40.0f);
	havayGO->SetMesh(havayMeshes);
	havayGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	havayGO->SetShader(texturedShader);
	havayGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(havayGO);
	/////////////////////////
	MeshCollection * iceMeshes = new MeshCollection();
	loadMeshesFromFile("Model/ice.fbx", iceMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * iceGO = new GameObject();
	iceGO->SetPosition(-150.0f, 0.0f, -40.0f);
	iceGO->SetMesh(iceMeshes);
	iceGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	iceGO->SetShader(texturedShader);
	iceGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(iceGO);
	/////////////////////////
	MeshCollection * iceGrayMeshes = new MeshCollection();
	loadMeshesFromFile("Model/ice_gray.fbx", iceGrayMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * iceGrayGO = new GameObject();
	iceGrayGO->SetPosition(-250.0f, 50.0f, 40.0f);
	iceGrayGO->SetMesh(iceGrayMeshes);
	iceGrayGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	iceGrayGO->SetShader(texturedShader);
	iceGrayGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(iceGrayGO);
	/////////////////////////
	MeshCollection * orangeMeshes = new MeshCollection();
	loadMeshesFromFile("Model/orange.fbx", orangeMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * orangeGO = new GameObject();
	orangeGO->SetPosition(200.0f, -60.0f, 240.0f);
	orangeGO->SetMesh(orangeMeshes);
	orangeGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	orangeGO->SetShader(texturedShader);
	orangeGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(orangeGO);
	/////////////////////////
	MeshCollection * pineMeshes = new MeshCollection();
	loadMeshesFromFile("Model/pine.fbx", pineMeshes);

	textureID = loadTextureFromFile("Model/colour.png");

	GameObject * pineGO = new GameObject();
	pineGO->SetPosition(-150.0f, 40.0f, -200.0f);
	pineGO->SetMesh(pineMeshes);
	pineGO->setScale(vec3(0.1f, 0.1f, 0.1f));
	pineGO->SetShader(texturedShader);
	pineGO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(pineGO);


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
		glClearColor(0.0, 0.0, 0.0, 1.0);
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


/* Needs work isn't currently working correctly*/
void Game::createObject(const std::string & fileLocation, const std::string & textureLocation)
{
	MeshCollection * Meshes = new MeshCollection();
	loadMeshesFromFile(fileLocation, Meshes);

	textureID = loadTextureFromFile(textureLocation);

	GameObject * GO = new GameObject();
	GO->SetPosition(0.0f, 0.0f, -30.0f);
	GO->SetMesh(Meshes);
	GO->setScale(vec3(0.1f, 0.1f, 0.1f));
	GO->SetShader(texturedShader);
	GO->SetDiffuseTexture(textureID);

	GameObjectList.push_back(GO);
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
