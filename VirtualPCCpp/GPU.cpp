#include "GPU.h"

GPU::GPU()
	:
	commandBuffer(Memory(0)),
	commandArgBuffer(Memory(0)),
	vRam(Memory(0)),
	ram(nullptr),
	screen(nullptr),
	functionCounter(0),
	commandCounter(0),
	commandArgCounter(0),
	firstAvailableByte(0),
	coreCount(0) {}

GPU::GPU(u16 commandBufferSize, u16 vRamSize, u16 coreCount_, u16 commandArgBufferSize, SDLWindow* screen_, Memory* ram_)
	:
	commandBuffer(Memory(commandBufferSize)),
	commandArgBuffer(Memory(commandArgBufferSize)),
	vRam(Memory(vRamSize)),
	ram(ram_),
	screen(screen_),
	functionCounter(0),
	commandCounter(0),
	commandArgCounter(0),
	firstAvailableByte(0),
	coreCount(coreCount_) {}

void getArgument(u16& arg, byte& b1, byte& b2) {
	if (arg > 255) {
		int temp = (arg - 255) / 256;
		b1 = 255;
		b2 = temp;
	}else{
		b1 = arg;
		b2 = 0;
	}
}

void GPU::executeCommand() {
	byte instruction = commandBuffer.memory[commandCounter];

	switch (instruction) {

	case 0: {
		commandBuffer.Clear();
		commandArgBuffer.Clear();
		commandCounter = 0;
		break; }

	case 1: {
		byte arg1b1 = commandArgBuffer.memory[commandArgCounter + 0];
		byte arg1b2 = commandArgBuffer.memory[commandArgCounter + 1];
		byte arg2b1 = commandArgBuffer.memory[commandArgCounter + 2];
		byte arg2b2 = commandArgBuffer.memory[commandArgCounter + 3];
		byte arg3b1 = commandArgBuffer.memory[commandArgCounter + 4];
		byte arg3b2 = commandArgBuffer.memory[commandArgCounter + 5];

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);
		u16 arg3 = arg3b1 + (arg3b2 * 256);

		for (int i = 0; i < arg2 - arg1; i++) {
			byte arg1b1p, arg1b2p, arg2b1p, arg2b2p;
			u16 arg1p = arg1 + i;
			u16 arg2p = arg2 + i;
			getArgument(arg1p, arg1b1p, arg1b2p);
			getArgument(arg2p, arg2b1p, arg2b2p);

			vRam.memory[firstAvailableByte + 0] = 1;
			vRam.memory[firstAvailableByte + 1] = arg1b1p;
			vRam.memory[firstAvailableByte + 2] = arg1b2p;
			vRam.memory[firstAvailableByte + 3] = arg2b1p;
			vRam.memory[firstAvailableByte + 4] = arg2b2p;

			firstAvailableByte += 5;
		}

		commandArgCounter += 6;
		commandCounter++;
		break; }

	case 2: {
		byte arg1b1 = commandArgBuffer.memory[commandArgCounter + 0];
		byte arg1b2 = commandArgBuffer.memory[commandArgCounter + 1];
		byte arg2b1 = commandArgBuffer.memory[commandArgCounter + 2];
		byte arg2b2 = commandArgBuffer.memory[commandArgCounter + 3];
		byte arg3b1 = commandArgBuffer.memory[commandArgCounter + 4];
		byte arg3b2 = commandArgBuffer.memory[commandArgCounter + 5];

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);
		u16 arg3 = arg3b1 + (arg3b2 * 256);

		for (int i = 0; i < arg2 - arg1; i++) {
			byte arg1b1p, arg1b2p, arg2b1p, arg2b2p;
			u16 arg1p = arg1 + i;
			u16 arg2p = arg2 + i;
			getArgument(arg1p, arg1b1p, arg1b2p);
			getArgument(arg2p, arg2b1p, arg2b2p);

			vRam.memory[firstAvailableByte + 0] = 1;
			vRam.memory[firstAvailableByte + 1] = arg1b1p;
			vRam.memory[firstAvailableByte + 2] = arg1b2p;
			vRam.memory[firstAvailableByte + 3] = arg2b1p;
			vRam.memory[firstAvailableByte + 4] = arg2b2p;

			firstAvailableByte += 5;
		}

		commandArgCounter += 6;
		commandCounter++;
		break; }
	}
}

void GPU::executeThread() {
	byte instruction = vRam.memory[functionCounter];

	switch (instruction) {

	case 1: {
		byte arg1b1, arg1b2, arg2b1, arg2b2;
		u16 arg1 = vRam.memory[functionCounter + 0];
		u16 arg2 = vRam.memory[functionCounter + 1];
		getArgument(arg1, arg1b1, arg1b2);
		getArgument(arg2, arg2b1, arg2b2);

		ram->memory[arg1] = vRam.memory[arg2];
		break; }	
	case 2: {
		byte arg1b1, arg1b2, arg2b1, arg2b2;
		u16 arg1 = vRam.memory[functionCounter + 0];
		u16 arg2 = vRam.memory[functionCounter + 1];
		getArgument(arg1, arg1b1, arg1b2);
		getArgument(arg2, arg2b1, arg2b2);

		vRam.memory[arg1] = ram->memory[arg2];
		break; }	
	}
}