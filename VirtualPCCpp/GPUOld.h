#pragma once

#include <cstdint>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;
typedef uint16_t u16;

class GPU {
public:
	Memory commandBuffer;
	Memory argBuffer;
	Memory functionBuffer;
	Memory builtIn;
	Memory characters;
	SDLWindow& screen;
	u16 programCounter;
	u16 commandCounter;
	u16 programSize;

	GPU();
	GPU(byte, u16, SDLWindow&);
	void executeThreads(u16);
	void spawnThreads(byte, u16, u16, u16, u16, u16, u16, u16);
	void execute();
	void tick();
};