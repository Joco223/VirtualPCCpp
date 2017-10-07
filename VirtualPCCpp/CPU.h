#pragma once

#include <cstdint>
#include <iostream>

#include "Memory.h"
#include "GPU.h"

typedef unsigned char byte;
typedef uint16_t u16;

class CPU {
public:
	Memory cache;
	Memory& ram;
	u16 registerOP1;
	u16 registerOP2;
	int register0;
	int register1;
	int programCounter;
	u16 keyboardRegister;
	u16 inputCount;
	u16 firstAvailable;
	u16 currentTime;
	GPU& gpu;
	bool halt;

	CPU(int, Memory&, GPU&);
	void execute(u16);
	void tick();
};