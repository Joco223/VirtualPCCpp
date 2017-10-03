#pragma once

#include "SDLWindow.h"

namespace NSSDL {
	int initSDL(SDLWindow* window, int& width, int &height);
	void updateSDL(SDLWindow* window);
	void cleanSDL(SDLWindow* window);
}