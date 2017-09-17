#include "NSSDL.h"
#include "SDLWindow.h"

namespace NSSDL{
	int NSSDL::initSDL(SDLWindow& window, int& width, int& height) {
		window.window = nullptr;
		window.surface = nullptr;
		window.renderer = nullptr;
		window.texture = nullptr;
		window.pixelSpace = nullptr;

		window.window = SDL_CreateWindow("Virtual PC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		window.surface = SDL_GetWindowSurface(window.window);
		window.pixelSpace = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		window.pixels = (unsigned int*)window.pixelSpace->pixels;
		window.renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED);
		window.texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

		return 0;
	}

	void NSSDL::updateSDL(SDLWindow& window) {
		SDL_UpdateTexture(window.texture, NULL, window.pixels, window.pixelSpace->pitch);
		SDL_RenderClear(window.renderer);
		SDL_RenderCopy(window.renderer, window.texture, NULL, NULL);
		SDL_RenderPresent(window.renderer);
	}

	void NSSDL::cleanSDL(SDLWindow& window) {
		SDL_DestroyTexture(window.texture);
		SDL_DestroyRenderer(window.renderer);
		SDL_DestroyWindow(window.window);
	}
}