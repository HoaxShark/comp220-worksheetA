#include "Texture.h"

GLuint loadTextureFromFile(const std::string& filename)
{
	GLuint textureID;


	GLenum	textureFormat = GL_RGB;
	GLenum	internalFormat = GL_RGB8;

	// load img to surface from file
	SDL_Surface * surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		printf("Could not load file %s", IMG_GetError());
		return 0;
	}

	// check for number of colours per pixel
	GLint	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)					//	contains	an	alpha	channel
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else {
			textureFormat = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColors == 3)					//	no	alpha	channel
	{
		if (surface->format->Rmask == 0x000000ff) {
			textureFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else {
			textureFormat = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}

	// generate textureID
	glGenTextures(1, &textureID);
	// bind textureID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Copy from surface to openGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);


	SDL_FreeSurface(surface);

	return textureID;
}