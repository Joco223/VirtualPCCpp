#include "GPUCore.h"

GPUCore::GPUCore(Memory& vRam_, SDLWindow* screen_, int intMemSize, int idX_, int idY_) 
	:
	vRam(vRam_),
	screen(screen_),
	halt(true),
	idX(idX_),
	intMem(Memory(intMemSize)),
	idY(idY_)
	{}

void GPUCore::tick() {
	if (halt == false) {
		registerOP = vRam.memory[programCounter];
		execute(registerOP);
	}
}

int GPUCore::checkArgumentV(int source, int size) {
	if ((unsigned int)source <= vRam.memory.size()) {
		if(size == 1) { return vRam.memory[source] & 0xFF; }
		if(size == 2) { return (vRam.memory[source + 1] << 8 | (vRam.memory[source] & 0xFF)); } 
	}else {
		return 0;
	}
}

int GPUCore::checkArgumentInt(int source, int size) {
	if ((unsigned int)source <= intMem.memory.size()) {
		if(size == 1) { return intMem.memory[source] & 0xFF; }
		if(size == 2) { return intMem.memory[source + 1] << 8 | (intMem.memory[source] & 0xFF); }
	}else if (source == intMem.memory.size() + 1) {
		return register0;
	}else if (source == intMem.memory.size() + 2) {
		return register1;
	}else if (source == intMem.memory.size() + 3) {
		return idX;
	}else if (source == intMem.memory.size() + 4) {
		return idY;
	}else {
		return 0;
	}
}

void GPUCore::execute(int registerOP) {
	int inst = registerOP;

	switch (inst) {
		case 0: {
			halt = true;
			break; }

		case 1: { //Load to register0 from ram, 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentInt(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 2: { //Load to register1 from ram, 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentInt(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 3: { //Load to register0 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentInt(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 4: { //Load to register1 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentInt(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 5: { //Write from register0 to ram 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int position = vRam.memory[programCounter + 2] << 8 | arg1;
			intMem.memory[position] = (register0 & 0xFF);
			intMem.memory[position + 1] = (byte)(register0 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 6: { //Write from register1 to ram 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int position = vRam.memory[programCounter + 2] << 8 | arg1;
			intMem.memory[position] = (register1 & 0xFF);
			intMem.memory[position + 1] = (byte)(register1 >> 8);
			programCounter += 2;
			programCounter++;
			break; }

		case 7: { //Write from register0 to ram 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int position = vRam.memory[programCounter + 2] << 8 | arg1;
			intMem.memory[position] = (register0 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }

		case 8: { //Write from register1 to ram 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int position = vRam.memory[programCounter + 2] << 8 | arg1;
			intMem.memory[position] = (register1 & 0xFF);
			programCounter += 2;
			programCounter++;
			break; }

		case 10: { //Add register0 and register1
			register0 = register0 + register1;
			programCounter++;
			break; }

		case 11: { //Subtract register1 from register0
			register0 = register0 - register1;
			programCounter++;
			break; }

		case 12: { //Multiply register0 and register1
			register0 = register0 * register1;
			programCounter++;
			break; }

		case 13: { //Divide register0 by register1
			register0 = register0 / register1;
			programCounter++;
			break; }

		case 14: { //Check if register0 is greater than register1
			register0 = register0 > register1;
			programCounter++;
			break; }

		case 15: { //Check if register0 is greater or equal to register1
			register0 = register0 >= register1;
			programCounter++;
			break; }

		case 16: { //Check if register1 is greater than register0
			register0 = register0 < register1;
			programCounter++;
			break; }

		case 17: { //Check if register1 is greater or equal to register0
			register0 = register0 <= register1;
			programCounter++;
			break; }

		case 18: { //Check if register0 and register1 have same values
			register0 = register0 == register1;
			programCounter++;
			break; }

		case 19: { //Check if register0 is different than register1
			register0 = register0 != register1;
			programCounter++;
			break; }

		case 20: { //Jump to place in code
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;

			programCounter = memPos;
			break; }

		case 21: { //Compare to true/false
			byte condition = vRam.memory[programCounter + 1];
			byte arg1 = vRam.memory[programCounter + 2];
			int position = vRam.memory[programCounter + 3] << 8 | arg1;
			if (register0 == condition) {
				programCounter += 3;
				programCounter++;
			}else if(register0 != condition){		
				programCounter = position;	
			}
			break; }

		case 26: { //Load to register1 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			regR = checkArgumentInt(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 27: { //Load to register1 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			regG = checkArgumentInt(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 28: { //Load to register1 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			regB = checkArgumentInt(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 29: { //Check if register0 is greater or equal to register1
			int x = static_cast<char>(register0 >> 32) | static_cast<char>(register0 >> 24) | static_cast<char>(register0 >> 16) | static_cast<char>(register0 >> 8) | static_cast<char>(register0 >> 0xff);
			int y = static_cast<char>(register1 >> 32) | static_cast<char>(register1 >> 24) | static_cast<char>(register1 >> 16) | static_cast<char>(register1 >> 8) | static_cast<char>(register1 >> 0xff);

			register0 = x >= y;
			programCounter++;
			break; }

		case 30: { //Color a single pixel on screen
			screen->pixels[register1 * 320 + register0] = int(regB << 16) | int(regG << 8) | int(regR);

			programCounter++;
			break; }

		case 31: { //Get core id
			register0 = idX;
			programCounter++;
			break; }

		case 32: { //Get core id
			register0 = idY;
			programCounter++;
			break; }

		case 33: { //Load to register0 from ram, 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentV(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 34: { //Load to register1 from ram, 2 bytes
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentV(memPos, 2);
			programCounter += 2;
			programCounter++;
			break; }

		case 35: { //Load to register0 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register0 = checkArgumentV(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 36: { //Load to register1 from ram, 1 byte
			byte arg1 = vRam.memory[programCounter + 1];
			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			register1 = checkArgumentV(memPos, 1);
			programCounter += 2;
			programCounter++;
			break; }

		case 37: {
			byte arg1 = vRam.memory[programCounter + 1];
			byte arg2 = vRam.memory[programCounter + 3];
			byte arg3 = vRam.memory[programCounter + 5];

			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			int memPos2 = vRam.memory[programCounter + 4] << 8 | arg2;
			int memPos3 = vRam.memory[programCounter + 6] << 8 | arg3;

			int v1 = checkArgumentInt(memPos, 2);
			int v2 = checkArgumentInt(memPos2, 2);
			int v3 = checkArgumentInt(memPos3, 2);

			register0 = std::max({v1, v2, v3});

			programCounter += 6;
			programCounter++;
			break; }

		case 38: {
			byte arg1 = vRam.memory[programCounter + 1];
			byte arg2 = vRam.memory[programCounter + 3];
			byte arg3 = vRam.memory[programCounter + 5];

			int memPos = vRam.memory[programCounter + 2] << 8 | arg1;
			int memPos2 = vRam.memory[programCounter + 4] << 8 | arg2;
			int memPos3 = vRam.memory[programCounter + 6] << 8 | arg3;

			int v1 = checkArgumentInt(memPos, 2);
			int v2 = checkArgumentInt(memPos2, 2);
			int v3 = checkArgumentInt(memPos3, 2);

			register0 = std::min({v1, v2, v3});

			programCounter += 6;
			programCounter++;
			break; }

	}
}