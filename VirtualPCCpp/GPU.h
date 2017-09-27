#pragma once

#include <cstdint>
#include <string>

#include "Memory.h"
#include "SDLWindow.h"

typedef unsigned char byte;
typedef uint16_t u16;


class GPU {
private:
	Memory commandBuffer;
	Memory commandArgBuffer;
	Memory vRam;
	Memory* ram;
	SDLWindow* screen;
public:
	u16 functionCounter;
	u16 commandCounter;
	u16 commandArgCounter;
	u16 firstAvailableByte;
	u16 coreCount;

	void accessCommandBuffer(std::string, u16, byte, byte&);
	void accessCommandArgBuffer(std::string, u16, byte, byte&);
	void accessVRam(std::string, u16, byte, byte&);

	GPU();
	GPU(u16, u16, u16, u16, SDLWindow*, Memory*);
	void executeCommand();
	void executeThread();
	void tick();
};