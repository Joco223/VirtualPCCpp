#include "CPU.h"

CPU::CPU(int cacheSize, Memory& ram_, GPU& gpu_)
	:
	registerOP1(0),
	registerOP2(0),
	register0(0),
	register1(0),
	programCounter(0),
	keyboardRegister(0),
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
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 2: { //Load register2 from cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = cache.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 3: { //Load register1 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = ram.memory[memPos];
		programCounter += 2;
		programCounter++;
		break; }

	case 4: { //Load register2 from ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
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
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case 8: { //Write from register1 to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case 9: { //Write from register0 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0;
		programCounter += 2;
		programCounter++;
		break; }

	case 10: { //Write from register1 to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register1;
		programCounter += 2;
		programCounter++;
		break; }

	case 11: { //Write from cache to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		u16 memPos2 = arg2p2 << 8 | (arg2p1 & 0xFF);
		ram.memory[memPos2] = cache.memory[memPos];
		programCounter++;
		break; }

	case 12: { //Write from ram to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg2p1 = cache.memory[programCounter + 3];
		byte arg2p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		u16 memPos2 = arg2p2 << 8 | (arg2p1 & 0xFF);
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
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		programCounter = memPos;
		break; }

	case 31: { //Compare to true/false
		byte conditionp1 = cache.memory[programCounter + 1];
		byte conditionp2 = cache.memory[programCounter + 2];
		u16 condition = conditionp2 << 8 | (conditionp1 & 0xFF);
		byte arg1p1 = cache.memory[programCounter + 3];
		byte arg1p2 = cache.memory[programCounter + 4];
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		if (register0 == condition) {
			programCounter = memPos;
		}else if(register0 != condition){
			programCounter += 4;
			programCounter++;
		}
		break; }

	case 32: { //Wait a number of cycles
		byte waitTimep1 = cache.memory[programCounter + 1];
		byte waitTimep2 = cache.memory[programCounter + 2];
		u16 waitTime = waitTimep2 << 8 | (waitTimep1 & 0xFF);
		if (currentTime < cache.memory[waitTime]) {
			currentTime++;
		}else if (currentTime >= cache.memory[waitTime]) {
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
		u16 memPos = arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = keyboardRegister;
		programCounter += 2;
		programCounter++;
		break; }

	case 50: { //Copy from RAM to VRAM
		byte arg1p1 = cache.memory[programCounter +  1];
		byte arg1p2 = cache.memory[programCounter +  2];
		byte arg2p1 = cache.memory[programCounter +  3];
		byte arg2p2 = cache.memory[programCounter +  4];
		byte arg3p1 = cache.memory[programCounter +  5];
		byte arg3p2 = cache.memory[programCounter +  6];
		byte arg4p1 = cache.memory[programCounter +  7];
		byte arg4p2 = cache.memory[programCounter +  8];
		byte arg5p1 = cache.memory[programCounter +  9];
		byte arg5p2 = cache.memory[programCounter + 10];
		byte arg6p1 = cache.memory[programCounter + 11];
		byte arg6p2 = cache.memory[programCounter + 12];

		u16 arg1 = arg1p2 << 8 | (arg1p1 & 0xFF);
		u16 arg2 = arg2p2 << 8 | (arg2p1 & 0xFF);
		u16 arg3 = arg3p2 << 8 | (arg3p1 & 0xFF);
		u16 arg4 = arg4p2 << 8 | (arg4p1 & 0xFF);
		u16 arg5 = arg5p2 << 8 | (arg5p1 & 0xFF);
		u16 arg6 = arg6p2 << 8 | (arg6p1 & 0xFF);

		gpu.commandBuffer.memory[gpu.commandCounter] = 1;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = cache.memory[arg1];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = cache.memory[arg2];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = cache.memory[arg3];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = cache.memory[arg4];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = cache.memory[arg5];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = cache.memory[arg6];

		programCounter += 12;
		programCounter++;
		break; }

	case 51: { //Copy from VRAM to RAM
		byte arg1p1 = cache.memory[programCounter +  1];
		byte arg1p2 = cache.memory[programCounter +  2];
		byte arg2p1 = cache.memory[programCounter +  3];
		byte arg2p2 = cache.memory[programCounter +  4];
		byte arg3p1 = cache.memory[programCounter +  5];
		byte arg3p2 = cache.memory[programCounter +  6];
		byte arg4p1 = cache.memory[programCounter +  7];
		byte arg4p2 = cache.memory[programCounter +  8];
		byte arg5p1 = cache.memory[programCounter +  9];
		byte arg5p2 = cache.memory[programCounter + 10];
		byte arg6p1 = cache.memory[programCounter + 11];
		byte arg6p2 = cache.memory[programCounter + 12];

		u16 arg1 = arg1p2 << 8 | (arg1p1 & 0xFF);
		u16 arg2 = arg2p2 << 8 | (arg2p1 & 0xFF);
		u16 arg3 = arg3p2 << 8 | (arg3p1 & 0xFF);
		u16 arg4 = arg4p2 << 8 | (arg4p1 & 0xFF);
		u16 arg5 = arg5p2 << 8 | (arg5p1 & 0xFF);
		u16 arg6 = arg6p2 << 8 | (arg6p1 & 0xFF);

		gpu.commandBuffer.memory[gpu.commandCounter] = 2;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = cache.memory[arg1];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = cache.memory[arg2];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = cache.memory[arg3];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = cache.memory[arg4];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = cache.memory[arg5];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = cache.memory[arg6];

		programCounter += 12;
		programCounter++;
		break; }

	case 52: { //Draw recrtangle
		byte arg1p1 = cache.memory[programCounter +    1];
		byte arg1p2 = cache.memory[programCounter +    2];
		byte arg2p1 = cache.memory[programCounter +    3];
		byte arg2p2 = cache.memory[programCounter +    4];
		byte arg3p1 = cache.memory[programCounter +    5];
		byte arg3p2 = cache.memory[programCounter +    6];
		byte arg4p1 = cache.memory[programCounter +    7];
		byte arg4p2 = cache.memory[programCounter +    8];
		byte arg5p1 = cache.memory[programCounter +    9];
		byte arg5p2 = cache.memory[programCounter +   10];
		byte arg6p1 = cache.memory[programCounter +   11];
		byte arg6p2 = cache.memory[programCounter +   12];
		byte arg7p1 = cache.memory[programCounter +   13];
		byte arg7p2 = cache.memory[programCounter +   14];
		byte arg8p1 = cache.memory[programCounter +   15];
		byte arg8p2 = cache.memory[programCounter +   16];
		byte arg9p1 = cache.memory[programCounter +   17];
		byte arg9p2 = cache.memory[programCounter +   18];
		byte arg10p1 = cache.memory[programCounter +  19];
		byte arg10p2 = cache.memory[programCounter +  20];
		byte arg11p1 = cache.memory[programCounter +  21];
		byte arg11p2 = cache.memory[programCounter +  22];

		u16 arg1  = arg1p2 << 8 | (arg1p1 & 0xFF);
		u16 arg2  = arg2p2 << 8 | (arg2p1 & 0xFF);
		u16 arg3  = arg3p2 << 8 | (arg3p1 & 0xFF);
		u16 arg4  = arg4p2 << 8 | (arg4p1 & 0xFF);
		u16 arg5  = arg5p2 << 8 | (arg5p1 & 0xFF);
		u16 arg6  = arg6p2 << 8 | (arg6p1 & 0xFF);
		u16 arg7  = arg7p2 << 8 | (arg7p1 & 0xFF);
		u16 arg8  = arg8p2 << 8 | (arg8p1 & 0xFF);
		u16 arg9  = arg9p2 << 8 | (arg9p1 & 0xFF);
		u16 arg10 = arg10p2 << 8 | (arg10p1 & 0xFF);
		u16 arg11 = arg11p2 << 8 | (arg11p1 & 0xFF);

		gpu.commandBuffer.memory[gpu.commandCounter] = 3;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  0] = cache.memory[arg1];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  1] = cache.memory[arg2];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  2] = cache.memory[arg3];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  3] = cache.memory[arg4];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  4] = cache.memory[arg5];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  5] = cache.memory[arg6];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  6] = cache.memory[arg7];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  7] = cache.memory[arg8];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  8] = cache.memory[arg9];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter +  9] = cache.memory[arg10];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 10] = cache.memory[arg11];

		programCounter += 22;
		programCounter++;
		break; }

	case 53: { //Draw a character
		byte characterPositionp1 = cache.memory[programCounter + 1];
		byte characterPositionp2 = cache.memory[programCounter + 2];
		byte xPosp1 = cache.memory[programCounter + 3];
		byte xPosp2 = cache.memory[programCounter + 4];
		byte yPosp1 = cache.memory[programCounter + 5];
		byte yPosp2 = cache.memory[programCounter + 6];
		byte rp1 = cache.memory[programCounter + 7];
		byte rp2 = cache.memory[programCounter + 8];
		byte gp1 = cache.memory[programCounter + 9];
		byte gp2 = cache.memory[programCounter + 10];
		byte bp1 = cache.memory[programCounter + 11];
		byte bp2 = cache.memory[programCounter + 12];

		u16 arg1 = characterPositionp2 << 8 | (characterPositionp1 & 0xFF);
		u16 arg2 = xPosp2 << 8 | (xPosp1 & 0xFF);
		u16 arg3 = yPosp2 << 8 | (yPosp1 & 0xFF);
		u16 arg4 = rp2 << 8 | (rp1 & 0xFF);
		u16 arg5 = gp2 << 8 | (gp1 & 0xFF);
		u16 arg6 = bp2 << 8 | (bp1 & 0xFF);

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = cache.memory[arg1];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = cache.memory[arg2];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = cache.memory[arg3];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = cache.memory[arg4];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = cache.memory[arg5];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = cache.memory[arg6];

		programCounter += 12;
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