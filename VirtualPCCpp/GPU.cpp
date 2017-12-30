#include "GPU.h"


GPU::GPU(int commandBufferSize, int vRamSize, int coreCount, int commandArgBufferSize, SDLWindow* screen_, Memory& ram_)
	:
	commandBuffer(Memory(commandBufferSize)),
	commandArgBuffer(Memory(commandArgBufferSize)),
	vRam(Memory(vRamSize)),
	ram(ram_),
	screen(screen_)
{
	for (int x = 0; x < coreCount; x++) {
		cores.emplace_back(vRam, screen_, 256, x, 0);
	}

	//srand(time(NULL));
	
	int countR = 0;
	int characterCount = 0;
	characters.resize(99);
	
	std::string line;
	std::ifstream myfile ("fontBasic.txt");

	for (int i = 0; i < 99; i++) {
		characters[i].rows.resize(8);
	}
	
	if (myfile.is_open()){
		while (getline (myfile,line)) {
			if (countR < 8) {
				characters[characterCount].rows[countR] = line;
				countR++;
			}else{
				characterCount++;
				countR = 0;
			}
		}
	}
}

void convertByte(int number, byte& b1, byte& b2, byte& b3) {
	b1 = number & 0xff;
	b2 = (byte)(number >> 8);
	b3 = (byte)(number >> 16);
}

void GPU::executeCommand() {
	/*byte instruction = commandBuffer.memory[commandCounter];

	switch (instruction) {

	case 0: {
		commandBuffer.Clear();
		commandArgBuffer.Clear();
		commandCounter = 0;
		break; }

	case 1: {
		for(int i = 0;)
	break; }

	case 2: {
		//TODO
		break; }

	case 3: {
		byte arg1p1 = commandArgBuffer.memory[commandArgCounter + 0];
		byte arg1p2 = commandArgBuffer.memory[commandArgCounter + 1];
		byte arg1p3 = commandArgBuffer.memory[commandArgCounter + 2];
		byte arg2p1 = commandArgBuffer.memory[commandArgCounter + 3];
		byte arg2p2 = commandArgBuffer.memory[commandArgCounter + 4];
		byte arg2p3 = commandArgBuffer.memory[commandArgCounter + 5];

		int arg1 = arg1p3 << 16 | arg1p2 << 8 | (arg1p1 & 0xFF);
		int arg2 = arg2p3 << 16 | arg2p2 << 8 | (arg2p1 & 0xFF);

		for (int i = 0; i < arg2; i++) {
			vRam.memory[(i * 8) + firstAvailableByte + 0] = 3;
			vRam.memory[(i * 8) + firstAvailableByte + 1] = ram.memory[(i * 7) + arg1 + 0];
			vRam.memory[(i * 8) + firstAvailableByte + 2] = ram.memory[(i * 7) + arg1 + 1];
			vRam.memory[(i * 8) + firstAvailableByte + 3] = ram.memory[(i * 7) + arg1 + 2];
			vRam.memory[(i * 8) + firstAvailableByte + 4] = ram.memory[(i * 7) + arg1 + 3];
			vRam.memory[(i * 8) + firstAvailableByte + 5] = ram.memory[(i * 7) + arg1 + 4];
			vRam.memory[(i * 8) + firstAvailableByte + 6] = ram.memory[(i * 7) + arg1 + 5];
			vRam.memory[(i * 8) + firstAvailableByte + 7] = ram.memory[(i * 7) + arg1 + 6];
		}

		firstAvailableByte += 8 * arg2;

		commandArgCounter += 6;
		commandCounter++;
		break; }

	case 4: {
		byte arg1 = commandArgBuffer.memory[commandArgCounter + 0];
		byte arg2 = commandArgBuffer.memory[commandArgCounter + 1];
		byte arg3 = commandArgBuffer.memory[commandArgCounter + 2];

		character cc = characters[arg1];

		for (int i = 0; i < 7; i++) {
			std::string cr = cc.rows[i];
			for (int j = 0; j < 6; j++) {
				int xPos = (arg2 * 6) + j + 1;
				byte b1, b2, b3;
				convertByte(xPos, b1, b2, b3);
				int yPos = (arg3 * 7) + i + 1;
				byte b4, b5, b6;
				convertByte(yPos, b4, b5, b6);
				if (cr[j] == '0') {
					vRam.memory[firstAvailableByte + 0] = 3;
					vRam.memory[firstAvailableByte + 1] = b1;
					vRam.memory[firstAvailableByte + 2] = b2;
					vRam.memory[firstAvailableByte + 3] = b4;
					vRam.memory[firstAvailableByte + 4] = b5;
					vRam.memory[firstAvailableByte + 5] = 0;
					vRam.memory[firstAvailableByte + 6] = 0;
					vRam.memory[firstAvailableByte + 7] = 0;

					firstAvailableByte += 8;
				}else if (cr[j] = '1') {
					vRam.memory[firstAvailableByte + 0] = 3;
					vRam.memory[firstAvailableByte + 1] = b1;
					vRam.memory[firstAvailableByte + 2] = b2;
					vRam.memory[firstAvailableByte + 3] = b4;
					vRam.memory[firstAvailableByte + 4] = b5;
					vRam.memory[firstAvailableByte + 5] = 255;
					vRam.memory[firstAvailableByte + 6] = 255;
					vRam.memory[firstAvailableByte + 7] = 255;

					firstAvailableByte += 8;
				}
			}
		}

		commandArgCounter += 3;
		commandCounter++;
		break; }

	case 5: {
		commandBuffer.Clear();
		commandArgBuffer.Clear();
		commandCounter = 0;
		commandArgCounter = 0;
		break; }

	case 6: {
		vRam.Clear();
		functionCounter = 0;
		firstAvailableByte = 0;

		break; }
	}*/
}

void GPU::executeThread() {
	/*byte instruction = vRam.memory[functionCounter];

	switch (instruction) {

	case 1: {
		//TODO
		break; }	
	case 2: {
		//TODO
		break; }	

	case 3: {
		byte arg1b1 = vRam.memory[functionCounter + 1];
		byte arg1b2 = vRam.memory[functionCounter + 2];
		byte arg2b1 = vRam.memory[functionCounter + 3];
		byte arg2b2 = vRam.memory[functionCounter + 4];
		byte arg3 = vRam.memory[functionCounter + 5];
		byte arg4 = vRam.memory[functionCounter + 6];
		byte arg5 = vRam.memory[functionCounter + 7];

		int arg1  = arg1b2 << 8 | (arg1b1 & 0xFF);
		int arg2  = arg2b2 << 8 | (arg2b1 & 0xFF);

		byte r = arg3 * 2;
		byte g = arg4 * 2;
		byte b = arg5 * 2;

		screen->pixels[arg2 * 320 + arg1] = int(b << 16) | int(g << 8) | int(r);
		functionCounter += 7;
		functionCounter++;
		break; }	
	}*/
}

void GPU::startCores() {
	for (int i = 0; i < cores.size(); i++) {
		//int numb = rand() % tasks.size();
		if (tasks.size() > 0) {
			cores[i].programCounter = programCounter;
			cores[i].idX = tasks[0].x;
			cores[i].idY = tasks[0].y;
			tasks.erase(tasks.begin());
			cores[i].halt = false;
		}else{
			cores[i].idX = 0;
			cores[i].idY = 0;
		}
	}
}

void GPU::tick() {
	for (int i = 0; i < cores.size(); i++) {
		if (cores[i].halt == true) {
			if (tasks.size() > 0) {
				cores[i].programCounter = programCounter;
				cores[i].idX = tasks[0].x;
				cores[i].idY = tasks[0].y;
				tasks.erase(tasks.begin());
				cores[i].halt = false;
			}
		}else{
			cores[i].tick();
		}		
	}
}