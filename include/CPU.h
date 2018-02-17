#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Memory.h"
#include "GPU.h"

typedef unsigned char byte;

struct stack_object {
	std::string name;
	std::vector<unsigned int> parameters;
	std::vector<unsigned int> parametersSize;
	unsigned int BP;
	unsigned int PP;
	int return_register;
};

class CPU {
public:
	Memory& ram;
	Memory& hdd;

	int registerOP;
	int interruptRegister;

	std::vector<unsigned int> registers;

	std::vector<stack_object> stack;

	unsigned int currentStackPos;
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
