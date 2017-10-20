#include "CPU.h"

CPU::CPU(int sectorSize_, int numSectors_, Memory& ram_, Memory& hdd_, GPU& gpu_)
	:
	registerOP(0),
	register0(0),
	register1(0),
	programCounter(0),
	keyboardRegister(0),
	currentTime(0),
	firstAvailableS(0),
	ram(ram_),
	halt(false),
	sectorSize(sectorSize_),
	numSectors(numSectors_),
	hdd(hdd_),
	gpu(gpu_)
{

	std::ifstream HDD;
	HDD.open("HDD.txt");
	std::string line;

	int j = 0;
	int i = 0;

	if (HDD.is_open()){
		while (getline (HDD,line)) {
			std::string buf; 
			std::stringstream ss(line);

			while (ss >> buf) {
				hdd.memory[j * sectorSize + i] = std::stoi(buf);
				i++;
			}
			i = 0;
			j++;
		}
	}

	HDD.close();
}

int CPU::checkArgument(int source, int size) {
	if (source <= ram.memory.size()) {
		if(size == 1) { return ram.memory[source] & 0xFF; }
		if(size == 2) { return ram.memory[source + 1] <<  8 | (ram.memory[source] & 0xFF); }
		if(size == 3) { return ram.memory[source + 2] << 16 | (ram.memory[source + 1] << 8) | (ram.memory[source] & 0xFF); }
	}else if (source == ram.memory.size() + 1) {
		return register0;
	}else if (source == ram.memory.size() + 2) {
		return register1;
	}
}

extern bool quit;

void CPU::execute(u16 registerIns) {
	u16 opCode = registerIns;

	switch (opCode) {
	case 0: { //Halt
		halt = true;
		break; }

	case 1: { //Load to register0 from ram, 3 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register0 = checkArgument(memPos, 3);
		programCounter += 2;
		programCounter++;
		break; }

	case 2: { //Load to register1 from ram, 3 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register1 = checkArgument(memPos, 3);
		programCounter += 2;
		programCounter++;
		break; }

	case 4: { //Load to register0 from ram, 2 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register0 = checkArgument(memPos, 2);
		programCounter += 2;
		programCounter++;
		break; }

	case 5: { //Load to register1 from ram, 2 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register1 = checkArgument(memPos, 2);
		programCounter += 2;
		programCounter++;
		break; }

	case 6: { //Load to register0 from ram, 1 byte
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register0 = checkArgument(memPos, 1);
		programCounter += 2;
		programCounter++;
		break; }

	case 7: { //Load to register1 from ram, 1 byte
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		register1 = checkArgument(memPos, 1);
		programCounter += 2;
		programCounter++;
		break; }

	case 8: { //Write from register0 to ram 3 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		int position = checkArgument(memPos, 3);
		ram.memory[position] = register0 & 0xFF;
		ram.memory[position + 1] = (byte)(register0 >> 8);
		ram.memory[position + 2] = (byte)(register0 >> 16);
		programCounter += 2;
		programCounter++;
		break; }

	case 9: { //Write from register1 to ram 3 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int memPos = ram.memory[programCounter + 2] << 8 | arg1;
		int position = checkArgument(memPos, 3);
		ram.memory[position] = register1 & 0xFF;
		ram.memory[position + 1] = (byte)(register1 >> 8);
		ram.memory[position + 2] = (byte)(register1 >> 16);
		programCounter += 2;
		programCounter++;
		break; }

	case 10: { //Write from register0 to ram 2 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int position = ram.memory[programCounter + 2] << 8 | arg1;
		ram.memory[position] = (register0 & 0xFF);
		ram.memory[position + 1] = (byte)(register0 >> 8);
		programCounter += 2;
		programCounter++;
		break; }

	case 11: { //Write from register1 to ram 2 bytes
		byte arg1 = ram.memory[programCounter + 1];
		int position = ram.memory[programCounter + 2] << 8 | arg1;
		ram.memory[position] = (register1 & 0xFF);
		ram.memory[position + 1] = (byte)(register1 >> 8);
		programCounter += 2;
		programCounter++;
		break; }

	case 12: { //Write from register0 to ram 1 byte
		byte arg1 = ram.memory[programCounter + 1];
		int position = ram.memory[programCounter + 2] << 8 | arg1;
		ram.memory[position] = (register0 & 0xFF);
		programCounter += 2;
		programCounter++;
		break; }

	case 13: { //Write from register1 to ram 1 byte
		byte arg1 = ram.memory[programCounter + 1];
		int position = ram.memory[programCounter + 2] << 8 | arg1;
		ram.memory[position] = (register1 & 0xFF);
		programCounter += 2;
		programCounter++;
		break; }

	case 16: { //Write from ram to hdd
		int position = register0;

		int position2 = register1;

		hdd.memory[position2] = ram.memory[position];
		programCounter++;
		break; }

	case 17: { //Write from hdd to ram
		int position = register0;

		int position2 = register1;

		ram.memory[position2] = hdd.memory[position];
		programCounter++;
		break; }

	case 18: { //Shutdown the pc

		std::ofstream file;

		file.open("HDD.txt", std::ios::out | std::ios::trunc);
		file.close();

		file.open("HDD.txt");
		for (int i = 0; i < numSectors; i++) {
			for (int j = 0; j < sectorSize; j++) {
				file << (int)hdd.memory[i * sectorSize + j] << " ";
			}
			file << '\n';
		}
		file.close();
		std::cout << '\n';
		std::cout << "The PC has shut down" << '\n';
		programCounter++;
		break; }

	case 20: { //Add register0 and register1
		register0 = register0 + register1;
		programCounter++;
		break; }

	case 21: { //Subtract register1 from register0
		register0 = register0 - register1;
		programCounter++;
		break; }

	case 22: { //Multiply register0 and register1
		register0 = register0 * register1;
		programCounter++;
		break; }

	case 23: { //Divide register0 by register1
		register0 = register0 / register1;
		programCounter++;
		break; }

	case 24: { //Check if register0 is greater than register1
		register0 = register0 > register1;
		programCounter++;
		break; }

	case 25: { //Check if register0 is greater or equal to register1
		register0 = register0 >= register1;
		programCounter++;
		break; }

	case 26: { //Check if register1 is greater than register0
		register0 = register0 < register1;
		programCounter++;
		break; }

	case 27: { //Check if register1 is greater or equal to register0
		register0 = register0 <= register1;
		programCounter++;
		break; }

	case 28: { //Check if register0 and register1 have same values
		register0 = register0 == register1;
		programCounter++;
		break; }

	case 29: { //Check if register0 is different than register1
		register0 = register0 != register1;
		programCounter++;
		break; }

	case 30: { //Jump to place in code
		int memPos = register0;
		int position = checkArgument(memPos, 3);
		programCounter = position;
		break; }

	case 31: { //Compare to true/false
		byte condition = ram.memory[programCounter + 1];
		int position = register1;
		if (register0 == condition) {
			programCounter = position;
		}else if(register0 != condition){
			programCounter++;
			programCounter++;
		}
		break; }

	case 32: { //Wait a number of cycles
		int memPos = register0;
		int waitTime = checkArgument(memPos, 3);
		if (currentTime < waitTime) {
			currentTime++;
		}else if (currentTime >= waitTime) {
			currentTime = 0;
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
		int memPos = register0;
		int position = checkArgument(memPos, 3);
		ram.memory[position] = keyboardRegister;
		programCounter++;
		break; }

	case 50: { //Copy from RAM to VRAM
		
		break; }

	case 51: { //Copy from VRAM to RAM
		
		break; }

	case 52: { //Draw rectangle
		int position = register0;

		int position2 = register1;

		gpu.commandBuffer.memory[gpu.commandCounter] = 3;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = (position & 0xff);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = (byte)(position >> 8);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = (byte)(position >> 16);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 3] = (position2 & 0xff);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 4] = (byte)(position2 >> 8);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 5] = (byte)(position2 >> 16);

		programCounter++;
		break; }

	case 53: { //Draw a character
		int position = register0;

		gpu.commandBuffer.memory[gpu.commandCounter] = 4;

		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = (position & 0xff);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = (byte)(position >> 8);
		gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = (byte)(position >> 16);

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
		registerOP = ram.memory[programCounter];
		execute(registerOP);
	}
}