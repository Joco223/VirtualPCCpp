#pragma once

#include <cstdint>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;
typedef uint16_t u16;


class GPU {
public:
	Memory commandBuffer;
	Memory commandArgBuffer;
	Memory vRam;
	Memory* ram;
	SDLWindow* screen;
	u16 functionCounter;
	u16 commandCounter;
	u16 commandArgCounter;
	u16 firstAvailableByte;
	u16 coreCount;

	GPU();
	GPU(int, int, int, int, SDLWindow*, Memory*);
	void executeCommand();
	void executeThread();
	void tick();
};