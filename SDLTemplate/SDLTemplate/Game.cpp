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

	//Initalise random seed
	std::srand(time(NULL));

	//Initialise times
	float lastTime = 0.0f;
	float tickTime = 0.0f;
	float deltaTime = 0.0f;

	//Initalise the SDL components
	mainWindow = init.initaliseSDLWindow();
	renderer = init.initaliseSDLRenderer();

	//Initalise OpenGL 
	init.SetOpenGLAttributes();
	gl_Context = init.initialiseContext(mainWindow);
	init.initaliseGlew(mainWindow);

	// set camera position
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	// set camera target
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	// calculate camera direction
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	// set the up baseline
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	// get the cross product of up and camera direction to be used as right
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	// get the cross product of up and right to get the up direction
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
}

void Game::gameLoop()
{
	initialiseGame();

	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vertTextured.glsl", "fragTextured.glsl");

	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	//unsigned int numberOfVerts = 0;
	//unsigned int numberOfIndices = 0;
	//loadModelFromFile("Tank1.fbx", vertexbuffer, elementbuffer, numberOfVerts, numberOfIndices);

	std::vector<Mesh*> meshes;
	loadMeshesFromFile("Tank1.fbx", meshes);

	// set MVP matrix locations
	GLuint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLuint viewLocation = glGetUniformLocation(programID, "view");
	GLuint projLocation = glGetUniformLocation(programID, "proj");
	GLuint textureUniformLocation = glGetUniformLocation(programID, "textureSampler");



	// if we want another texture do the following
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, anothertextureID);

	

	//Current sdl event
	SDL_Event event;

	/*------------------------
	Main game loop
	------------------------*/
	while (gameRunning)
	{
		/* TODO fix delta time*/
		//Calculate deltaTime
		//lastTime = tickTime;
		//tickTime = SDL_GetTicks();
		//deltaTime = (tickTime - lastTime);

		// set camera speed relative to delta time
		//float cameraSpeed = baseCameraSpeed * deltaTime;
		//std::cout << lastTime << std::endl;

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

				case SDLK_w:
					cameraPos += baseCameraSpeed * cameraFront;
					break;

				case SDLK_a:
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * baseCameraSpeed;
					break;

				case SDLK_s:
					cameraPos -= baseCameraSpeed * cameraFront;
					break;

				case SDLK_d:
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * baseCameraSpeed;
					break;

				case SDLK_7:
					shapeScale = shapeScale + vec3(-0.01f, -0.01f, 0.0f);
					break;

				case SDLK_8:
					shapeScale = shapeScale + vec3(0.01f, 0.01f, 0.0f);
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
			}
		}

		//Update game and render with openGL
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearDepth(1.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Bind program
		glUseProgram(programID);

		mat4 modelMatrix = translate(position);
		// rotate around the z and y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0, 0.0, 1.0));
		// scale to vector shapeScale
		modelMatrix = glm::scale(modelMatrix, shapeScale);
		// note that we're translating the scene in the reverse direction of where we want to move
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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
	//Call all quit functions
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
	glDeleteTextures(1, &textureID);
	//Delete Program
	glDeleteProgram(programID);
	//Delete context
	SDL_GL_DeleteContext(gl_Context);
	//Close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}
