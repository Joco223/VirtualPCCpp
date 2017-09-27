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
	byte instruction = commandBuffer.read[commandCounter];

	switch (instruction) {

	case 0: {
		commandBuffer.Clear();
		commandArgBuffer.Clear();
		commandCounter = 0;
		break; }

	case 1: {
		byte arg1b1 = vRam.read(functionCounter + 0);
		byte arg1b2 = vRam.read(functionCounter + 1);
		byte arg2b1 = vRam.read(functionCounter + 2);
		byte arg2b2 = vRam.read(functionCounter + 3);
		byte arg3b1 = vRam.read(functionCounter + 4);
		byte arg3b2 = vRam.read(functionCounter + 5);

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);
		u16 arg3 = arg3b1 + (arg3b2 * 256);

		for (int i = 0; i < arg2 - arg1; i++) {
			byte arg1b1p, arg1b2p, arg2b1p, arg2b2p;
			u16 arg1p = arg1 + i;
			u16 arg2p = arg2 + i;
			getArgument(arg1p, arg1b1p, arg1b2p);
			getArgument(arg2p, arg2b1p, arg2b2p);

			vRam.write(firstAvailableByte + 0, 1);
			vRam.write(firstAvailableByte + 0, arg1b1p);
			vRam.write(firstAvailableByte + 0, arg1b2p);
			vRam.write(firstAvailableByte + 0, arg2b1p);
			vRam.write(firstAvailableByte + 0, arg2b2p);

			firstAvailableByte += 5;
		}

		commandArgCounter += 6;
		commandCounter++;
		break; }

	case 2: {
		byte arg1b1 = vRam.read(functionCounter + 0);
		byte arg1b2 = vRam.read(functionCounter + 1);
		byte arg2b1 = vRam.read(functionCounter + 2);
		byte arg2b2 = vRam.read(functionCounter + 3);
		byte arg3b1 = vRam.read(functionCounter + 4);
		byte arg3b2 = vRam.read(functionCounter + 5);

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);
		u16 arg3 = arg3b1 + (arg3b2 * 256);

		for (int i = 0; i < arg2 - arg1; i++) {
			byte arg1b1p, arg1b2p, arg2b1p, arg2b2p;
			u16 arg1p = arg1 + i;
			u16 arg2p = arg2 + i;
			getArgument(arg1p, arg1b1p, arg1b2p);
			getArgument(arg2p, arg2b1p, arg2b2p);

			vRam.write(firstAvailableByte + 0, 2);
			vRam.write(firstAvailableByte + 0, arg1b1p);
			vRam.write(firstAvailableByte + 0, arg1b2p);
			vRam.write(firstAvailableByte + 0, arg2b1p);
			vRam.write(firstAvailableByte + 0, arg2b2p);

			firstAvailableByte += 5;
		}

		commandArgCounter += 6;
		commandCounter++;
		break; }

	case 3: {
		byte arg1b1 = vRam.read(functionCounter + 0);
		byte arg1b2 = vRam.read(functionCounter + 1);
		byte arg2b1 = vRam.read(functionCounter + 2);
		byte arg2b2 = vRam.read(functionCounter + 3);
		byte arg3b1 = vRam.read(functionCounter + 4);
		byte arg3b2 = vRam.read(functionCounter + 5);
		byte arg4b1 = vRam.read(functionCounter + 6);
		byte arg4b2 = vRam.read(functionCounter + 7);
		byte arg5 = vRam.read(functionCounter + 8);
		byte arg6 = vRam.read(functionCounter + 9);
		byte arg7 = vRam.read(functionCounter + 10);

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);
		u16 arg3 = arg3b1 + (arg3b2 * 256);
		u16 arg4 = arg4b1 + (arg4b2 * 256);

		u16 xSize = arg3 - arg1;
		u16 ySize = arg4 - arg2;

		u16 cXPos, cYPos;

		for (int y = 0; y < ySize; y++) {
			for (int x = 0; x < xSize; x++) {
				cYPos = arg2 + y;
				cXPos = arg1 + x;

				byte x1, x2, y1, y2;
				getArgument(cXPos, x1 ,x2);
				getArgument(cYPos, y1, y2);

				vRam.write(firstAvailableByte + 0, 3);
				vRam.write(firstAvailableByte + 1, x1);
				vRam.write(firstAvailableByte + 2, x2);
				vRam.write(firstAvailableByte + 3, y1);
				vRam.write(firstAvailableByte + 4, y2);
				vRam.write(firstAvailableByte + 5, arg5);
				vRam.write(firstAvailableByte + 6, arg6);
				vRam.write(firstAvailableByte + 7, arg7);

				firstAvailableByte += 8;
			}
		}

		commandArgCounter += 11;
		commandCounter++;
		break; }
	}
}

void GPU::executeThread() {
	byte instruction = vRam.read(functionCounter);

	switch (instruction) {

	case 1: {
		//TODO
		break; }	
	case 2: {
		//TODO
		break; }	

	case 3: {
		byte arg1b1 = vRam.read(functionCounter + 1);
		byte arg1b2 = vRam.read(functionCounter + 2);
		byte arg2b1 = vRam.read(functionCounter + 3);
		byte arg2b2 = vRam.read(functionCounter + 4);
		byte arg3 = vRam.read(functionCounter + 5);
		byte arg4 = vRam.read(functionCounter + 6);
		byte arg5 = vRam.read(functionCounter + 7);

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);

		byte r = arg3 * 4;
		byte g = arg4 * 4;
		byte b = arg5 * 4;

		this->screen->pixels[arg2 * 320 + arg1] = int(b << 16) | int(g << 8) | int(r);
		functionCounter += 8;
		break; }
	}
}

void GPU::tick() {
	for (int i = 0; i < coreCount; i++) {
		executeThread();
	}
}

void GPU::accessCommandBuffer(std::string choice, u16 pos, byte value = 0, byte& output) {
	if (choice == "write") {
		commandBuffer.write(pos, value);
	}else if (choice == "read") {
		output = commandBuffer.read(pos);
	}
}

void GPU::accessCommandArgBuffer(std::string choice, u16 pos, byte value = 0, byte& output) {
	if (choice == "write") {
		commandArgBuffer.write(pos, value);
	}else if (choice == "read") {
		output = commandArgBuffer.read(pos);
	}
}

void GPU::accessVRam(std::string choice, u16 pos, byte value = 0, byte& output) {
	if (choice == "write") {
		vRam.write(pos, value);
	}else if (choice == "read") {
		output = vRam.read(pos);
	}
}