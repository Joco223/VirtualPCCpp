#include "GPU.h"

#include <iostream>
#include <bitset>
#include <vector>

GPU::GPU()
	:
	builtIn(Memory(0)),
	characters(Memory(0)),
	functionBuffer(Memory(0)),
	commandBuffer(Memory(64)),
	argBuffer(Memory(384)),
	screen(SDLWindow()),
	programCounter(0),
	programSize(0){}

GPU::GPU(byte builtInSize, u16 functionBufferSize, SDLWindow& screen_)
	:
	builtIn(Memory((int)builtInSize)),
	characters(Memory(350)),
	functionBuffer(Memory(functionBufferSize)),
	commandBuffer(Memory(64)),
	argBuffer(Memory(384)),
	screen(screen_),
	programCounter(0),
	programSize(0)
{
	characters.memory[  0] = 0b00100;
	characters.memory[  1] = 0b01010;
	characters.memory[  2] = 0b01110;
	characters.memory[  3] = 0b01010;
	characters.memory[  4] = 0b00100;

	characters.memory[  5] = 0b00100;
	characters.memory[  6] = 0b01100;
	characters.memory[  7] = 0b00100;
	characters.memory[  8] = 0b00100;
	characters.memory[  9] = 0b01110;

	characters.memory[ 10] = 0b01110;
	characters.memory[ 11] = 0b01010;
	characters.memory[ 12] = 0b00100;
	characters.memory[ 13] = 0b01000;
	characters.memory[ 14] = 0b01110;

	characters.memory[ 15] = 0b01100;
	characters.memory[ 16] = 0b00010;
	characters.memory[ 17] = 0b01110;
	characters.memory[ 18] = 0b00010;
	characters.memory[ 19] = 0b01100;

	characters.memory[ 20] = 0b01010;
	characters.memory[ 21] = 0b01010;
	characters.memory[ 22] = 0b01110;
	characters.memory[ 23] = 0b00010;
	characters.memory[ 24] = 0b00010;

	characters.memory[ 25] = 0b01110;
	characters.memory[ 26] = 0b01000;
	characters.memory[ 27] = 0b01100;
	characters.memory[ 28] = 0b00010;
	characters.memory[ 29] = 0b01100;

	characters.memory[ 30] = 0b01110;
	characters.memory[ 31] = 0b01000;
	characters.memory[ 32] = 0b01110;
	characters.memory[ 33] = 0b01010;
	characters.memory[ 34] = 0b01110;

	characters.memory[ 35] = 0b01110;
	characters.memory[ 36] = 0b00010;
	characters.memory[ 37] = 0b00010;
	characters.memory[ 38] = 0b00100;
	characters.memory[ 39] = 0b00100;

	characters.memory[ 40] = 0b01110;
	characters.memory[ 41] = 0b01010;
	characters.memory[ 42] = 0b01110;
	characters.memory[ 43] = 0b01010;
	characters.memory[ 44] = 0b01110;

	characters.memory[ 45] = 0b01110;
	characters.memory[ 46] = 0b01010;
	characters.memory[ 47] = 0b01110;
	characters.memory[ 48] = 0b00010;
	characters.memory[ 49] = 0b01110;

	characters.memory[ 50] = 0b00100;
	characters.memory[ 51] = 0b00100;
	characters.memory[ 52] = 0b00100;
	characters.memory[ 53] = 0b00000;
	characters.memory[ 54] = 0b00100;

	characters.memory[ 55] = 0b01010;
	characters.memory[ 56] = 0b11111;
	characters.memory[ 57] = 0b01010;
	characters.memory[ 58] = 0b11111;
	characters.memory[ 59] = 0b01010;

	characters.memory[ 60] = 0b01010;
	characters.memory[ 61] = 0b00010;
	characters.memory[ 62] = 0b00100;
	characters.memory[ 63] = 0b01000;
	characters.memory[ 64] = 0b01010;

	characters.memory[ 65] = 0b00010;
	characters.memory[ 66] = 0b00100;
	characters.memory[ 67] = 0b00100;
	characters.memory[ 68] = 0b00100;
	characters.memory[ 69] = 0b00010;

	characters.memory[ 70] = 0b01000;
	characters.memory[ 71] = 0b00100;
	characters.memory[ 72] = 0b00100;
	characters.memory[ 73] = 0b00100;
	characters.memory[ 74] = 0b01000;

	characters.memory[ 75] = 0b00100;
	characters.memory[ 76] = 0b00100;
	characters.memory[ 77] = 0b00000;
	characters.memory[ 78] = 0b00000;
	characters.memory[ 79] = 0b00000;

	characters.memory[ 80] = 0b00000;
	characters.memory[ 81] = 0b00000;
	characters.memory[ 82] = 0b01110;
	characters.memory[ 83] = 0b00000;
	characters.memory[ 84] = 0b00000;

	characters.memory[ 85] = 0b00110;
	characters.memory[ 86] = 0b00100;
	characters.memory[ 87] = 0b00100;
	characters.memory[ 88] = 0b00100;
	characters.memory[ 89] = 0b00110;

	characters.memory[ 90] = 0b01100;
	characters.memory[ 91] = 0b00100;
	characters.memory[ 92] = 0b00100;
	characters.memory[ 93] = 0b00100;
	characters.memory[ 94] = 0b01100;

	characters.memory[ 95] = 0b00100;
	characters.memory[ 96] = 0b01010;
	characters.memory[ 97] = 0b00000;
	characters.memory[ 98] = 0b00000;
	characters.memory[ 99] = 0b00000;

	characters.memory[100] = 0b00000;
	characters.memory[101] = 0b00000;
	characters.memory[102] = 0b00000;
	characters.memory[103] = 0b00000;
	characters.memory[104] = 0b01110;

	characters.memory[105] = 0b00010;
	characters.memory[106] = 0b00100;
	characters.memory[107] = 0b01100;
	characters.memory[108] = 0b00100;
	characters.memory[109] = 0b00010;

	characters.memory[110] = 0b01000;
	characters.memory[111] = 0b00100;
	characters.memory[112] = 0b00110;
	characters.memory[113] = 0b00100;
	characters.memory[114] = 0b01000;

	characters.memory[115] = 0b00000;
	characters.memory[116] = 0b00100;
	characters.memory[117] = 0b01110;
	characters.memory[118] = 0b00100;
	characters.memory[119] = 0b00000;

	characters.memory[120] = 0b00000;
	characters.memory[121] = 0b01110;
	characters.memory[122] = 0b00000;
	characters.memory[123] = 0b01110;
	characters.memory[124] = 0b00000;

	characters.memory[125] = 0b00000;
	characters.memory[126] = 0b00100;
	characters.memory[127] = 0b00010;
	characters.memory[128] = 0b00100;
	characters.memory[129] = 0b00000;

	characters.memory[130] = 0b00000;
	characters.memory[131] = 0b00100;
	characters.memory[132] = 0b01000;
	characters.memory[133] = 0b00100;
	characters.memory[134] = 0b00000;

	characters.memory[135] = 0b01000;
	characters.memory[136] = 0b00100;
	characters.memory[137] = 0b00100;
	characters.memory[138] = 0b00100;
	characters.memory[139] = 0b00010;

	characters.memory[140] = 0b00100;
	characters.memory[141] = 0b00100;
	characters.memory[142] = 0b00100;
	characters.memory[143] = 0b00100;
	characters.memory[144] = 0b00100;

	characters.memory[145] = 0b00010;
	characters.memory[146] = 0b00100;
	characters.memory[147] = 0b00100;
	characters.memory[148] = 0b00100;
	characters.memory[149] = 0b01000;

	characters.memory[150] = 0b00000;
	characters.memory[151] = 0b00100;
	characters.memory[152] = 0b00000;
	characters.memory[153] = 0b00100;
	characters.memory[154] = 0b00000;

	characters.memory[155] = 0b00000;
	characters.memory[156] = 0b00100;
	characters.memory[157] = 0b00000;
	characters.memory[158] = 0b00100;
	characters.memory[159] = 0b01000;

	characters.memory[160] = 0b00000;
	characters.memory[161] = 0b00100;
	characters.memory[162] = 0b00000;
	characters.memory[163] = 0b00000;
	characters.memory[164] = 0b00000;

	characters.memory[165] = 0b01100;
	characters.memory[166] = 0b10100;
	characters.memory[167] = 0b01111;
	characters.memory[168] = 0b10010;
	characters.memory[169] = 0b01101;

	characters.memory[170] = 0b01110;
	characters.memory[171] = 0b00010;
	characters.memory[172] = 0b00100;
	characters.memory[173] = 0b00000;
	characters.memory[174] = 0b00100;

	characters.memory[175] = 0b00100;
	characters.memory[176] = 0b01010;
	characters.memory[177] = 0b01110;
	characters.memory[178] = 0b01010;
	characters.memory[179] = 0b01010;

	characters.memory[180] = 0b01100;
	characters.memory[181] = 0b01010;
	characters.memory[182] = 0b01100;
	characters.memory[183] = 0b01010;
	characters.memory[184] = 0b01100;

	characters.memory[185] = 0b00100;
	characters.memory[186] = 0b01010;
	characters.memory[187] = 0b01000;
	characters.memory[188] = 0b01010;
	characters.memory[189] = 0b00100;

	characters.memory[190] = 0b01100;
	characters.memory[191] = 0b01010;
	characters.memory[192] = 0b01010;
	characters.memory[193] = 0b01010;
	characters.memory[194] = 0b01100;

	characters.memory[195] = 0b01110;
	characters.memory[196] = 0b01000;
	characters.memory[197] = 0b01110;
	characters.memory[198] = 0b01000;
	characters.memory[199] = 0b01110;

	characters.memory[200] = 0b01110;
	characters.memory[201] = 0b01000;
	characters.memory[202] = 0b01110;
	characters.memory[203] = 0b01000;
	characters.memory[204] = 0b01000;

	characters.memory[205] = 0b00100;
	characters.memory[206] = 0b01010;
	characters.memory[207] = 0b01000;
	characters.memory[208] = 0b01010;
	characters.memory[209] = 0b00110;

	characters.memory[210] = 0b01010;
	characters.memory[211] = 0b01010;
	characters.memory[212] = 0b01110;
	characters.memory[213] = 0b01010;
	characters.memory[214] = 0b01010;

	characters.memory[215] = 0b01110;
	characters.memory[216] = 0b00100;
	characters.memory[217] = 0b00100;
	characters.memory[218] = 0b00100;
	characters.memory[219] = 0b01110;

	characters.memory[220] = 0b01110;
	characters.memory[221] = 0b00010;
	characters.memory[222] = 0b00010;
	characters.memory[223] = 0b01010;
	characters.memory[224] = 0b00100;

	characters.memory[225] = 0b01010;
	characters.memory[226] = 0b01010;
	characters.memory[227] = 0b01100;
	characters.memory[228] = 0b01010;
	characters.memory[229] = 0b01010;

	characters.memory[230] = 0b01000;
	characters.memory[231] = 0b01000;
	characters.memory[232] = 0b01000;
	characters.memory[233] = 0b01000;
	characters.memory[234] = 0b01110;

	characters.memory[235] = 0b01010;
	characters.memory[236] = 0b01110;
	characters.memory[237] = 0b01010;
	characters.memory[238] = 0b01010;
	characters.memory[239] = 0b01010;

	characters.memory[240] = 0b01010;
	characters.memory[241] = 0b01010;
	characters.memory[242] = 0b01110;
	characters.memory[243] = 0b01110;
	characters.memory[244] = 0b01010;

	characters.memory[245] = 0b01110;
	characters.memory[246] = 0b01010;
	characters.memory[247] = 0b01010;
	characters.memory[248] = 0b01010;
	characters.memory[249] = 0b01110;

	characters.memory[250] = 0b01110;
	characters.memory[251] = 0b01010;
	characters.memory[252] = 0b01110;
	characters.memory[253] = 0b01000;
	characters.memory[254] = 0b01000;

	characters.memory[255] = 0b01110;
	characters.memory[256] = 0b01010;
	characters.memory[257] = 0b01010;
	characters.memory[258] = 0b01100;
	characters.memory[259] = 0b00010;

	characters.memory[260] = 0b01110;
	characters.memory[261] = 0b01010;
	characters.memory[262] = 0b01100;
	characters.memory[263] = 0b01010;
	characters.memory[264] = 0b01010;

	characters.memory[265] = 0b01110;
	characters.memory[266] = 0b01000;
	characters.memory[267] = 0b01110;
	characters.memory[268] = 0b00010;
	characters.memory[269] = 0b01110;

	characters.memory[270] = 0b01110;
	characters.memory[271] = 0b00100;
	characters.memory[272] = 0b00100;
	characters.memory[273] = 0b00100;
	characters.memory[274] = 0b00100;

	characters.memory[275] = 0b01010;
	characters.memory[276] = 0b01010;
	characters.memory[277] = 0b01010;
	characters.memory[278] = 0b01010;
	characters.memory[279] = 0b01110;

	characters.memory[280] = 0b01010;
	characters.memory[281] = 0b01010;
	characters.memory[282] = 0b01010;
	characters.memory[283] = 0b01010;
	characters.memory[284] = 0b00100;

	characters.memory[285] = 0b01010;
	characters.memory[286] = 0b01010;
	characters.memory[287] = 0b01010;
	characters.memory[288] = 0b01110;
	characters.memory[289] = 0b01010;

	characters.memory[290] = 0b01010;
	characters.memory[291] = 0b01010;
	characters.memory[292] = 0b00100;
	characters.memory[293] = 0b01010;
	characters.memory[294] = 0b01010;

	characters.memory[295] = 0b01010;
	characters.memory[296] = 0b01010;
	characters.memory[297] = 0b00100;
	characters.memory[298] = 0b00100;
	characters.memory[299] = 0b00100;

	characters.memory[300] = 0b01110;
	characters.memory[301] = 0b00010;
	characters.memory[302] = 0b00100;
	characters.memory[303] = 0b01000;
	characters.memory[304] = 0b01110;

	characters.memory[305] = 0b00000;
	characters.memory[306] = 0b00000;
	characters.memory[307] = 0b01010;
	characters.memory[308] = 0b00100;
	characters.memory[309] = 0b01010;
}

void GPU::executeThreads(byte opCode) {
	switch (functionBuffer.memory[opCode]) {
	case 1:
		byte xP0 = functionBuffer.memory[opCode + 1];
		byte xP1 = functionBuffer.memory[opCode + 2];
		byte yP0 = functionBuffer.memory[opCode + 3];
		byte yP1 = functionBuffer.memory[opCode + 4];
		u16 xP = xP0 + (xP1 * 256);
		u16 yP = yP0 + (yP1 * 256) + 1;
		screen.pixels[yP * 320 + xP] = int(255 << 16) | int(255 << 8) | int(255);
		
		break;
	}
	programCounter += 4;
}

void GPU::spawnThreads(byte function, u16 arg1, u16 arg2, u16 arg3) {
	switch (function) {
	case 1:
		byte l1 = characters.memory[arg1 * 5 + 0];
		byte l2 = characters.memory[arg1 * 5 + 1];
		byte l3 = characters.memory[arg1 * 5 + 2];
		byte l4 = characters.memory[arg1 * 5 + 3];
		byte l5 = characters.memory[arg1 * 5 + 4];
		byte rows[5] = { l1, l2, l3, l4, l5 };
		std::vector<u16> xPositions;
		std::vector<u16> yPositions;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				std::bitset<5> b1(rows[i]);		
				if (b1[4 - j] > 0) {
					xPositions.push_back(j);
					yPositions.push_back(i);				
				}
			}
		}
		for (int i = 0; i < xPositions.size(); i++) {
			programSize++;
			functionBuffer.memory[i * 5] = 1;
			u16 xPos = arg2 * 5 + xPositions[i];
			u16 yPos = arg3 * 5 + yPositions[i];
			if (xPos <= 255) {
				functionBuffer.memory[(i * 5) + 1] = xPos;
				functionBuffer.memory[(i * 5) + 2] = 0;

			}
			if (yPos <= 255) {
				functionBuffer.memory[(i * 5) + 3] = yPos;
				functionBuffer.memory[(i * 5) + 4] = 0;
			}
		}
		function = 0;
		break;
	}
}

void GPU::execute() {
	if (functionBuffer.Empty() == true) {
		switch (commandBuffer.memory[commandCounter]) {
		case 0:
			programSize = 0;	
			commandCounter = 0;
			functionBuffer.Clear();
			break;
		case 1:
			u16 arg1 = argBuffer.memory[commandCounter * 6 + 0] + (argBuffer.memory[commandCounter * 6 + 1] * 256);
			u16 arg2 = argBuffer.memory[commandCounter * 6 + 2] + (argBuffer.memory[commandCounter * 6 + 3] * 256);
			u16 arg3 = argBuffer.memory[commandCounter * 6 + 4] + (argBuffer.memory[commandCounter * 6 + 5] * 256);
			spawnThreads(commandBuffer.memory[commandCounter], arg1, arg2, arg3);
			commandCounter++;
			break;
		}
	}	
}

void GPU::tick() {
	for (int i = 0; i < 8; i++) {
		if (programCounter < programSize * 5) {
			executeThreads(programCounter);
			programCounter++;
		}else{
			programSize = 0;
			programCounter = 0;
			functionBuffer.Clear();
		}
	}	
}