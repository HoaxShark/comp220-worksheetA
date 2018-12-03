#include "Game.h"

/*
The particle generator class isn't currently in use, I was struggling to get it
to work so have been doing it another way with generating objects with the same object creation as previously

TODO:
	Object Creation Class
	use renderDoc for debugging very important for the coursework
	work off my feedback
	joystick controls
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


	
	// generate planets
	createObject("Model/egypt.fbx", "Model/colour.png", 0.0f, 40.0f, -300.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 1.0f, 0.0f), -0.1f);
	createObject("Model/forest.fbx", "Model/colour.png", 100.0f, -40.0f, -150.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.4f, 1.0f, 0.0f), 0.1f);
	createObject("Model/ice.fbx", "Model/colour.png", -300.0f, -20.0f, -450.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.5f, 0.5f), 0.1f);
	createObject("Model/iceGray.fbx", "Model/colour.png", 100.0f, -140.0f, -250.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 1.0f), -0.1f);
	createObject("Model/orange.fbx", "Model/colour.png", 600.0f, 100.0f, -650.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 1.0f, 0.0f), 0.1f);
	createObject("Model/pine.fbx", "Model/colour.png", -500.0f, -200.0f, -450.0f, vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 1.0f, 0.0f), 0.1f);

	// create light object
	createLightObject("Model/myCube.fbx", "Model/light2.png", 10.0f, 10.0f, -20.0f, vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), 1.4f, 0.001f);
	createLightObject("Model/myCube.fbx", "Model/light2.png", 10.0f, 10.0f, -20.0f, vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), 1.2f, 0.002f);
	createLightObject("Model/myCube.fbx", "Model/light2.png", 10.0f, 10.0f, -20.0f, vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.001f);
	createLightObject("Model/myCube.fbx", "Model/light2.png", 10.0f, 10.0f, -20.0f, vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), 0.8f, 0.0005f);

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

		// update light
		for (GameObject * obj : LightObjectList)
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
			// add player pos to vector
			pos = pos + playerPos;
			obj->SetPosition(pos.x, pos.y, pos.z);
			
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

		// draw skybox
		skybox->renderSkybox(player.camera.getViewMatrix(), proj);

		// note that we're translating the scene in the reverse direction of where we want to move
		view = player.camera.getViewMatrix();
		proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.1f, 6000.0f);

		/*
		// Draw particles	
		Particles->Draw(proj);
		*/

		// draw objects
		drawObjects(GameObjectList);

		// draw lights
		drawObjects(LightObjectList);

		SDL_GL_SwapWindow(mainWindow);
	}
	// Call all quit functions
	gameQuit();
}


/* Creates a new game object and stores it in the objectList
takes file and texture locations, x,y,z positions, vec3 for scale, and vec3 for the axis to rotate around, and a rotation speed, set speed to 0 for no rotation*/
void Game::createObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed)
{
	MeshCollection * Meshes = new MeshCollection();
	loadMeshesFromFile(fileLocation, Meshes);

	textureID = loadTextureFromFile(textureLocation);

	GameObject * GO = new GameObject();
	GO->SetPosition(posX, posY, posZ);
	GO->SetMesh(Meshes);
	GO->setScale(scale);
	GO->setRotationAxis(rotationAxis);
	GO->setRotationSpeed(speed);
	GO->SetShader(texturedShader);
	GO->SetDiffuseTexture(textureID);
	GameObjectList.push_back(GO);
}

/* Creates a new game object and stores it in the objectList
takes file and texture locations, x,y,z positions, vec3 for scale, and vec3 for the axis to rotate around, and a rotation speed, set speed to 0 for no rotation*/
void Game::createLightObject(const std::string & fileLocation, const std::string & textureLocation, float posX, float posY, float posZ, glm::vec3 scale, glm::vec3 rotationAxis, float speed, float scaleFactor)
{
	MeshCollection * Meshes = new MeshCollection();
	loadMeshesFromFile(fileLocation, Meshes);

	textureID = loadTextureFromFile(textureLocation);

	GameObject * GO = new GameObject();
	GO->SetPosition(posX, posY, posZ);
	GO->SetMesh(Meshes);
	GO->setScale(scale);
	GO->setRotationAxis(rotationAxis);
	GO->setRotationSpeed(speed);
	GO->setScaleFactor(scaleFactor);
	GO->setIsParticle(true);
	GO->SetShader(texturedShader);
	GO->SetDiffuseTexture(textureID);
	LightObjectList.push_back(GO);
}

/* Renders game objects from a given list */
void Game::drawObjects(std::vector<GameObject*> list)
{
	for (GameObject * obj : list) {

		Shader * currentShader = obj->GetShader();
		currentShader->Use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, obj->GetDiffuseTexture());

		glUniformMatrix4fv(currentShader->GetUniform("modelMatrix"), 1, GL_FALSE, glm::value_ptr(obj->GetModelTransformation()));
		glUniformMatrix4fv(currentShader->GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(currentShader->GetUniform("proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(currentShader->GetUniform("diffuseTexture"), 0);
		// lighting
		glUniform4fv(currentShader->GetUniform("ambientMaterialColour"), 1, glm::value_ptr(ambientMaterialColour));
		glUniform4fv(currentShader->GetUniform("diffuseMaterialColour"), 1, glm::value_ptr(diffuseMaterialColour));
		glUniform4fv(currentShader->GetUniform("specularMaterialColour"), 1, glm::value_ptr(specularMaterialColour));

		glUniform4fv(currentShader->GetUniform("ambientLightColour"), 1, glm::value_ptr(ambientLightColour));
		glUniform4fv(currentShader->GetUniform("diffuseLightColour"), 1, glm::value_ptr(diffuseLightColour));
		glUniform4fv(currentShader->GetUniform("specularLightColour"), 1, glm::value_ptr(specularLightColour));

		glUniform3fv(currentShader->GetUniform("lightDirection"), 1, glm::value_ptr(lightDirection));
		glUniform1f(currentShader->GetUniform("specularMaterialPower"), specularMaterialPower);

		glUniform3fv(currentShader->GetUniform("cameraPosition"), 1, glm::value_ptr(cameraPosition));

		obj->Render();
	}
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

