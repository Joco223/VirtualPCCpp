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
	u16 memPos;
	u16 memPos2;
	u16 programPos;
	u16 condition;

	switch (opCode) {
	case 0: //Halt
		halt = true;
		break;

	case 1: //Load register1 from cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register1 = cache.memory[memPos];
		programCounter++;
		programCounter++;
		break;

	case 2: //Load register2 from cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register2 = cache.memory[memPos];
		programCounter++;
		programCounter++;
		break;

	case 3: //Load register1 from ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register1 = cache.memory[memPos];
		programCounter++;
		programCounter++;
		break;

	case 4: //Load register2 from ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		register2 = cache.memory[memPos];
		programCounter++;
		programCounter++;
		break;

	case 5: //clear register1
		register1 = 0;
		break;

	case 6: //Clear register2
		register2 = 0;
		break;

	case 7: //Write from register1 to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		cache.memory[memPos] = register1;
		programCounter++;
		programCounter++;
		break;

	case 8: //Write from register2 to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		cache.memory[memPos] = register2;
		programCounter++;
		programCounter++;
		break;

	case 9: //Write from register1 to ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		ram.memory[memPos] = register1;
		programCounter++;
		programCounter++;
		break;

	case 10: //Write from register2 to ram
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		ram.memory[memPos] = register2;
		programCounter++;
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
		break;

	case 12: //Write from ram to cache
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256);
		memPos2 = opCodeArg2p1 + (opCodeArg2p2 * 256);
		cache.memory[memPos2] = ram.memory[memPos];
		break;

	case 20: //Sum register1 and register2
		register1 = register1 + register2;
		break;

	case 21: //Subtract register1 and register2
		register1 = register1 - register2;
		break;

	case 22: //Multiply register1 and register2
		register1 = register1 * register2;
		break;

	case 23: //Divide register1 and register2
		register1 = register1 / register2;
		break;

	case 24: //Check if register1 is bigger than register2
		register1 = register1 > register2;
		break;

	case 25: //Check if register1 is bigger or equal to register2
		register1 = register1 >= register2;
		break;

	case 26: //Check if register2 is bigger than register1
		register1 = register1 < register2;
		break;

	case 27: //Check if register2 is bigger or equal to register1
		register1 = register1 <= register2;
		break;

	case 28: //Checl if register1 and register2 have same values
		register1 = register1 == register2;
		break;

	case 29: //Check if register1 is different than register2
		register1 = register1 != register2;
		break;

	case 30: //Jump to place in code
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		memPos = opCodeArg1p1 + (opCodeArg1p2 * 256) - 1;	
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
		break;

	case 40: //Cout from register1
		std::cout << register1 << '\n';
		break;

	case 41: //Cout from register2
		std::cout << register2 << '\n';
		break;

	case 50:
		opCodeArg1p1 = cache.memory[programCounter + 1];
		opCodeArg1p2 = cache.memory[programCounter + 2];
		opCodeArg2p1 = cache.memory[programCounter + 3];
		opCodeArg2p2 = cache.memory[programCounter + 4];
		opCodeArg3p1 = cache.memory[programCounter + 5];
		opCodeArg3p2 = cache.memory[programCounter + 6];
		opCodeArg4p1 = cache.memory[programCounter + 7];
		opCodeArg4p2 = cache.memory[programCounter + 8];
		programPos = opCodeArg4p1 + (opCodeArg4p2 * 256);
		gpu.commandBuffer.memory[programPos] = 1;
		gpu.argBuffer.memory[programPos * 6 + 0] = opCodeArg1p1;
		gpu.argBuffer.memory[programPos * 6 + 1] = opCodeArg1p2;
		gpu.argBuffer.memory[programPos * 6 + 2] = opCodeArg2p1;
		gpu.argBuffer.memory[programPos * 6 + 3] = opCodeArg2p2;
		gpu.argBuffer.memory[programPos * 6 + 4] = opCodeArg3p1;
		gpu.argBuffer.memory[programPos * 6 + 5] = opCodeArg3p2;
		programCounter += 8;
		break;

	case 51:
		
		break;
	}
}

void CPU::tick() {
	if (halt == false) {
		registerOP1 = cache.memory[programCounter];
		execute(register1);
		programCounter++;
	}
}