#include "CPU.h"

CPU::CPU(int sectorSize_, int numSectors_, Memory& ram_, Memory& hdd_, GPU& gpu_)
	:
	registerOP(0),
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

	registers.resize(12);

	/*std::ifstream HDD;
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

	HDD.close();*/
}

int CPU::checkArgument(int source, int size) {
	if ((unsigned int)source <= ram.memory.size()) {
		if(size == 1) { return ram.memory[source] & 0xFF; } else {return 0;}
		if(size == 2) { return ram.memory[source + 1] <<  8 | (ram.memory[source] & 0xFF); } else {return 0;}
		if(size == 3) { return ram.memory[source + 3] << 24 | (ram.memory[source + 2] << 16) | (ram.memory[source + 1] << 8) | (ram.memory[source] & 0xFF); } else {return 0;}
	}else if (source == ram.memory.size() + 1) {
		return interruptRegister;
	}else {
		return registers[source - (ram.memory.size() + 2)];
	}
}

int CPU::checkArgumentH(int source, int size) {
	if ((unsigned int)source <= ram.memory.size()) {
		if(size == 1) { return hdd.memory[source] & 0xFF; } else {return 0;}
		if(size == 2) { return hdd.memory[source + 1] <<  8 | (hdd.memory[source] & 0xFF); } else {return 0;}
		if(size == 3) { return hdd.memory[source + 3] << 24 | (hdd.memory[source + 2] << 16) | (hdd.memory[source + 1] << 8) | (hdd.memory[source] & 0xFF); } else {return 0;}
	}else if (source == hdd.memory.size() + 1) {
		return interruptRegister;
	}else {
		return registers[source - (hdd.memory.size() + 2)];
	}
}

int CPU::checkArgumentG(int source, int size) {
	if ((unsigned int)source <= gpu.vRam.memory.size()) {
		if(size == 1) { return gpu.vRam.memory[source] & 0xFF; } else {return 0;}
		if(size == 2) { return gpu.vRam.memory[source + 1] <<  8 | (gpu.vRam.memory[source] & 0xFF); } else {return 0;}
		if(size == 3) { return gpu.vRam.memory[source + 3] << 24 | (gpu.vRam.memory[source + 2] << 16) | (gpu.vRam.memory[source + 1] << 8) | (gpu.vRam.memory[source] & 0xFF); } else {return 0;}
	}else if (source == gpu.vRam.memory.size() + 1) {
		return interruptRegister;
	}else {
		return registers[source - (gpu.vRam.memory.size() + 2)];
	}
}

extern bool quit;

byte getBits(byte origin, byte length){
	return (origin >> (4 * length)) & 0xF;
}

void CPU::execute() {
	switch (registerOP) {
		case 0x00: { //Halt
			halt = true;
			break; }

		case 0x01: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgument(memPos, sizeA);
			programCounter += 5;
		break; };

		case 0x02: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			if(sizeA >= 1 && sizeA <= 3) {
				ram.memory[position] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					ram.memory[position + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 3) {
					ram.memory[position + 2] = (byte)(registers[regA] >> 16);
					ram.memory[position + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x02 at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 5;
		break; }

		case 0x03: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 7] << 16 | ram.memory[programCounter + 6] << 8 | ram.memory[programCounter + 5], sizeA);
			if(sizeA >= 1 && sizeA <= 4) {
				ram.memory[position + (offset * sizeA)] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					ram.memory[position + (offset * sizeA) + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 4) {
					ram.memory[position + (offset * sizeA) + 2] = (byte)(registers[regA] >> 16);
					ram.memory[position + (offset * sizeA) + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x03 at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 8;
		break; }

		case 0x04: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 7] << 16 | ram.memory[programCounter + 6] << 8 | ram.memory[programCounter + 5], sizeA);
			registers[regA] = checkArgument(memPos + (offset * sizeA), sizeA);
			programCounter += 8;
		break; };

		case 0x05: { //Shutdown the pc
			/*std::ofstream file;

			//So it is all set up

			file.open("HDD.txt", std::ios::out | std::ios::trunc);
			file.close();
+
			file.open("HDD.txt");
			for (int i = 0; i < numSectors; i++) {
				for (int j = 0; j < sectorSize; j++) {
					file << (int)hdd.memory[i * sectorSize + j] << " ";
				}
				file << '\n';
			}
			file.close();
			std::cout << '\n';*/

			std::cout << "The PC has shut down" << '\n';
			halt = true;
			break; }

		case 0x06: { //Addition
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] + registers[regB];
			programCounter += 2;
		break; }

		case 0x07: { //Subtraction
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] - registers[regB];
			programCounter += 2;
		break; }

		case 0x08: { //Signed multiplication
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) * static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x09: { //Unsigned multiplication
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] * registers[regB];
			programCounter += 2;
		break; }

		case 0x0A: { //Signed integer division
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) / static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0B: { //Unsigned integer division
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] / registers[regB];
			programCounter += 2;
		break; }

		case 0x0C: { //Signed larger than comparison
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) > static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0D: { //Unsigned larger than comarison
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] > registers[regB];
			programCounter += 2;
		break; }

		case 0x0E: { //Signed larger than or equal to comparison
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) >= static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0F: { //Unsigned larger than or equal to comarison
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] >= registers[regB];
			programCounter += 2;
		break; }

		case 0x10: { //Equal comparison
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] == registers[regB];
			programCounter += 2;
		break; }

		case 0x11: { //Jump to place in code
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte typeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			switch(typeA) {
				case 0x0: {
					if(registers[regA] == 0){
						programCounter = position;
					}else{
						programCounter += 5;
					}
				break; }

				case 0x1: {
					if(registers[regA] == 1){
						programCounter = position;
					}else{
						programCounter += 5;
					}
				break; }

				case 0x2: {
					programCounter = position;
				break; }
			}


			break; }

		case 0x12: { //Increment register by 1
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA]++;
			programCounter += 2;
		break; }

		case 0x13: { //Cout from register
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			std::cout << registers[regA] << '\n';
			programCounter += 2;
		break; }

		case 0x14: {
			gpu.startCores();
			programCounter++;
			break; }

		case 0x15: { //Decrement register by 1
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA]--;
			programCounter += 2;
		break; }

		case 0x16: { //Change screen buffer elements
			byte regA = getBits(ram.memory[programCounter + 1], 0);
			byte regB = getBits(ram.memory[programCounter + 1], 1);
			int memPos = ram.memory[programCounter + 5] << 16 | ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3];
			byte arg = checkArgument(memPos, 1);
			byte mode = getBits(ram.memory[programCounter + 2], 0);
			if(mode == 0) { gpu.setCharID(registers[regA], registers[regB], arg); };
			if(mode == 1) { gpu.setCharCB(registers[regA], registers[regB], arg); };
			if(mode == 2) { gpu.setCharCF(registers[regA], registers[regB], arg); };
			programCounter += 6;
			break; }

		case 0x17: { //Set register to 0
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA] = 0;
			programCounter += 2;
		break; }

		case 0x18: { //Get parameter from stack_object
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte param = getBits(argument, 1);
			registers[regA] = stack[currentStackPos].parameters[param];
			programCounter += 2;
		break; }

		case 0x19: { //Set parameter in stack_object
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte param = getBits(argument, 1);
			stack[currentStackPos].parameters[param] = registers[regA];
			programCounter += 2;
		break; }

		case 0x1A: { //Call a function from the stack
			currentStackPos = ram.memory[programCounter + 1];
			stack[currentStackPos].return_register = ram.memory[programCounter + 2];

			int offset = 0;

			for(int i = 0; i < stack[currentStackPos].parameters.size(); i++) {
				int memPos = ram.memory[programCounter + (5 + i * 3)] << 16 | ram.memory[programCounter + (4 + i * 3)] << 8 | ram.memory[programCounter + (3 + i * 3)];
				byte arg = checkArgument(memPos, 1);

				offset += 3;
				stack[currentStackPos].parameters[i] = arg;
			}

			stack[currentStackPos].PP = programCounter + offset + 3;
			programCounter = stack[currentStackPos].BP;
		break; }

		case 0x1B: { //Return from the function (Reserving first parameter as the return value)
			byte argument = ram.memory[programCounter + 1];
			programCounter = stack[currentStackPos].PP;
			registers[stack[currentStackPos].return_register] = stack[currentStackPos].parameters[argument];
		break;}

		case 0x1C: {
			if(gpu.tasks.size() == 0){
				for(int y = 0; y < gpu.tasksY; y++){
					for(int x = 0; x < gpu.tasksX; x++){
						gpu.tasks.push_back({x, y});
					}
				}
				gpu.started = true;
			}
			programCounter++;
		break; }

		case 0x1D: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgumentG(memPos, sizeA);
			programCounter += 5;
		break; }

		case 0x1E: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			if(sizeA >= 1 && sizeA <= 3) {
				gpu.vRam.memory[position] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					gpu.vRam.memory[position + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 3) {
					gpu.vRam.memory[position + 2] = (byte)(registers[regA] >> 16);
					gpu.vRam.memory[position + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x1E at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 5;
		break; }

		case 0x1F: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 7] << 16 | ram.memory[programCounter + 6] << 8 | ram.memory[programCounter + 5], sizeA);
			if(sizeA >= 1 && sizeA <= 4) {
				gpu.vRam.memory[position + (offset * sizeA)] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					gpu.vRam.memory[position + (offset * sizeA) + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 4) {
					gpu.vRam.memory[position + (offset * sizeA) + 2] = (byte)(registers[regA] >> 16);
					gpu.vRam.memory[position + (offset * sizeA) + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x1F at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 8;
		break; }

		case 0x20: {
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 7] << 16 | ram.memory[programCounter + 6] << 8 | ram.memory[programCounter + 5], sizeA);
			registers[regA] = checkArgumentG(memPos + (offset * sizeA), sizeA);
			programCounter += 8;
		break; };
	}
}

void CPU::interrupt(){
	/*if (interrupted == true) {

		currentIntPos = programCounter;

		byte b1 = currentIntPos & 0xff;
		byte b2 = (byte)(currentIntPos >> 8);

		ram.memory[interTartgetPos] = b1;
		ram.memory[interTartgetPos + 1] = b2;

		programCounter = interPos;

		interrupted = false;
	}*/
}


void CPU::tick() {
	if (halt == false) {
		registerOP = ram.memory[programCounter];
		execute();
		//interrupt();
	}
}
