#pragma once

#ifdef _WIN32
#include <SDL.h>
#endif

#ifdef _LINUX
#include <SDL/SDL.h>
#endif

class SDLWindow {
public:
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Surface* pixelSpace;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	unsigned int* pixels;

	SDLWindow(SDL_Window*&, SDL_Surface*&, SDL_Surface*&, SDL_Renderer*&, SDL_Texture*&, unsigned int*&);
};

