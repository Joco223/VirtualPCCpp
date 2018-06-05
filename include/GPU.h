#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <mutex>

#include "Memory.h"
#include "SDLWindow.h"
#include "GPUCore.h"

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

struct screenPos {
	int x;
	int y;
};

extern int scale;

class GPU {
public:
	Memory vRam;
	Memory& ram;
	SDLWindow* screen;
	int programCounter;
	std::vector<task_id> tasks;
	std::vector<screenCharacter> screenCharacters;
	std::vector<GPUCore> cores;
	std::vector<screenPos> charactersNUpdate;
	SDL_Texture* font;
	SDL_Texture* characterBuffer;
	bool update;
	int coresX, coresY;
	bool started;
	int currentTask;
	bool screenUpdated;
	int tasksX, tasksY;
	int targetPC;

	GPU(int, int, int, SDLWindow*, Memory&);
	void setCharID(byte x, byte y, byte ID);
	void setCharCB(byte x, byte y, byte cB);
	void setCharCF(byte x, byte y, byte cF);
	void updateCharacters();
	void loadFont();
	void updateScreen();
	void executeCommand();
	void executeThread();
	void setPC(int);
	void tick();
};
