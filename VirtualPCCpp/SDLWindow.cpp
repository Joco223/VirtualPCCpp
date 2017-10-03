#include "SDLWindow.h"

SDLWindow::SDLWindow(SDL_Window*& window_, SDL_Surface*& surface_, SDL_Surface*& pixelSpace_, SDL_Renderer*& renderer_, SDL_Texture*&  texture_, unsigned int*& pixels_) {
	window = window_;
	surface = surface_;
	pixelSpace = pixelSpace_;
	renderer = renderer_;
	texture = texture_;
	pixels = pixels_;
}