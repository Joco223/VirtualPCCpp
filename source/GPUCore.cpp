#include "GPUCore.h"

GPUCore::GPUCore(Memory& vRam_, Memory& progMem_, SDLWindow* screen_, bool& screenUpdated_, int coresYS_, int idX_, int idY_)
	:
	vRam(vRam_),
	progMem(progMem_),
	coresYS(coresYS_),
	screen(screen_),
	halt(true),
	idX(idX_),
	idY(idY_),
	screenUpdated(screenUpdated_)
	{
		registers.resize(12);
	}

void GPUCore::tick() {
	registerOP = progMem.memory[programCounter];
	execute(registerOP);
}

int GPUCore::checkArgument(int source, int size) {
	if ((unsigned int)source <= vRam.memory.size()) {
		if(size == 1) { return vRam.memory[source] & 0xFF; }
		if(size == 2) { return vRam.memory[source + 1] <<  8 | vRam.memory[source] & 0xFF; }
		if(size == 3) { return vRam.memory[source + 3] << 24 | vRam.memory[source + 2] << 16 | vRam.memory[source + 1] << 8 | vRam.memory[source] & 0xFF; }
	}else{
		return registers[source - (vRam.memory.size() + 1)];
	}
}

byte GPUCore::getBits(byte origin, byte length){
	return (origin >> (4 * length)) & 0xF;
}

void GPUCore::execute(int registerOP) {
	int inst = registerOP;

	switch (inst) {
		case 0x00: { //Halt
			halt = true;
			break; }

		case 0x01: {
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
			registers[regA] = checkArgument(memPos, sizeA);
			programCounter += 6;
		break; };

		case 0x03: {
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int position =progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
			int offset = checkArgument(progMem.memory[programCounter + 9] << 24 | progMem.memory[programCounter + 8] << 16 | progMem.memory[programCounter + 7] << 8 | progMem.memory[programCounter + 6], sizeA);
			if(sizeA >= 1 && sizeA <= 4) {
				vRam.memory[position + (offset * sizeA)] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					vRam.memory[position + (offset * sizeA) + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 4) {
					vRam.memory[position + (offset * sizeA) + 2] = (byte)(registers[regA] >> 16);
					vRam.memory[position + (offset * sizeA) + 3] = (byte)(registers[regA] >> 24);
				}
			}
			programCounter += 10;
		break; }

		case 0x04: {
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
			int offset = checkArgument(progMem.memory[programCounter + 9] << 24 | progMem.memory[programCounter + 8] << 16 | progMem.memory[programCounter + 7] << 8 | progMem.memory[programCounter + 6], sizeA);
			registers[regA] = checkArgument(memPos + (offset * sizeA), sizeA);
			programCounter += 10;
		break; };

		case 0x06: { //Addition
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] + registers[regB];
			programCounter += 2;
		break; }

		case 0x07: { //Subtraction
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] - registers[regB];
			programCounter += 2;
		break; }

		case 0x08: { //Signed multiplication
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) * static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x09: { //Unsigned multiplication
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] * registers[regB];
			programCounter += 2;
		break; }

		case 0x0A: { //Signed integer division
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) / static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0B: { //Unsigned integer division
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] / registers[regB];
			programCounter += 2;
		break; }

		case 0x0C: { //Signed larger than comparison
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) > static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0D: { //Unsigned larger than comarison
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] > registers[regB];
			programCounter += 2;
		break; }

		case 0x0E: { //Signed larger than or equal to comparison
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = static_cast<signed int>(registers[regA]) >= static_cast<signed int>(registers[regB]);
			programCounter += 2;
		break; }

		case 0x0F: { //Unsigned larger than or equal to comarison
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] >= registers[regB];
			programCounter += 2;
		break; }

		case 0x10: { //Equal comparison
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] == registers[regB];
			programCounter += 2;
		break; }

		case 0x11: { //Jump to place in code
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte typeA = getBits(argument, 1);
			int position = progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
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
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA]++;
			programCounter += 2;
		break; }

		case 0x13: { //Decrement register by 1
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA]--;
			programCounter += 2;
		break; }

		case 0x14: { //Set register to 0
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA] = 0;
			programCounter += 2;
		break; }

		case 0x15: { //Color a single pixel on screen
			byte argument = progMem.memory[programCounter + 1];
			byte regR = registers[getBits(argument, 0)];
			byte regG = registers[getBits(argument, 1)];
			byte argument2 = progMem.memory[programCounter + 2];
			byte regB = registers[getBits(argument2, 0)];

			if(registers[0] >= 0 && registers[0] <= 640 && registers[1] >= 0 && registers[1] <= 480){
				screen->pixels[registers[1] * 640 + registers[0]] = int(regR << 16) | int(regG << 8) | int(regB);
			}

			programCounter += 3;
			screenUpdated = true;
			break; }

		case 0x16: { //Get core idX
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA] = idX;
			programCounter += 2;
			break; }

		case 0x17: { //Get core idY
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA] = idY;
			programCounter += 2;
			break; }

		case 0x18: { //Get core ID
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			registers[regA] = (idX + coresYS * idY);
			programCounter += 2;
			break; }

		case 0x19: { //Modulo
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte regB = getBits(argument, 1);
			registers[regA] = registers[regA] % registers[regB];
			programCounter += 2;
		break; }

		case 0x1A: { //Load from vRam with a pointer
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int memPos = progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
			registers[regA] = checkArgument(checkArgument(memPos, 3), sizeA);
			programCounter += 8;
		break; };

		case 0x1B: { //Write to vRam from a pointer
			byte argument = progMem.memory[programCounter + 1];
			byte regA = getBits(argument, 0);
			byte sizeA = getBits(argument, 1);
			int pointer =progMem.memory[programCounter + 6] << 24 | progMem.memory[programCounter + 5] << 24 | progMem.memory[programCounter + 4] << 16 | progMem.memory[programCounter + 3] << 8 | progMem.memory[programCounter + 2];
			int position = progMem.memory[pointer + 2] << 16 | progMem.memory[pointer + 1] << 8 | progMem.memory[pointer];

			if(sizeA >= 1 && sizeA <= 3) {
				vRam.memory[position] = registers[regA] & 0xFF;
				if(sizeA >= 2) {
					vRam.memory[position + 1] = (byte)(registers[regA] >> 8);
				}
				if(sizeA == 3) {
					vRam.memory[position + 2] = (byte)(registers[regA] >> 16);
					vRam.memory[position + 3] = (byte)(registers[regA] >> 24);
				}
			}else{
				std::cout << "Invalid data type size at instruction 0x1B at memory position: 0x" << std::hex << programCounter << '\n';
			}
			programCounter += 7;
		break; }
	}
}
