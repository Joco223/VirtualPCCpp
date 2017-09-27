#include "PC.h"

PC::PC()
	:
	cpu(),
	ram(),
	hdd() {}

PC::PC(CPU cpu_, Memory ram_, Memory hdd_, SDLWindow* screen_) 
	:
	cpu(cpu_),
	ram(ram_),
	hdd(hdd_),
	screen(screen_)
{}