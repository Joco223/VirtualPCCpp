#pragma once

#include "SDLWindow.h"
#include "Memory.h"
#include "CPU.h"

typedef unsigned char byte;

class PC {
public:
	CPU cpu;
	Memory ram;
	Memory hdd;
	SDLWindow screen;

	PC();
	PC(CPU, Memory, Memory, SDLWindow);
};