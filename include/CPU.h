#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

#ifdef _WIN32
#include <SDL_net.h>
#endif

#ifdef __linux
#include <SDL2/SDL_net.h>
#endif

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

	std::vector<unsigned int> registers;
	//std::vector<float> fRegisters;
	std::vector<unsigned char> modemRegisters;
	std::vector<unsigned int> interRegisters;
	std::vector<bool> sockets;

	std::string vPCaddress;
	bool onlineMode;

	std::vector<stack_object> stack;

	unsigned int currentStackPos;
	int programCounter;
	int sectorSize;
	int numSectors;
	int interPos;
	int interTartgetPos;
	int currentIntPos;
	int interFinishPos;
	bool interrupted;
	bool interruptExecution;
	unsigned int currentTime;
	unsigned int currentFrame;
	GPU& gpu;
	bool halt;

	CPU(int, int, Memory&, Memory&, GPU&);
	int checkArgument(int, int);
	int checkArgumentH(int, int);
	int checkArgumentG(int, int);
	void execute(TCPsocket);
	void interrupt();
	void tick(TCPsocket*);
};
