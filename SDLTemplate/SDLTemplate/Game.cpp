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

	// bind vertex arrays
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// x,y,z,r,g,b,a, tu, tv
	static const Vertex square[] =
	{
		{ -0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f },
		{ 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f },
		{ 0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f },
		{ -0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f ,0.0f,1.0f }
	};

	static const int indice[] =
	{
		0,1,2,
		2,0,3
	};

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL. Change to 
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), square, GL_STATIC_DRAW);
	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vertTextured.glsl", "fragTextured.glsl");

	GLuint textureID = loadTextureFromFile("Crate.jpg");

	// set buffer location in memory
	glGenBuffers(1, &elementbuffer);
	// bind the buffer (bound until you unbind it, or bind something else)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	// call to copy the data, array type, size of data 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indice, GL_STATIC_DRAW);

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
		//Calculate deltaTime
		lastTime = tickTime;
		tickTime = SDL_GetTicks();
		deltaTime = (tickTime - lastTime);

		// set camera speed relative to delta time
		float cameraSpeed = baseCameraSpeed * deltaTime;

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
					cameraPos += cameraSpeed * cameraFront;
					break;

				case SDLK_a:
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;

				case SDLK_s:
					cameraPos -= cameraSpeed * cameraFront;
					break;

				case SDLK_d:
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
		glClearColor(0.0, 0.5, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Bind program
		glUseProgram(programID);

		mat4 modelMatrix = translate(position);
		// rotate around the z and y axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0, 1.0, 1.0));
		// scale to vector shapeScale
		modelMatrix = glm::scale(modelMatrix, shapeScale);		rotateAngle += 5.0f;
		// note that we're translating the scene in the reverse direction of where we want to move
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// sends across modelMatrix
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1i(textureUniformLocation, 0);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer
		(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(Vertex),                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colour
		glEnableVertexAttribArray(1);
		glVertexAttribPointer
		(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(3 * sizeof(float))
		);

		// 3rd attribute buffer : texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer
		(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			(void*)(7 * sizeof(float))
		);


		
		// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		SDL_GL_SwapWindow(mainWindow);
	}
	//Call all quit functions
	gameQuit();
}

// Clean up resources when the game is exited
void Game::gameQuit()
{
	glDeleteTextures(1, &textureID);
	//Delete Program
	glDeleteProgram(programID);
	//Delete Buffer
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &elementbuffer);
	//Delete Vertex Array
	glDeleteVertexArrays(1, &VertexArrayID);
	//Delete context
	SDL_GL_DeleteContext(gl_Context);
	//Close window
	SDL_DestroyWindow(mainWindow);
	SDL_Quit();
}
