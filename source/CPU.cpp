#include "CPU.h"

CPU::CPU(int sectorSize_, int numSectors_, Memory& ram_, Memory& hdd_, GPU& gpu_)
	:
	registerOP(0),
	programCounter(0),
	currentTime(0),
	ram(ram_),
	halt(false),
	interrupted(false),
	sectorSize(sectorSize_),
	numSectors(numSectors_),
	hdd(hdd_),
	gpu(gpu_),
	onlineMode(false)
{

	registers.resize(15);
	interRegisters.resize(12);
	modemRegisters.resize(9);
	sockets.resize(255);
	for(int i = 0; i < 12; i++) {interRegisters[i] = 0;}
	for(int i = 0; i < 255; i++) {sockets[i] = false;}
	for(int i = 0; i < 9; i++) {modemRegisters[i] = 0;}
	interrupted = false;
	interruptExecution = false;

	std::ifstream HDD;
	HDD.open("HDD.txt");
	std::string line;

	int j = 0;
	int i = 0;

	std::cout << "Loading HDD...  ";

	std::string buf;
	std::stringstream ss(line);
	int prevP = 0;

	std::cout << '[';

	if (HDD.is_open()){
		while (getline (HDD,line)) {

			ss.str(line);

			while (ss >> buf) {
				hdd.memory[j * sectorSize + i] = std::stoi(buf);
				i++;
			}
			i = 0;
			j++;
			int percent = (int)(((float)(j * sectorSize + i) / (float)(sectorSize * numSectors)) * 100) / 5;

			if(prevP < percent) {std::cout << '|';}
			prevP = percent;

		}

	}

	std::cout << ']' << '\r';

	std::cout << '\n' << "Finished loading the HDD..." << '\n' << '\n';

	HDD.close();
}

int CPU::checkArgument(int source, int size) {
	if ((unsigned int)source <= ram.memory.size()) {
		if(size == 1) { return ram.memory[source] & 0xFF; }
		if(size == 2) { return ram.memory[source + 1] << 8 | ram.memory[source] & 0xFF ; }
		if(size == 3) { return (ram.memory[source + 3] << 24) | (ram.memory[source + 2] << 16) | (ram.memory[source + 1] << 8) | (ram.memory[source] & 0xFF); }
	}else {
		return registers[source - (ram.memory.size() + 1)];
	}
}

int CPU::checkArgumentH(int source, int size) {
	if ((unsigned int)source <= ram.memory.size()) {
		if(size == 1) { return hdd.memory[source] & 0xFF; }
		if(size == 2) { return hdd.memory[source + 1] <<  8 | (hdd.memory[source] & 0xFF); }
		if(size == 3) { return hdd.memory[source + 3] << 24 | (hdd.memory[source + 2] << 16) | (hdd.memory[source + 1] << 8) | (hdd.memory[source] & 0xFF); }
	}else {
		return registers[source - (hdd.memory.size() + 1)];
	}
}

int CPU::checkArgumentG(int source, int size) {
	if ((unsigned int)source <= gpu.vRam.memory.size()) {
		if(size == 1) { return gpu.vRam.memory[source] & 0xFF; }
		if(size == 2) { return gpu.vRam.memory[source + 1] <<  8 | (gpu.vRam.memory[source] & 0xFF); }
		if(size == 3) { return gpu.vRam.memory[source + 3] << 24 | (gpu.vRam.memory[source + 2] << 16) | (gpu.vRam.memory[source + 1] << 8) | (gpu.vRam.memory[source] & 0xFF); }
	}else {
		return registers[source - (gpu.vRam.memory.size() + 1)];
	}
}

extern bool quit;

byte getBits(byte origin, byte length){
	return (origin >> (4 * length)) & 0xF;
}

void CPU::execute(TCPsocket clientSocket) {
	switch (registerOP) {
		case 0x00: { //Halt
			halt = true;
			break; }

		case 0x01: { //Load memory from a postion
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];

			registers[regA] = checkArgument(memPos, sizeA);
			programCounter += 6;
		break; };

		case 0x02: { //Write to memory in postion
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
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
			programCounter += 6;
		break; }

		case 0x03: { //Write to memory in postion with offset
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 9] << 24 | ram.memory[programCounter + 8] << 16 | ram.memory[programCounter + 7] << 8 | ram.memory[programCounter + 6], sizeA);
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
			programCounter += 10;
		break; }

		case 0x04: { //Load memory from a postion with offset
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 9] << 24 | ram.memory[programCounter + 8] << 16 | ram.memory[programCounter + 7] << 8 | ram.memory[programCounter + 6], sizeA);
			registers[regA] = checkArgument(memPos + (offset * sizeA), sizeA);
			programCounter += 10;
		break; };

		case 0x05: { //Shutdown the pc
			std::ofstream file;;

			file.open("HDD.txt", std::ios::out | std::ios::trunc);
			file.close();

			std::cout << '\n' << "Saving the HDD...  [";
			int prevP = 0;

			file.open("HDD.txt");

			for (int i = 0; i < numSectors; i++) {
				for (int j = 0; j < sectorSize; j++) {
					file << (std::to_string((int)hdd.memory[i * sectorSize + j]));
					file << " ";
				}
				file << '\n';

				int percent = (int)(((float)(i + 1) / (float)(numSectors)) * 100) / 5;

				if(prevP < percent) {std::cout << '|';}
				prevP = percent;
			}

			file.close();

			std::cout << ']';
			std::cout << '\n';

			std::cout << "Finished saving the HDD..." << '\n';

			std::cout << "The PC has shut down. Press Ctrl+C to exit the program." << '\n';
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
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			switch(typeA) {
				case 0x0: {
					if(registers[regA] == 0){
						programCounter = position;
					}else{
						programCounter += 6;
					}
				break; }

				case 0x1: {
					if(registers[regA] == 1){
						programCounter = position;
					}else{
						programCounter += 6;
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
			int memPos = ram.memory[programCounter + 6] << 24 | ram.memory[programCounter + 5] << 16 | ram.memory[programCounter + 4] << 8 | ram.memory[programCounter + 3];
			byte arg = checkArgument(memPos, 1);
			byte mode = getBits(ram.memory[programCounter + 2], 0);
			if(mode == 0) { gpu.setCharID(registers[regA], registers[regB], arg); };
			if(mode == 1) { gpu.setCharCB(registers[regA], registers[regB], arg); };
			if(mode == 2) { gpu.setCharCF(registers[regA], registers[regB], arg); };
			gpu.update = true;
			programCounter += 7;
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

		case 0x1C: { //Starts GPU program exectuion
			gpu.tasksX = registers[getBits(ram.memory[programCounter + 1], 0)];
			gpu.tasksY = registers[getBits(ram.memory[programCounter + 1], 1)];

			if(gpu.tasks.size() == 0) {
				for(int y = 0; y < gpu.tasksY; y++){
					for(int x = 0; x < gpu.tasksX; x++){
						gpu.tasks.push_back({x, y});
					}
				}
				gpu.started = true;
			}

			programCounter += 2;
		break; }

		case 0x1D: { //Load from vRam
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgumentG(memPos, sizeA);
			programCounter += 6;
		break; }

		case 0x1E: { //Write to vRam
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
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
			programCounter += 6;
		break; }

		case 0x1F: { //Write to vRam with offset
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 9] << 24 | ram.memory[programCounter + 8] << 16 | ram.memory[programCounter + 7] << 8 | ram.memory[programCounter + 6], sizeA);
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
			programCounter += 10;
		break; }

		case 0x20: { //Load from vRam with offset
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int offset = checkArgument(ram.memory[programCounter + 9] << 24 | ram.memory[programCounter + 8] << 16 | ram.memory[programCounter + 7] << 8 | ram.memory[programCounter + 6], sizeA);
			registers[regA] = checkArgumentG(memPos + (offset * sizeA), sizeA);
			programCounter += 10;
		break; };

		case 0x21: { //Load from interrupt register
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);

			registers[regA] = interRegisters[regB];
			programCounter += 2;
		break; };

		case 0x22: { //Jump from interrupt
			int position = ram.memory[programCounter + 4] << 24 | ram.memory[programCounter + 3] << 16 | ram.memory[programCounter + 2] << 8 | ram.memory[programCounter + 1];
			programCounter = position;
			interruptExecution = false;
		break; }

		case 0x23: { //Load from HDD
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgumentH(memPos, sizeA);
			programCounter += 6;
		break; }

		case 0x24: { //Write to HDD
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			if(sizeA >= 1 && sizeA <= 3) {
				hdd.memory[position] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					hdd.memory[position + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 3) {
					hdd.memory[position + 2] = (byte)(registers[regA] >> 16);
					hdd.memory[position + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x24 at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 6;
		break; }

		case 0x25: { //Modulo
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regB] = registers[regA] % registers[regB];
			programCounter += 2;
		break; }

		case 0x26: { //Call updateCharacters
			//gpu.updateCharacters();
			programCounter++;
		break; }

		case 0x27: { //Load from ram with a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgument(checkArgument(memPos, 3), sizeA);
			programCounter += 6;
		break; };

		case 0x28: { //Write to ram from a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int pointer = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int position = ram.memory[pointer + 3] << 24 | ram.memory[pointer + 2] << 16 | ram.memory[pointer + 1] << 8 | ram.memory[pointer];

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
				std::cout << "Invalid data type size at instruction 0x28 at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 6;
		break; }

		case 0x29: { //Load from HDD with a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgumentH(checkArgument(memPos, 3), sizeA);
			programCounter += 6;
		break; };

		case 0x2A: { //Write to HDD from a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int pointer = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int position = ram.memory[pointer + 3] << 24 | ram.memory[pointer + 2] << 16 | ram.memory[pointer + 1] << 8 | ram.memory[pointer];

			if(sizeA >= 1 && sizeA <= 3) {
				hdd.memory[position] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					hdd.memory[position + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 3) {
					hdd.memory[position + 2] = (byte)(registers[regA] >> 16);
					hdd.memory[position + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x2A at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 6;
		break; }

		case 0x2B: { //Load from vRam with a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[regA] = checkArgumentG(checkArgument(memPos, 3), sizeA);
			programCounter += 6;
		break; };

		case 0x2C: { //Write to vRam from a pointer
			byte argument = ram.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int pointer = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			int position = ram.memory[pointer + 3] << 24 | ram.memory[pointer + 2] << 16 | ram.memory[pointer + 1] << 8 | ram.memory[pointer];

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
				std::cout << "Invalid data type size at instruction 0x2C at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 6;
		break; }

		case 0x2D: { //Load data into modem registers
			if(onlineMode){
				int pointer = ram.memory[programCounter + 4] << 24 | ram.memory[programCounter + 3] << 16 | ram.memory[programCounter + 2] << 8 | ram.memory[programCounter + 1];
				int memPos = checkArgument(pointer, 3);
				modemRegisters[3] = ram.memory[memPos + 0];
				modemRegisters[4] = ram.memory[memPos + 1];
				modemRegisters[5] = ram.memory[memPos + 2];
				modemRegisters[6] = ram.memory[memPos + 3];
				modemRegisters[7] = ram.memory[memPos + 4];

			}else{
				std::cout << "Online mode is off. Please connect vPC to a vPC server to use this" << '\n';
			}
			programCounter += 5;
		break; }

		case 0x2E: { //Load address into modem registers
			if(onlineMode){
				byte argument = ram.memory[programCounter + 1];
				byte regA = getBits(argument, 0);
				byte regB = getBits(argument, 1);
				byte argument2 = ram.memory[programCounter + 2];
				byte regC = getBits(argument2, 0);
				modemRegisters[0] = registers[regA];
				modemRegisters[1] = registers[regB];
				modemRegisters[2] = registers[regC];
			}else{
				std::cout << "Online mode is off. Please connect vPC to a vPC server to use this" << '\n';
			}
			programCounter += 3;
		break; }

		case 0x2F: { //Toggle a socket
			if(onlineMode){
				int memPos = ram.memory[programCounter + 4] << 24 | ram.memory[programCounter + 3] << 16 | ram.memory[programCounter + 2] << 8 | ram.memory[programCounter + 1];

				sockets[checkArgument(memPos, 1)] = !sockets[checkArgument(memPos, 1)];
			}else{
				std::cout << "Online mode is off. Please connect vPC to a vPC server to use this" << '\n';
			}
			programCounter += 5;
		break; }

		case 0x30: { //Set socket for connection
			if(onlineMode){
				int memPos = ram.memory[programCounter + 4] << 24 | ram.memory[programCounter + 3] << 16 | ram.memory[programCounter + 2] << 8 | ram.memory[programCounter + 1];

				modemRegisters[8] = checkArgument(memPos, 1);
			}else{
				std::cout << "Online mode is off. Please connect vPC to a vPC server to use this" << '\n';
			}
			programCounter += 5;
		break; }

		case 0x31: { //Send a message
			if(onlineMode){
				std::string message = "0" + vPCaddress + (char)modemRegisters[0] + (char)modemRegisters[1] + (char)modemRegisters[2]
													   + (char)modemRegisters[3] + (char)modemRegisters[4] + (char)modemRegisters[5] + (char)modemRegisters[6] + (char)modemRegisters[7];
				if(sockets[modemRegisters[8]] == true){
					SDLNet_TCP_Send(clientSocket, message.c_str(), 12);
				}else{
					std::cout << "Failed to send the connection, socket closed" << '\n';
				}
			}else{
				std::cout << "Online mode is off. Please connect vPC to a vPC server to use this" << '\n';
			}
			programCounter++;
		break; }

		case 0x32: { //Set register to some value
			unsigned int reg = ram.memory[programCounter + 1];
			unsigned int value = ram.memory[programCounter + 5] << 24 | ram.memory[programCounter + 4] << 16 | ram.memory[programCounter + 3] << 8 | ram.memory[programCounter + 2];
			registers[reg] = value;
			programCounter += 6;
		break; }
	}
}

void CPU::interrupt(){
	if (interrupted == true && interruptExecution == false) {

		currentIntPos = programCounter;

		byte b1 = currentIntPos & 0xff;
		byte b2 = (byte)(currentIntPos >> 8);
		byte b3 = (byte)(currentIntPos >> 16);

		ram.memory[interTartgetPos] = b1;
		ram.memory[interTartgetPos + 1] = b2;
		ram.memory[interTartgetPos + 2] = b3;
		ram.memory[interTartgetPos + 3] = b3;

		programCounter = interPos;

		interrupted = false;
		interruptExecution = true;
	}
}


void CPU::tick(TCPsocket* clientSocket) {
	if (halt == false) {
		currentTime++;
		registers[13] = currentTime;
		registers[14] = currentFrame;
		registerOP = ram.memory[programCounter];
		execute(*clientSocket);
		interrupt();
	}
}
