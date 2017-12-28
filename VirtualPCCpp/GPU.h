#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Memory.h"
#include "SDLWindow.h"
#include "GPUCore.h"

typedef unsigned char byte;
typedef uint16_t u16;

struct character {
	std::vector<std::string> rows;
};

class GPU {
public:
	Memory commandBuffer;
	Memory commandArgBuffer;
	std::vector<int> stack;
	Memory vRam;
	Memory& ram;
	SDLWindow* screen;
	int programCounter;
	std::vector<character> characters;
	std::vector<GPUCore> cores;

	GPU(int, int, int, int, int, SDLWindow*, Memory&);
	void executeCommand();
	void executeThread();
	void startCores();
	void tick();
};