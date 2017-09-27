#include "CPU.h"

#include <iostream>

CPU::CPU()
	:
	registerOP1(0),
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

	switch (opCode) {
	case 0: {
		halt = true;
		break; }

	case 1:{
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = cache.read(memPos);
		programCounter += 2;
		break; }

	case 2: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register2 = cache.read(memPos);
		programCounter += 2;
		break; }

	case 3: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = ram.read(memPos);
		programCounter += 2;
		break; }

	case 4:{
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		register1 = ram.read(memPos);
		programCounter += 2;
		break; }

	case 5:{
		register1 = 0;
		break; }

	case 6: {
		register2 = 0;
		break; }

	case 7: {
		byte agr1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = agr1p1 + (arg1p2 * 256);
		cache.write(memPos, register1);
		programCounter += 2;
		break; }

	case 8: {
		byte agr1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = agr1p1 + (arg1p2 * 256);
		cache.write(memPos, register2);
		programCounter += 2;
		break; }

	case 9: {
		byte agr1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = agr1p1 + (arg1p2 * 256);
		ram.write(memPos, register1);
		programCounter += 2;
		break; }

	case 10: {
		byte agr1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = agr1p1 + (arg1p2 * 256);
		ram.write(memPos, register2);
		programCounter += 2;
		break; }

	case 11: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		byte arg2p1 = cache.read(programCounter + 3);
		byte arg2p2 = cache.read(programCounter + 4);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p1 + (arg2p2 * 256);
		ram.write(memPos2, cache.read(memPos));
		break; }

	case 12: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		byte arg2p1 = cache.read(programCounter + 3);
		byte arg2p2 = cache.read(programCounter + 4);
		u16 memPos = arg1p1 + (arg1p2 * 256);
		u16 memPos2 = arg2p1 + (arg2p2 * 256);
		cache.write(memPos2, ram.read(memPos));
		break; }

	case 20: {
		register1 = register1 + register2;
		break; }

	case 21: {
		register1 = register1 - register2;
		break; }

	case 22: {
		register1 = register1 * register2;
		break; }

	case 23: {
		register1 = register1 / register2;
		break; }

	case 24: {
		register1 = register1 > register2;
		break; }

	case 25: {
		register1 = register1 >= register2;
		break; }

	case 26: {
		register1 = register1 < register2;
		break; }

	case 27: {
		register1 = register1 <= register2;
		break; }

	case 28: {
		register1 = register1 == register2;
		break; }

	case 29: {
		register1 = register1 != register2;
		break; }

	case 30: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		u16 memPos = arg1p1 + (arg1p2 * 256) - 1;	
		programCounter = memPos;
		break; }

	case 31: {
		byte arg1 = cache.read(programCounter + 1);
		byte arg2p1 = cache.read(programCounter + 2);
		byte arg2p2 = cache.read(programCounter + 3);
		byte condition = arg1;
		u16 memPos = arg2p1 + (arg2p2 * 256) - 1;
		if (register1 == condition) {
			programCounter = memPos;
		}else if(register1 != condition){	
			programCounter += 3;
		}	
		break; }

	case 40: {
		std::cout << register1 << '\n';
		break; }

	case 41: {
		std::cout << register2 << '\n';
		break; }

	case 50: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		byte arg2p1 = cache.read(programCounter + 3);
		byte arg2p2 = cache.read(programCounter + 4);
		byte arg3p1 = cache.read(programCounter + 5);
		byte arg3p2 = cache.read(programCounter + 6);
		byte temp;

		gpu.accessCommandBuffer("write", gpu.commandCounter, 1, temp);

		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 0, arg1p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 1, arg1p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 2, arg2p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 3, arg2p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 4, arg3p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 5, arg3p2, temp);

		programCounter += 6;
		break; }

	case 51: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		byte arg2p1 = cache.read(programCounter + 3);
		byte arg2p2 = cache.read(programCounter + 4);
		byte arg3p1 = cache.read(programCounter + 5);
		byte arg3p2 = cache.read(programCounter + 6);
		byte temp;

		gpu.accessCommandBuffer("write", gpu.commandCounter, 2, temp);

		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 0, arg1p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 1, arg1p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 2, arg2p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 3, arg2p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 4, arg3p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 5, arg3p2, temp);

		programCounter += 6;
		break; }

	case 52: {
		byte arg1p1 = cache.read(programCounter + 1);
		byte arg1p2 = cache.read(programCounter + 2);
		byte arg2p1 = cache.read(programCounter + 3);
		byte arg2p2 = cache.read(programCounter + 4);
		byte arg3p1 = cache.read(programCounter + 5);
		byte arg3p2 = cache.read(programCounter + 6);
		byte arg4p1 = cache.read(programCounter + 7);
		byte arg4p2 = cache.read(programCounter + 8);
		byte arg5 = cache.read(programCounter + 9);
		byte arg6 = cache.read(programCounter + 10);
		byte arg7 = cache.read(programCounter + 11);
		byte temp;

		gpu.accessCommandBuffer("write", gpu.commandCounter, 3, temp);

		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 0 , arg1p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 1 , arg1p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 2 , arg2p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 3 , arg2p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 4 , arg3p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 5 , arg3p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 6 , arg4p1, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 7 , arg4p2, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 8 , arg5, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 9 , arg6, temp);
		gpu.accessCommandArgBuffer("write", gpu.commandArgCounter + 10, arg7, temp);

		programCounter += 11;
		break; }
	}
}

void CPU::tick() {
	if (halt == false) {
		registerOP1 = cache.read(programCounter);
		execute(register1);
		programCounter++;
	}
}

void CPU::accessCache(std::string choice, u16 pos, byte value = 0, byte& output) {
	if (choice == "write") {
		cache.write(pos, value);
	}else if (choice == "read") {
		output = cache.read(pos);
	}
}

void CPU::accessRam(std::string choice, u16 pos, byte value = 0, byte& output) {
	if (choice == "write") {
		ram.write(pos, value);
	}else if (choice == "read") {
		output = ram.read(pos);
	}
}