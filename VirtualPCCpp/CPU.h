#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include "Memory.h"
#include "GPU.h"

typedef unsigned char byte;
typedef uint16_t u16;

class CPU {
public:
	Memory cache;
	Memory& ram;
	int registerOP;
	int register0;
	int register1;
	int programCounter;
	int cacheSize;
	int firstAvailableS;
	std::vector<int> stack;
	u16 keyboardRegister;
	u16 currentTime;
	GPU& gpu;
	bool halt;

	CPU(int, Memory&, GPU&);
	int checkArgument(int, int);
	void execute(u16);
	void tick();
};