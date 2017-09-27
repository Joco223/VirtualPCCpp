#pragma once

class SDLWindow;

namespace NSSDL {
	int initSDL(SDLWindow*& window, int& width, int &height);
	void updateSDL(SDLWindow*& window);
	void cleanSDL(SDLWindow*& window);
}