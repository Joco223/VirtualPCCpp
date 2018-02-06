#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h> 

#include "Memory.h"
#include "SDLWindow.h"
#include "GPUCore.h"

typedef unsigned char byte;
typedef uint16_t u16;

struct task_id {
	int x;
	int y;
};

struct screenCharacter {
	byte x;
	byte y;
	byte r;
	byte g;
	byte b;
	byte rB;
	byte gB;
	byte bB;
	byte characterID;
};

class GPU {
public:
	Memory commandBuffer;
	Memory commandArgBuffer;
	std::vector<int> stack;
	Memory vRam;
	Memory& ram;
	SDLWindow* screen;
	int programCounter;
	std::vector<task_id> tasks;
	std::vector<screenCharacter> screenCharacters;
	std::vector<GPUCore> cores;
	SDL_Texture* font;

	GPU(int, int, int, int, SDLWindow*, Memory&);
	void setCharID(byte x, byte y, byte ID);
	void setCharCB(byte x, byte y, byte cB);
	void setCharCF(byte x, byte y, byte cF);
	void loadFont();
	void updateScreen();
	void executeCommand();
	void executeThread();
	void startCores();
	void tick();
};
