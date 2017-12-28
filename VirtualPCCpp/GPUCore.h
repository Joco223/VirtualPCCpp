#pragma once

#include <vector>
#include <iostream>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;

class GPUCore {
public:
	Memory& vRam;
	SDLWindow* screen;
	int registerOP;
	int register0;
	int register1;
	byte regR;
	byte regG;
	byte regB;
	int programCounter;
	int idX;
	int idY;
	bool halt;

	GPUCore(Memory&, SDLWindow*, int, int);
	int checkArgument(int, int);
	void execute(int);
	void tick();
};