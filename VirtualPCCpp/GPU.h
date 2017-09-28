#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;
typedef uint16_t u16;

struct character {
	std::vector<std::string> rows;
};


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
	std::vector<character> characters;

	GPU();
	GPU(int, int, int, int, SDLWindow*, Memory*);
	void executeCommand();
	void executeThread();
	void tick();
};