#include "stdafx.h"
#include "Utility.h"
#include <SDL_image.h>

//Useful function to load an image as an SDL_Texture. Requires the SDL_Image_2.0.3 library.
SDL_Texture * Utility::loadTexture(SDL_Renderer * renderer, char * file)
{
	//Load a texture from file
	SDL_Texture* texture = IMG_LoadTexture(renderer, file);
	return texture;
}
