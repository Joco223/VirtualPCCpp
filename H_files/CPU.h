#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Memory.h"
#include "GPU.h"

typedef unsigned char byte;

class CPU {
public:
	Memory& ram;
	Memory& hdd;

	int registerOP;
	int interruptRegister;

	std::vector<unsigned int> registers;

	int programCounter;
	int sectorSize;
	int numSectors;
	int interPos;
	int interTartgetPos;
	int currentIntPos;
	bool interrupted;
	int currentTime;
	GPU& gpu;
	bool halt;

	CPU(int, int, Memory&, Memory&, GPU&);
	int checkArgument(int, int);
	int checkArgumentH(int, int);
	int checkArgumentG(int, int);
	void execute();
	void interrupt();
	void tick();
};
