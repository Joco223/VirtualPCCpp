#pragma once

#include <SDL.h>

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

