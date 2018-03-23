#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Memory.h"
#include "SDLWindow.h"

class GPUCore {
public:
	Memory& vRam;
	Memory& progMem;
	int coresYS;
	SDLWindow* screen;
	int registerOP;
	std::vector<unsigned int> registers;
	int programCounter;
	int idX;
	int idY;
	bool halt;

	GPUCore(Memory&, Memory&, SDLWindow*, int, int, int);
	byte getBits(byte, byte);
	int checkArgument(int, int);
	void execute(int);
	void tick();
};
