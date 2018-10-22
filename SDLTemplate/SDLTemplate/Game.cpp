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
}

void Game::gameLoop()
{
	initialiseGame();

	// bind vertex arrays
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// x,y,z,r,g,b,a
	static const Vertex square[] =
	{
		{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
		{0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f},
		{0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f},
	};

	static const int indice[] =
	{
		0,1,2,
		2,0,3
	};

	// An array of 4 vectors which represents 8 vertices
	// {x,y,z,r,g,b,a}
	static const Vertex cubeVertexArray[] =
	{
		{ -0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f,  0.0f, 0.0f},
		{ 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f},
		{ 0.5f,0.5f,0.0f,  1.0f,1.0f,0.0f,1.0f,  0.0f, 0.0f},
		{ -0.5f,0.5f,0.0f,  1.0f,1.0f,1.0f,1.0f,  0.0f, 0.0f},

		{ -0.5f,-0.5f,-1.0f,  1.0f,0.0f,0.0f,1.0f,  0.0f, 0.0f},
		{ 0.5f,-0.5f,-1.0f,  1.0f,1.0f,0.0f,1.0f,  0.0f, 0.0f},
		{ 0.5f,0.5f,-1.0f,  0.0f,0.0f,1.0f,1.0f,  0.0f, 0.0f},
		{ -0.5f,0.5f,-1.0f,  0.0f,1.0f,0.0f,1.0f,  0.0f, 0.0f}

	};

	// Indicies must be set in anti-clockwise if on the outside of the cube order due to back-face culling
	static const int cubeIndiciesArray[] =
	{
		0,1,2, // Represenative of one triangle
		2,3,0,

		6,5,4,
		4,7,6,

		7,3,2,
		2,6,7,

		6,2,1,
		1,5,6,

		3,7,4,
		4,0,3,

		1,0,5,
		5,0,4
	};

	// Culls the clockwise facing side of the triangle
	glEnable(GL_CULL_FACE);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL. Change to 
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), cubeVertexArray, GL_STATIC_DRAW);
	// Hold shader programme, rename to what the ID does
	GLuint programID = LoadShaders("vert.glsl", "frag.glsl");


	// set buffer location in memory
	glGenBuffers(1, &elementbuffer);
	// bind the buffer (bound until you unbind it, or bind something else)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	// call to copy the data, array type, size of data 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), cubeIndiciesArray, GL_STATIC_DRAW);

	// set MVP matrix locations
	GLuint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	GLuint viewLocation = glGetUniformLocation(programID, "view");
	GLuint projLocation = glGetUniformLocation(programID, "proj");

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

				case SDLK_UP:
					position = position + vec3(0.0f, 0.1f, 0.0f);
					break;

				case SDLK_LEFT:
					position = position + vec3(-0.1f, 0.0f, 0.0f);
					break;

				case SDLK_RIGHT:
					position = position + vec3(0.1f, 0.0f, 0.0f);
					break;

				case SDLK_DOWN:
					position = position + vec3(0.0f, -0.1f, 0.0f);
					break;

				case SDLK_7:
					shapeScale = shapeScale + vec3(-0.01f, -0.01f, 0.0f);
					break;

				case SDLK_8:
					shapeScale = shapeScale + vec3(0.01f, 0.01f, 0.0f);
					break;

				case SDLK_9:
					viewPos = viewPos + vec3(0.0f, 0.0f, -0.1f);
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

		//Bind program
		glUseProgram(programID);

		mat4 modelMatrix = translate(position);
		// rotate around the z axis
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateAngle), glm::vec3(0.0, 0.0, 1.0));
		// scale to vector shapeScale
		modelMatrix = glm::scale(modelMatrix, shapeScale);		rotateAngle += 0.0f;
		// note that we're translating the scene in the reverse direction of where we want to move
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.001f));

		// sends across modelMatrix
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(proj));

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

		// 2nd attribute buffer : r,g,b,a
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
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
		glDisableVertexAttribArray(0);

		SDL_GL_SwapWindow(mainWindow);
	}
	//Call all quit functions
	gameQuit();
}

// Clean up resources when the game is exited
void Game::gameQuit()
{
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
