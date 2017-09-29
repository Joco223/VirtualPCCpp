#include "GPU.h"

#include <iostream>
#include <fstream>
#include <string>

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

GPU::GPU(int commandBufferSize, int vRamSize, int coreCount_, int commandArgBufferSize, SDLWindow* screen_, Memory* ram_)
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
	coreCount(coreCount_) 
{
	int countR = 0;
	int characterCount = 0;
	characters.resize(68);
	
	std::string line;
	std::ifstream myfile ("C:/Users/jovanipavle/Desktop/Characters 7x7/fontBasic.txt");

	for (int i = 0; i < 68; i++) {
		characters[i].rows.resize(8);
	}
	
	if (myfile.is_open()){
		while (getline (myfile,line)) {
			if (countR < 8) {
				characters[characterCount].rows[countR] = line;
				countR++;
			}else{
				characterCount++;
				countR = 0;
			}
		}
	}
}

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

	case 3: {
		byte arg1b1 = commandArgBuffer.memory[commandArgCounter + 0];
		byte arg1b2 = commandArgBuffer.memory[commandArgCounter + 1];
		byte arg2b1 = commandArgBuffer.memory[commandArgCounter + 2];
		byte arg2b2 = commandArgBuffer.memory[commandArgCounter + 3];
		byte arg3b1 = commandArgBuffer.memory[commandArgCounter + 4];
		byte arg3b2 = commandArgBuffer.memory[commandArgCounter + 5];
		byte arg4b1 = commandArgBuffer.memory[commandArgCounter + 6];
		byte arg4b2 = commandArgBuffer.memory[commandArgCounter + 7];
		byte arg5 = commandArgBuffer.memory[commandArgCounter + 8];
		byte arg6 = commandArgBuffer.memory[commandArgCounter + 9];
		byte arg7 = commandArgBuffer.memory[commandArgCounter + 10];

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
				getArgument(cXPos, x1, x2);
				getArgument(cYPos, y1, y2);

				vRam.memory[firstAvailableByte + 0] = 3;
				vRam.memory[firstAvailableByte + 1] = x1;
				vRam.memory[firstAvailableByte + 2] = x2;
				vRam.memory[firstAvailableByte + 3] = y1;
				vRam.memory[firstAvailableByte + 4] = y2;
				vRam.memory[firstAvailableByte + 5] = arg5;
				vRam.memory[firstAvailableByte + 6] = arg6;
				vRam.memory[firstAvailableByte + 7] = arg7;

				firstAvailableByte += 8;
			}
		}

		commandArgCounter += 11;
		commandCounter++;
		break; }

	case 4: {
		byte size = commandArgBuffer.memory[commandArgCounter + 0];
		byte xPos = commandArgBuffer.memory[commandArgCounter + 1];
		byte yPos = commandArgBuffer.memory[commandArgCounter + 2];
		byte r = commandArgBuffer.memory[commandArgCounter + 3];
		byte g = commandArgBuffer.memory[commandArgCounter + 4];
		byte b = commandArgBuffer.memory[commandArgCounter + 5];

		std::vector<byte> chars;

		for (int i = 0; i < size; i++) {
			chars.push_back(commandArgBuffer.memory[commandArgCounter + 6 + i]);
		}

		for (int i = 0; i < chars.size(); i++) {
			character currentC = characters[chars[i]];
			std::vector<u16> xPositions;
			std::vector<u16> yPositions;
			int cSize = 0;

			for (int j = 0; j < currentC.rows.size(); j++){
				for (int k = 0; k < 6; k++) {
					if (currentC.rows[j][k] == '1') {
						xPositions.push_back((xPos + i) * 6 + k);
						yPositions.push_back(yPos * 8 + j);
						cSize++;
					}
				}
			}

			for (int j = 0; j < cSize; j++) {
				byte x1, x2, y1, y2;
				u16 xPosition = xPositions[j];
				getArgument(xPosition, x1, x2);
				getArgument(yPositions[j], y1, y2);

				vRam.memory[firstAvailableByte + 0] = 3;
				vRam.memory[firstAvailableByte + 1] = x1;
				vRam.memory[firstAvailableByte + 2] = x2;
				vRam.memory[firstAvailableByte + 3] = y1;
				vRam.memory[firstAvailableByte + 4] = y2;
				vRam.memory[firstAvailableByte + 5] = b;
				vRam.memory[firstAvailableByte + 6] = g;
				vRam.memory[firstAvailableByte + 7] = r;

				firstAvailableByte += 8;
			}
		}

		commandArgCounter += 3 + size - 1;
		commandCounter++;
		break; }
	}
}

void GPU::executeThread() {
	byte instruction = vRam.memory[functionCounter];

	switch (instruction) {

	case 1: {
		//TODO
		break; }	
	case 2: {
		//TODO
		break; }	

	case 3: {
		byte arg1b1 = vRam.memory[functionCounter + 1];
		byte arg1b2 = vRam.memory[functionCounter + 2];
		byte arg2b1 = vRam.memory[functionCounter + 3];
		byte arg2b2 = vRam.memory[functionCounter + 4];
		byte arg3 = vRam.memory[functionCounter + 5];
		byte arg4 = vRam.memory[functionCounter + 6];
		byte arg5 = vRam.memory[functionCounter + 7];

		u16 arg1 = arg1b1 + (arg1b2 * 256);
		u16 arg2 = arg2b1 + (arg2b2 * 256);	

		byte r = arg3 * 4;
		byte g = arg4 * 4;
		byte b = arg5 * 4;

		screen->pixels[arg2 * 320 + arg1] = int(b << 16) | int(g << 8) | int(r);
		functionCounter += 8;
		break; }	
	}
}

void GPU::tick() {
	for (int i = 0; i < coreCount; i++) {
		executeThread();
	}
}