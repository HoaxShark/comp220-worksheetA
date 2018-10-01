#pragma once
#include <SDL.h>
#include <iostream>

class Utility
{
public:
	Utility() {};
	~Utility() {};

	//Load a texture from file. Unneeded as SDL_Image provides this function.
	SDL_Texture* loadTexture(SDL_Renderer* renderer, char* file);
};