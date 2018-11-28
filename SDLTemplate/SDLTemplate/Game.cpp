#include "Game.h"

/* TODO:
	REFACTOR SKYBOX CODE AND DELETE MEMORY ON CLOSE
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
}

void Game::gameLoop()
{
	initialiseGame();

	// Mouse setup
	SDL_ShowCursor(0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Hold shader programme, rename to what the ID does
	//GLuint programID = LoadShaders("vertTextured.glsl", "fragTextured.glsl");

	// create shaders
	texturedShader = new Shader();
	texturedShader->Load("vertTextured.glsl", "fragTextured.glsl");

	skyboxShader = new Shader();
	skyboxShader->Load("vertSkybox.glsl", "fragSkybox.glsl");

	// skybox vertices
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// generate array and buffer for the skybox
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


	// generate planets
	createObject("Model/egypt.fbx", "Model/colour.png", 0.0f, 40.0f, -300.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 1.0f, 0.0f), -0.1f);
	createObject("Model/forest.fbx", "Model/colour.png", 100.0f, -40.0f, -150.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.4f, 1.0f, 0.0f), 0.1f);
	createObject("Model/ice.fbx", "Model/colour.png", -300.0f, -20.0f, -450.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.5f, 0.5f), 0.1f);
	createObject("Model/iceGray.fbx", "Model/colour.png", 100.0f, -140.0f, -250.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 1.0f), -0.1f);
	createObject("Model/orange.fbx", "Model/colour.png", 600.0f, 100.0f, -650.0f, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 1.0f, 0.0f), 0.1f);
	createObject("Model/pine.fbx", "Model/colour.png", -500.0f, -200.0f, -450.0f, vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 1.0f, 0.0f), 0.1f);

	// create vector to hold skybox image locations
	std::vector<std::string> skyboxFaces
	{
		"Skybox/right.png",
		"Skybox/left.png",
		"Skybox/top.png",
		"Skybox/bottom.png",
		"Skybox/front.png",
		"Skybox/back.png"
	};

	unsigned int cubemapTexture = loadCubemap(skyboxFaces);

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

		// draw skybox
		glDepthMask(GL_FALSE);
		skyboxShader->Use();
		view = glm::mat4(glm::mat3(player.camera.getViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetShaderProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetShaderProgramID(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		// note that we're translating the scene in the reverse direction of where we want to move
		view = player.camera.getViewMatrix();
		proj = perspective(radians(45.0f), (float)window.screenWidth / (float)window.screenHeight, 0.1f, 6000.0f);
		
		// draw objects
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

unsigned int Game::loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		SDL_Surface * surface = IMG_Load(faces[i].c_str());
		if (surface == nullptr)
		{
			printf("Could not load file %s", IMG_GetError());
			return 0;
		}
		else
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
			SDL_FreeSurface(surface);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// Clean up resources when the game is exited
void Game::gameQuit()
{
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

