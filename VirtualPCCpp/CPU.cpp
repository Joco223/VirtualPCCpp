#include "CPU.h"

#include <iostream>

CPU::CPU()
	:
	registerOP1(0),
	registerOP2(0),
	register1(0),
	register2(0),
	ram(Memory(0)),
	programCounter(0),
	halt(false),
	gpu(GPU()) {
	cache = Memory(128);
}

CPU::CPU(int cacheSize, Memory& ram_, GPU& gpu_)
	:
	registerOP1(0),
	registerOP2(0),
	register1(0),
	register2(0),
	programCounter(0),
	ram(ram_),
	halt(false),
	cache(Memory(cacheSize)),
	gpu(gpu_){
}

void CPU::execute(u16 register) {
	u16 opCode = registerOP1;
	byte opCodeArg1p1;
	byte opCodeArg1p2;
	byte opCodeArg2p1;
	byte opCodeArg2p2;
	byte opCodeArg3p1;
	byte opCodeArg3p2;
	byte opCodeArg4p1;
	byte opCodeArg4p2;
	byte opCodeArg5p1;
	byte opCodeArg5p2;
	byte opCodeArg6p1;
	byte opCodeArg6p2;
	u16 memPos;
	u16 memPos2;
	u16 programPos;
	u16 condition;

	//std::cout << opCode << std::endl;

	switch (opCode) {
	case 0: //Halt
		halt = true;
		break;

	case 1: //Load register1 from cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register1 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break;

	case 2: //Load register2 from cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register2 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break;

	case 3: //Load register1 from ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register1 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break;

	case 4: //Load register2 from ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register2 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break;

	case 5: //clear register1
		register1 = 0;
		programCounter++;
		break;

	case 6: //Clear register2
		register2 = 0;
		programCounter++;
		break;

	case 7: //Write from register1 to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		cache.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break;

	case 8: //Write from register2 to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		cache.memory[memPos] = register2;
		programCounter += 2;
		programCounter++;
		break;

	case 9: //Write from register1 to ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		ram.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break;

	case 10: //Write from register2 to ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		ram.memory[memPos] = register2;
		programCounter += 2;
		programCounter++;
		break;

	case 11: //Write from cache to ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		memPos2 = opCodeArg2p1 + (opCodeArg2p2 * 256);
		ram.memory[memPos2] = cache.memory[memPos];
		programCounter++;
		break;

	case 12: //Write from ram to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		memPos2 = opCodeArg2p1 + (opCodeArg2p2 * 256);
		cache.memory[memPos2] = ram.memory[memPos];
		programCounter++;
		break;

	case 20: //Sum register1 and register2
		register1 = register1 + register2;
		programCounter++;
		break;

	case 21: //Subtract register1 and register2
		register1 = register1 - register2;
		programCounter++;
		break;

	case 22: //Multiply register1 and register2
		register1 = register1 * register2;
		programCounter++;
		break;

	case 23: //Divide register1 and register2
		register1 = register1 / register2;
		programCounter++;
		break;

	case 24: //Check if register1 is bigger than register2
		register1 = register1 > register2;
		programCounter++;
		break;

	case 25: //Check if register1 is bigger or equal to register2
		register1 = register1 >= register2;
		programCounter++;
		break;

	case 26: //Check if register2 is bigger than register1
		register1 = register1 < register2;
		programCounter++;
		break;

	case 27: //Check if register2 is bigger or equal to register1
		register1 = register1 <= register2;
		programCounter++;
		break;

	case 28: //Checl if register1 and register2 have same values
		register1 = register1 == register2;
		programCounter++;
		break;

	case 29: //Check if register1 is different than register2
		register1 = register1 != register2;
		programCounter++;
		break;

	case 30: //Jump to place in code
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		programCounter = memPos;
		break;

	case 31: //Compare to true/false
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg2p1 = cache.memory[programCounter + 2];
		opCodeArg2p2 = cache.memory[programCounter + 3];
		condition = opCodeArg1p1;
		memPos = opCodeArg2p1 + (opCodeArg2p2 * 256) - 1;
		if (register1 == condition) {
			programCounter = memPos;
		}else if(register1 != condition){	
			programCounter += 3;
		}
		programCounter++;
		break;

	case 32: {
		u16 waitTime = cache.memory[programCounter + 1];
		u16 currentTime = 0;
		if (currentTime < waitTime) {
			std::cout << currentTime << std::endl;
			currentTime++;
		}else if (currentTime >= waitTime) {
			programCounter++;
		}
	}

	case 40: //Cout from register1
		std::cout << register1 << '\n';
		programCounter++;
		break;

	case 41: //Cout from register2
		std::cout << register2 << '\n';
		programCounter++;
		break;

	case 50: //Copy from RAM to VRAM
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		opCodeArg3p1 = cache.memory[programCounter + 5];
		opCodeArg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = opCodeArg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = opCodeArg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = opCodeArg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = opCodeArg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = opCodeArg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = opCodeArg3p2;

		programCounter += 6;
		programCounter++;
		break;

	case 51: //Copy from VRAM to RAM
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		opCodeArg3p1 = cache.memory[programCounter + 5];
		opCodeArg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = opCodeArg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = opCodeArg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = opCodeArg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = opCodeArg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = opCodeArg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = opCodeArg3p2;

		programCounter += 6;
		programCounter++;
		break;

	case 52: {
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];
		byte arg4p1 = cache.memory[programCounter + 7];
		byte arg4p2 = cache.memory[programCounter + 8];
		byte arg5 = cache.memory[programCounter +  9];
		byte arg6 = cache.memory[programCounter + 10];
		byte arg7 = cache.memory[programCounter + 11];

		gpu.commandBuffer.memory[gpu.commandCounter] = 3;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  5] = arg3p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  6] = arg4p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  7] = arg4p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  8] = arg5;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  9] = arg6;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 10] = arg7;

		programCounter += 11;
		programCounter++;
		break; }

	case 53: {
		byte size = cache.memory[programCounter + 1];
		byte xPos = cache.memory[programCounter + 2];
		byte yPos = cache.memory[programCounter + 3];
		byte r = cache.memory[programCounter + 4];
		byte g = cache.memory[programCounter + 5];
		byte b = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = size;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = xPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = yPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = r;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = g;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = b;

		for (int i = 0; i < size; i++) {
			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 6 + i] = cache.memory[programCounter + 7 + i];
		}

		programCounter += (6 + size);
		programCounter++;
		break; }

	case 54: {
		gpu.commandBuffer.memory[gpu.commandCounter] = 5;
		programCounter++;
		break; }

	case 55: {
		if (gpu.functionCounter == gpu.firstAvailableByte) {
			gpu.commandBuffer.memory[gpu.commandCounter] = 5;
			programCounter++;
		}
		break; }
	}
}

void CPU::tick() {
	if (halt == false) {
		registerOP1 = cache.memory[programCounter];
		execute(register1);
		
	}
}