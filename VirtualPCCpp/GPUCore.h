#pragma once

#include <vector>
#include <iostream>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;

class GPUCore {
public:
	Memory& vRam;
	Memory intMem;
	SDLWindow* screen;
	int registerOP;
	int register0;
	int register1;
	int programCounter;
	byte regR, regG, regB;
	int idX;
	int idY;
	bool halt;

	GPUCore(Memory&, SDLWindow*, int, int, int);
	int checkArgumentV(int, int);
	int checkArgumentInt(int, int);
	void execute(int);
	void tick();
};