#pragma once

#include <cstdint>

#include "Memory.h"
#include "GPU.h"

typedef unsigned char byte;
typedef uint16_t u16;

class CPU {
private:
	Memory cache;
	Memory& ram;
public:
	u16 registerOP1;
	u16 register1;
	u16 register2;
	u16 programCounter;
	GPU& gpu;
	bool halt;

	void accessCache(std::string, u16, byte, byte&);
	void accessRam(std::string, u16, byte, byte&);

	CPU();
	CPU(int, Memory&, GPU&);
	void execute(u16);
	void tick();
};