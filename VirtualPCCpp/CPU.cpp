#include "CPU.h"

CPU::CPU(int sectorSize_, int numSectors_, Memory& ram_, Memory& hdd_, GPU& gpu_)
	:
	registerOP(0),
	register0(0),
	register1(0),
	programCounter(0),
	interruptRegister(0),
	currentTime(0),
	ram(ram_),
	halt(false),
	interrupted(false),
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
	if ((unsigned int)source <= ram.memory.size()) {
		if(size == 1) { return ram.memory[source] & 0xFF; } else {return 0;}
		if(size == 2) { return ram.memory[source + 1] <<  8 | (ram.memory[source] & 0xFF); } else {return 0;}
		if(size == 3) { return ram.memory[source + 2] << 16 | (ram.memory[source + 1] << 8) | (ram.memory[source] & 0xFF); } else {return 0;}
	}else if (source == ram.memory.size() + 1) {
		return register0;
	}else if (source == ram.memory.size() + 2) {
		return register1;
	}else if (source == ram.memory.size() + 3){
		return interruptRegister;
	}else {
		return 0;
	}
}

int CPU::checkArgumentG(int source, int size) {
	if ((unsigned int)source <= gpu.vRam.memory.size()) {
		if(size == 1) { return gpu.vRam.memory[source] & 0xFF; } else {return 0;}
		if(size == 2) { return gpu.vRam.memory[source + 1] <<  8 | (gpu.vRam.memory[source] & 0xFF); } else {return 0;}
		if(size == 3) { return gpu.vRam.memory[source + 2] << 16 | (gpu.vRam.memory[source + 1] << 8) | (gpu.vRam.memory[source] & 0xFF); } else {return 0;}
	}else if (source == gpu.vRam.memory.size() + 1) {
		return register0;
	}else if (source == gpu.vRam.memory.size() + 2) {
		return register1;
	}else if (source == gpu.vRam.memory.size() + 3){
		return interruptRegister;
	}else {
		return 0;
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

		case 3: { //Load to register0 from ram, 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgument(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 4: { //Load to register1 from ram, 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgument(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 5: { //Load to register0 from ram, 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgument(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 6: { //Load to register1 from ram, 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgument(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 7: { //Write from register0 to ram 3 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = register0 & 0xFF;
			ram.memory[position + 1] = (byte)(register0 >> 8);
			ram.memory[position + 2] = (byte)(register0 >> 16);
			programCounter += 2;
			programCounter++;
			break; }

		case 8: { //Write from register1 to ram 3 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = register1 & 0xFF;
			ram.memory[position + 1] = (byte)(register1 >> 8);
			ram.memory[position + 2] = (byte)(register1 >> 16);
			programCounter += 2;
			programCounter++;
			break; }

		case 9: { //Write from register0 to ram 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = (register0 & 0xFF);
			ram.memory[position + 1] = (byte)(register0 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 10: { //Write from register1 to ram 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = (register1 & 0xFF);
			ram.memory[position + 1] = (byte)(register1 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 11: { //Write from register0 to ram 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = (register0 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }

		case 12: { //Write from register1 to ram 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			ram.memory[position] = (register1 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }

		case 15: { //Write from ram to hdd
			int position = register0;

			int position2 = register1;

			hdd.memory[position2] = ram.memory[position];
			programCounter++;
			break; }

		case 16: { //Write from hdd to ram
			int position = register0;

			int position2 = register1;

			ram.memory[position2] = hdd.memory[position];
			programCounter++;
			break; }

		case 17: { //Shutdown the pc
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
			//halt = true;
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
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;

			programCounter = memPos;
			break; }

		case 31: { //Compare to true/false
			byte condition = ram.memory[programCounter + 1];
			byte arg1 = ram.memory[programCounter + 2];
			int position = ram.memory[programCounter + 3] << 8 | arg1;
			if (register0 == condition) {
				programCounter += 3;
				programCounter++;
			}else if(register0 != condition){		
				programCounter = position;	
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

		case 42: { //Load 3 bytes into register0 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register0 = checkArgument(memPos + offset * 3, 3);
			programCounter += 4;
			programCounter++;
			break; }

		case 43: { //Load 3 bytes into register1 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register1 = checkArgument(memPos + offset * 3, 3);
			programCounter += 4;
			programCounter++;
			break; }

		case 44: { //Load 2 bytes into register0 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register0 = checkArgument(memPos + offset * 2, 2);
			programCounter += 4;
			programCounter++;
			break; }

		case 45: { //Load 2 bytes into register1 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register1 = checkArgument(memPos + offset * 2, 2);
			programCounter += 4;
			programCounter++;
			break; }

		case 46: { //Load 1 byte into register0 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register0 = checkArgument(memPos + offset, 1);
			programCounter += 4;
			programCounter++;
			break; }

		case 47: { //Load 1 byte into register1 with offset
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			register1 = checkArgument(memPos + offset, 1);
			programCounter += 4;
			programCounter++;
			break; }

		case 48: { //Write from register0 to ram 3 bytes with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = register0 & 0xFF;
			ram.memory[position + 1 + offset] = (byte)(register0 >> 8);
			ram.memory[position + 2 + offset] = (byte)(register0 >> 16);
			programCounter += 4;
			programCounter++;
			break; }

		case 49: { //Write from register1 to ram 3 bytes with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = register1 & 0xFF;
			ram.memory[position + 1 + offset] = (byte)(register1 >> 8);
			ram.memory[position + 2 + offset] = (byte)(register1 >> 16);
			programCounter += 4;
			programCounter++;
			break; }

		case 50: { //Write from register0 to ram 2 bytes with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = (register0 & 0xFF);
			ram.memory[position + 1 + offset] = (byte)(register0 >> 8);
			programCounter += 4;
			programCounter++;
			break; }

		case 51: { //Write from register1 to ram 2 bytes with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = (register1 & 0xFF);
			ram.memory[position + 1 + offset] = (byte)(register1 >> 8);
			programCounter += 4;
			programCounter++;
			break; }

		case 52: { //Write from register0 to ram 1 byte with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = (register0 & 0xFF);
			programCounter += 4;
			programCounter++;
			break; }

		case 53: { //Write from register1 to ram 1 byte with offset
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			int offset = checkArgument(ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3], 1);
			ram.memory[position + offset] = (register1 & 0xFF);
			programCounter += 4;
			programCounter++;
			break; }

		case 54: { //Compare two arrays of 8bit elements if they are the same (Up to 8 elements length)
			int size = register0;
			int pos1 = ram.memory[programCounter + 2] << 8 | ram.memory[programCounter + 1];
			int pos2 = ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3];

			bool same = true;
			
			for (int i = 0; i < size; i++) {
				int left = checkArgument(pos1 + i, 1);
				int right = checkArgument(pos2 + i, 1);
				if (left != right) {
					same = false;
					break;
				}
			}

			register0 = same;
			programCounter += 4;
			programCounter++;
			break; }

		case 80: { //Draw a character from register0
			/*byte arg1 = register0;
			byte arg2 = ram.memory[ram.memory[programCounter + 1]];
			byte arg3 = ram.memory[ram.memory[programCounter + 2]];

			gpu.commandBuffer.memory[gpu.commandCounter] = 4;

			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1;
			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg2;
			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg3;

			programCounter += 2;
			programCounter++;*/
			break; }

		case 81: { //Draw a character from register1
			/*byte arg1 = register1;
			byte arg2 = ram.memory[ram.memory[programCounter + 1]];
			byte arg3 = ram.memory[ram.memory[programCounter + 2]];

			gpu.commandBuffer.memory[gpu.commandCounter] = 4;

			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 0] = arg1;
			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 1] = arg2;
			gpu.commandArgBuffer.memory[gpu.commandArgCounter + 2] = arg3;

			programCounter += 2;
			programCounter++;*/
			break; }

		case 82: { //Clear GPU commandBuffer
			/*gpu.commandBuffer.memory[gpu.commandCounter] = 5;
			programCounter++;*/
			break; }

		case 83: { //Clear GPU VRAM
			/*if (gpu.functionCounter == gpu.firstAvailableByte) {
				gpu.commandBuffer.memory[gpu.commandCounter] = 6;
				programCounter++;
			}*/
			break; }

		case 84: {
			gpu.startCores();
			programCounter++;
			break; }

		case 85: { //Load to register0 from ram, 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentG(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 86: { //Load to register1 from ram, 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentG(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 87: { //Load to register0 from ram, 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentG(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 88: { //Load to register1 from ram, 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int memPos = ram.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentG(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 89: { //Write from register0 to ram 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			gpu.vRam.memory[position] = (register0 & 0xFF);
			gpu.vRam.memory[position + 1] = (byte)(register0 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 90: { //Write from register1 to ram 2 bytes
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			gpu.vRam.memory[position] = (register1 & 0xFF);
			gpu.vRam.memory[position + 1] = (byte)(register1 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 91: { //Write from register0 to ram 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			gpu.vRam.memory[position] = (register0 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }

		case 92: { //Write from register1 to ram 1 byte
			byte arg1 = ram.memory[programCounter + 1];
			int position = ram.memory[programCounter + 2] << 8 | arg1;
			gpu.vRam.memory[position] = (register1 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }
	}
}

void CPU::interrupt(){
	if (interrupted == true) {

		currentIntPos = programCounter;

		byte b1 = currentIntPos & 0xff;
		byte b2 = (byte)(currentIntPos >> 8);

		ram.memory[interTartgetPos] = b1;
		ram.memory[interTartgetPos + 1] = b2;

		programCounter = interPos;

		interrupted = false;
	}
}


void CPU::tick() {
	if (halt == false) {
		registerOP = ram.memory[programCounter];
		execute(registerOP);
		interrupt();
	}
}