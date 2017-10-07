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

	case 1: { //Load register1 from cache, 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = cache.memory[memPos + 2] << 16 | cache.memory[memPos + 1] << 8 | (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 2: { //Load register2 from cache, 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = cache.memory[memPos + 2] << 16 | cache.memory[memPos + 1] << 8 | (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 3: { //Load register1 from ram, 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = ram.memory[memPos + 2] << 16 | ram.memory[memPos + 1] << 8 | (ram.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 4: { //Load register2 from ram, 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = ram.memory[memPos + 2] << 16 | ram.memory[memPos + 1] << 8 | (ram.memory[memPos] & 0xFF);
		programCounter += 3;
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

	case 7: { //Write from register0 to cache 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		cache.memory[memPos + 1] = (byte)(register0 >> 8);
		cache.memory[memPos + 2] = (byte)(register0 >> 16);
		programCounter += 3;
		programCounter++;
		break; }

	case 8: { //Write from register1 to cache 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		cache.memory[memPos + 1] = (byte)(register0 >> 8);
		cache.memory[memPos + 2] = (byte)(register0 >> 16);
		programCounter += 3;
		programCounter++;
		break; }

	case 9: { //Write from register0 to ram 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		ram.memory[memPos + 1] = (byte)(register0 >> 8);
		ram.memory[memPos + 2] = (byte)(register0 >> 16);
		programCounter += 3;
		programCounter++;
		break; }

	case 10: { //Write from register1 to ram 3 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		ram.memory[memPos + 1] = (byte)(register0 >> 8);
		ram.memory[memPos + 2] = (byte)(register0 >> 16);
		programCounter += 3;
		programCounter++;
		break; }

	case 11: { //Write from cache to ram
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		byte arg2p1 = cache.memory[programCounter + 4];
		byte arg2p2 = cache.memory[programCounter + 5];
		byte arg2p3 = cache.memory[programCounter + 6];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		int memPos2 = arg2p3 << 16 | arg2p2 << 8 | (arg2p1 & 0xFF);
		ram.memory[memPos2] = cache.memory[memPos];
		programCounter += 6;
		programCounter++;
		break; }

	case 12: { //Write from ram to cache
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		byte arg2p1 = cache.memory[programCounter + 4];
		byte arg2p2 = cache.memory[programCounter + 5];
		byte arg2p3 = cache.memory[programCounter + 6];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		int memPos2 = arg2p3 << 16 | arg2p2 << 8 | (arg2p1 & 0xFF);
		cache.memory[memPos2] = ram.memory[memPos];
		programCounter += 6;
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
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		programCounter = memPos;
		break; }

	case 31: { //Compare to true/false
		byte condition = cache.memory[programCounter + 1];
		byte arg1p1 = cache.memory[programCounter + 2];
		byte arg1p2 = cache.memory[programCounter + 3];
		byte arg1p3 = cache.memory[programCounter + 4];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
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
		byte waitTimep3 = cache.memory[programCounter + 3];
		int waitTime = waitTimep3 << 16 | waitTimep2 << 8 | (waitTimep1 & 0xFF);
		if (currentTime < cache.memory[waitTime]) {
			currentTime++;
		}else if (currentTime >= cache.memory[waitTime]) {
			currentTime = 0;
			programCounter += 3;
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
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = keyboardRegister;
		programCounter += 3;
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

	case 52: { //Draw rectangle
		byte arg1p1 = cache.memory[programCounter +    1];
		byte arg1p2 = cache.memory[programCounter +    2];
		byte arg1p3 = cache.memory[programCounter +    3];
		byte arg2p1 = cache.memory[programCounter +    4];
		byte arg2p2 = cache.memory[programCounter +    5];
		byte arg2p3 = cache.memory[programCounter +    6];
		byte arg3p1 = cache.memory[programCounter +    7];
		byte arg3p2 = cache.memory[programCounter +    8];
		byte arg3p3 = cache.memory[programCounter +    9];
		byte arg4p1 = cache.memory[programCounter +   10];
		byte arg4p2 = cache.memory[programCounter +   11];
		byte arg4p3 = cache.memory[programCounter +   12];
		byte arg5p1 = cache.memory[programCounter +   13];
		byte arg5p2 = cache.memory[programCounter +   14];
		byte arg5p3 = cache.memory[programCounter +   15];
		byte arg6p1 = cache.memory[programCounter +   16];
		byte arg6p2 = cache.memory[programCounter +   17];
		byte arg6p3 = cache.memory[programCounter +   18];
		byte arg7p1 = cache.memory[programCounter +   19];
		byte arg7p2 = cache.memory[programCounter +   20];
		byte arg7p3 = cache.memory[programCounter +   21];
		byte arg8p1 = cache.memory[programCounter +   22];
		byte arg8p2 = cache.memory[programCounter +   23];
		byte arg8p3 = cache.memory[programCounter +   24];
		byte arg9p1 = cache.memory[programCounter +   25];
		byte arg9p2 = cache.memory[programCounter +   26];
		byte arg9p3 = cache.memory[programCounter +   27];
		byte arg10p1 = cache.memory[programCounter +  28];
		byte arg10p2 = cache.memory[programCounter +  29];
		byte arg10p3 = cache.memory[programCounter +  30];
		byte arg11p1 = cache.memory[programCounter +  31];
		byte arg11p2 = cache.memory[programCounter +  32];
		byte arg11p3 = cache.memory[programCounter +  33];

		int arg1  = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		int arg2  = arg2p3 << 16 | arg2p2 << 8 | (arg2p1 & 0xFF);
		int arg3  = arg3p3 << 16 | arg3p2 << 8 | (arg3p1 & 0xFF);
		int arg4  = arg4p3 << 16 | arg4p2 << 8 | (arg4p1 & 0xFF);
		int arg5  = arg5p3 << 16 | arg5p2 << 8 | (arg5p1 & 0xFF);
		int arg6  = arg6p3 << 16 | arg6p2 << 8 | (arg6p1 & 0xFF);
		int arg7  = arg7p3 << 16 | arg7p2 << 8 | (arg7p1 & 0xFF);
		int arg8  = arg8p3 << 16 | arg8p2 << 8 | (arg8p1 & 0xFF);
		int arg9  = arg9p3 << 16 | arg9p2 << 8 | (arg9p1 & 0xFF);
		int arg10 = arg10p3 << 16 | arg10p2 << 8 | (arg10p1 & 0xFF);
		int arg11 = arg11p3 << 16 | arg11p2 << 8 | (arg11p1 & 0xFF);

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

		programCounter += 33;
		programCounter++;
		break; }

	case 53: { //Draw a character
		byte characterPositionp1 = cache.memory[programCounter + 1];
		byte characterPositionp2 = cache.memory[programCounter + 2];
		byte characterPositionp3 = cache.memory[programCounter + 3];
		byte xPosp1 = cache.memory[programCounter + 4];
		byte xPosp2 = cache.memory[programCounter + 5];
		byte xPosp3 = cache.memory[programCounter + 6];
		byte yPosp1 = cache.memory[programCounter + 7];
		byte yPosp2 = cache.memory[programCounter + 8];
		byte yPosp3 = cache.memory[programCounter + 9];
		byte rp1 = cache.memory[programCounter + 10];
		byte rp2 = cache.memory[programCounter + 11];
		byte rp3 = cache.memory[programCounter + 12];
		byte gp1 = cache.memory[programCounter + 13];
		byte gp2 = cache.memory[programCounter + 14];
		byte gp3 = cache.memory[programCounter + 15];
		byte bp1 = cache.memory[programCounter + 16];
		byte bp2 = cache.memory[programCounter + 17];
		byte bp3 = cache.memory[programCounter + 18];

		int arg1 = characterPositionp3 << 16 | characterPositionp2 << 8 | (characterPositionp1 & 0xFF);
		int arg2 = xPosp3 << 16 | xPosp2 << 8 | (xPosp1 & 0xFF);
		int arg3 = yPosp3 << 16 | yPosp2 << 8 | (yPosp1 & 0xFF);
		int arg4 = rp3 << 16 | rp2 << 8 | (rp1 & 0xFF);
		int arg5 = gp3 << 16 | gp2 << 8 | (gp1 & 0xFF);
		int arg6 = bp3 << 16 | bp2 << 8 | (bp1 & 0xFF);

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = cache.memory[arg1];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = cache.memory[arg2];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = cache.memory[arg3];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = cache.memory[arg4];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = cache.memory[arg5];
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = cache.memory[arg6];

		programCounter += 18;
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

	case 80: { //Load register1 from cache, 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = cache.memory[memPos + 1] << 8 | (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 81: { //Load register2 from cache, 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = cache.memory[memPos + 1] << 8 | (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 82: { //Load register1 from ram, 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = ram.memory[memPos + 1] << 8 | (ram.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 83: { //Load register2 from ram, 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = ram.memory[memPos + 1] << 8 | (ram.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }
	
	case 84: { //Load register1 from cache, 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 85: { //Load register2 from cache, 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = (cache.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 86: { //Load register1 from ram, 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register0 = (ram.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 87: { //Load register2 from ram, 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		register1 = (ram.memory[memPos] & 0xFF);
		programCounter += 3;
		programCounter++;
		break; }

	case 88: { //Write from register0 to cache 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		cache.memory[memPos + 1] = (byte)(register0 >> 8);
		programCounter += 3;
		programCounter++;
		break; }

	case 89: { //Write from register1 to cache 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		cache.memory[memPos + 1] = (byte)(register0 >> 8);
		programCounter += 3;
		programCounter++;
		break; }

	case 90: { //Write from register0 to ram 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		ram.memory[memPos + 1] = (byte)(register0 >> 8);
		programCounter += 3;
		programCounter++;
		break; }

	case 91: { //Write from register1 to ram 2 bytes
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		ram.memory[memPos + 1] = (byte)(register0 >> 8);
		programCounter += 3;
		programCounter++;
		break; }

	case 92: { //Write from register0 to cache 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		programCounter += 3;
		programCounter++;
		break; }

	case 93: { //Write from register1 to cache 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		cache.memory[memPos] = register0 & 0xFF;
		programCounter += 3;
		programCounter++;
		break; }

	case 94: { //Write from register0 to ram 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		programCounter += 3;
		programCounter++;
		break; }

	case 95: { //Write from register1 to ram 1 byte
		byte arg1p1 = cache.memory[programCounter + 1];
		byte arg1p2 = cache.memory[programCounter + 2];
		byte arg1p3 = cache.memory[programCounter + 3];
		int memPos = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		ram.memory[memPos] = register0 & 0xFF;
		programCounter += 3;
		programCounter++;
		break; }

	}
}


void CPU::tick() {
	if (halt == false) {
		registerOP1 = cache.memory[programCounter];
		execute(register1);
	}
}