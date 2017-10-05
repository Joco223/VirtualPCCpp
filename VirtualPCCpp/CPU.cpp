#include "CPU.h"

CPU::CPU(int cacheSize, Memory& ram_, GPU& gpu_)
	:
	registerOP1(0),
	registerOP2(0),
	register0(0),
	register1(0),
	programCounter(0),
	keyboardRegister(97),
	inputCount(0),
	firstAvailable(0),
	currentTime(0),
	ram(ram_),
	halt(false),
	cache(Memory(cacheSize)),
	gpu(gpu_){
}

void CPU::execute(u16 register) {
	u16 opCode = registerOP1;

	switch (opCode) {
	case 0: { //Halt
		halt = true;
		break; }

	case 1: { //Load register1 from cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register0 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 2: { //Load register2 from cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 3: { //Load register1 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register0 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 4: { //Load register2 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 5: { //clear register1
		register0 = 0;
		programCounter++;
		break; }

	case 6: { //Clear register2
		register1 = 0;
		programCounter++;
		break; }

	case 7: { //Write from register0 to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case 8: { //Write from register1 to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case 9: { //Write from register0 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		ram.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case 10: { //Write from register1 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		ram.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case 11: { //Write from cache to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p1 + (arg2p2 * 256);
		ram.memory[memPos2] = cache.memory[memPos];
		programCounter++;
		break; }

	case 12: { //Write from ram to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p2 + (arg2p2 * 256);
		cache.memory[memPos2] = ram.memory[memPos];
		programCounter++;
		break; }

	case 20: { //Sum register1 and register2
		register0 = register0 + register1;
		programCounter++;
		break; }

	case 21: { //Subtract register1 and register2
		register0 = register0 - register1;
		programCounter++;
		break; }

	case 22: { //Multiply register1 and register2
		register0 = register0 * register1;
		programCounter++;
		break; }

	case 23: { //Divide register1 and register2
		register0 = register0 / register1;
		programCounter++;
		break; }

	case 24: { //Check if register1 is bigger than register2
		register0 = register0 > register1;
		programCounter++;
		break; }

	case 25: { //Check if register1 is bigger or equal to register2
		register0 = register0 >= register1;
		programCounter++;
		break; }

	case 26: { //Check if register2 is bigger than register1
		register0 = register0 < register1;
		programCounter++;
		break; }

	case 27: { //Check if register2 is bigger or equal to register1
		register0 = register0 <= register1;
		programCounter++;
		break; }

	case 28: { //Check if register1 and register2 have same values
		register0 = register0 == register1;
		programCounter++;
		break; }

	case 29: { //Check if register1 is different than register2
		register0 = register0 != register1;
		programCounter++;
		break; }

	case 30: { //Jump to place in code
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		programCounter = memPos;
		break; }

	case 31: { //Compare to true/false
		byte condition = cache.memory[programCounter + 1];
		byte arg1p1 = cache.memory[programCounter + 2];
		byte arg1p2 = cache.memory[programCounter + 3];
		u16 memPos = arg1p1 + (arg1p2 * 256) - 1;
		if (register0 == condition) {
			programCounter = memPos;
		}else if(register0 != condition){	
			programCounter += 3;
		}
		programCounter++;
		break; }

	case 32: { //Wait a number of cycles
		u16 waitTime = cache.memory[programCounter + 1];
		if (currentTime < waitTime) {
			currentTime++;
		}else if (currentTime >= waitTime) {
			currentTime = 0;
			programCounter += 2;
			programCounter++;
		}
		break; }

	case 40: { //Cout from register1
		std::cout << register0 << '\n';
		programCounter++;
		break; }

	case 41: { //Cout from register2
		std::cout << register1 << '\n';
		programCounter++;
		break; }

	case 42: { //Write from keyboard input register to place in cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p1 + (arg1p2 * 256);
		cache.memory[memPos] = keyboardRegister;
		programCounter += 2;
		programCounter++;
		break; }

	case 50: { //Copy from RAM to VRAM
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = arg3p2;

		programCounter += 6;
		programCounter++;
		break; }

	case 51: { //Copy from VRAM to RAM
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		byte arg3p1 = cache.memory[programCounter + 5];
		byte arg3p2 = cache.memory[programCounter + 6];

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg1p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg2p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = arg2p2;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = arg3p1;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = arg3p2;

		programCounter += 6;
		programCounter++;
		break; }

	case 52: { //Draw recrtangle
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

	case 53: { //Draw a character
		byte characterPositionp1 = cache.memory[programCounter + 1];
		byte characterPositionp2 = cache.memory[programCounter + 2];
		u16 characterPosition = characterPositionp1 + (characterPositionp2 * 256);
		byte characterCode = cache.memory[characterPosition];
		byte xPos = cache.memory[programCounter + 3];
		byte yPos = cache.memory[programCounter + 4];
		byte r = cache.memory[programCounter + 5];
		byte g = cache.memory[programCounter + 6];
		byte b = cache.memory[programCounter + 7];

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = characterCode;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = xPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = yPos;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = r;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = g;
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = b;

		programCounter += 7;
		programCounter++;
		break; }

	case 55: { //Clear GPU commandBuffer
		gpu.commandBuffer.memory[gpu.commandCounter] = 5;
		programCounter++;
		break; }

	case 56: { //Clear GPU VRAM
		if (gpu.functionCounter == gpu.firstAvailableByte) {
			gpu.commandBuffer.memory[gpu.commandCounter] = 6;
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