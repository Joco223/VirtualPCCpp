#pragma once

#include <cstdint>

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
	u16 register1;
	u16 register2;
	byte programCounter;
	GPU& gpu;
	bool halt;

	CPU();
	CPU(int, Memory&, GPU&);
	void execute(u16);
	void tick();
};