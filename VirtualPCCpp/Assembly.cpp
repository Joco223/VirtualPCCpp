#include "Assembly.h"

#include <iostream>

namespace Assembly {

	void convertByte(int& number, byte& b1, byte& b2) {
		if (number > 255) {
			int temp = (number - 255) / 256;
			b1 = 255;
			b2 = temp;
		}else{
			b1 = number;
			b2 = 0;
		}
	}

	bool checkArgSize(int& arg, int argc, std::string& instruction, int& instructionC, int maxSize) {
		if (arg > maxSize) {
			std::cout << "Error: Argument " << argc << " exceeds size limit at instruction " << instruction << " at position " << instructionC + 1 + argc << '\n';
			return true;
		}
	}

	void Assembly::Compile(std::vector<std::string>& code, CPU& cpu) {
		std::vector<char> charactersA = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', '-', ',', '.', '/', '\\', ':', ';', '<', '>', '=', '?', '[', ']', '{', '}', '`', '^', '|', '~', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', ' '};

		int currentPos = 0;
		int gpuCommand = 0;
		int wordsize = 0;

		for (int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			bool has_only_digits = (instruction.find_first_not_of("0123456789")) == std::string::npos;

			if (instruction == "LOAD0_C") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 1;
				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD1_C") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 2;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD0_C") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 3;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD1_R") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 4;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "CLR0") {
				cpu.cache.memory[currentPos] = 4;
			}else if (instruction == "CLR1") {
				cpu.cache.memory[currentPos] = 5;
			}else if (instruction == "WRT0_C") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 7;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT1_C") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 8;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT0_R") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 9;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT1_R") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 10;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRTC_R") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				cpu.cache.memory[currentPos] = 11;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				currentPos += 2;
			}else if (instruction == "WRTR_C") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				cpu.cache.memory[currentPos] = 12;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				currentPos += 2;
			}else if (instruction == "SUM") {
				cpu.cache.memory[currentPos] = 20;
			}else if (instruction == "SUB") {
				cpu.cache.memory[currentPos] = 21;
			}else if (instruction == "MLT") {
				cpu.cache.memory[currentPos] = 22;
			}else if (instruction == "DIV") {
				cpu.cache.memory[currentPos] = 23;
			}else if (instruction == "REG0_B") {
				cpu.cache.memory[currentPos] = 24;
			}else if (instruction == "REG0_BE") {
				cpu.cache.memory[currentPos] = 25;
			}else if (instruction == "REG1_B") {
				cpu.cache.memory[currentPos] = 26;
			}else if (instruction == "REG1_BE") {
				cpu.cache.memory[currentPos] = 27;
			}else if (instruction == "REG_EQL") {
				cpu.cache.memory[currentPos] = 28;
			}else if (instruction == "REG_DIF") {
				cpu.cache.memory[currentPos] = 29;
			}else if (instruction == "JMP") {
				std::string arg1 = code[i + 1];

				cpu.cache.memory[currentPos] = 30;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "CMP") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				cpu.cache.memory[currentPos] = 31;

				int Arg1 =  std::stoi(arg1);

				cpu.cache.memory[currentPos + 1] = Arg1;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg2, b1, b2);
				cpu.cache.memory[currentPos + 2] = b1;
				cpu.cache.memory[currentPos + 3] = b2;

				currentPos += 2;
			}else if (instruction == "COT0") {
				cpu.cache.memory[currentPos] = 40;
			}else if (instruction == "COT1") {
				cpu.cache.memory[currentPos] = 41;
			}else if (instruction == "WRT_R_VR") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];

				cpu.cache.memory[currentPos] = 50;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 65535) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos + 5] = b5;
				cpu.cache.memory[currentPos + 6] = b6;

				currentPos += 3;
			}else if (instruction == "WRT_VR_R") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];

				cpu.cache.memory[currentPos] = 51;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 65535) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos + 5] = b5;
				cpu.cache.memory[currentPos + 6] = b6;

				currentPos += 3;
			}else if (instruction == "DRAWP") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];
				std::string arg4 = code[i + 4];
				std::string arg5 = code[i + 5];
				std::string arg6 = code[i + 6];
				std::string arg7 = code[i + 7];

				cpu.cache.memory[currentPos] = 52;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 65535) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos + 5] = b5;
				cpu.cache.memory[currentPos + 6] = b6;

				int Arg4 = std::stoi(arg4);

				if(checkArgSize(Arg4, 4, instruction, i, 65535) == true) { break; }

				byte b7, b8;
				convertByte(Arg4, b7, b8);
				cpu.cache.memory[currentPos + 7] = b7;
				cpu.cache.memory[currentPos + 8] = b8;

				int Arg5 = std::stoi(arg5);

				if(checkArgSize(Arg5, 5, instruction, i, 65535) == true) { break; }

				cpu.cache.memory[currentPos + 9] = Arg5;

				int Arg6 = std::stoi(arg6);

				if(checkArgSize(Arg6, 6, instruction, i, 65535) == true) { break; }

				cpu.cache.memory[currentPos + 10] = Arg6;

				int Arg7 = std::stoi(arg7);

				if(checkArgSize(Arg7, 7, instruction, i, 65535) == true) { break; }

				cpu.cache.memory[currentPos + 11] = Arg7;

				currentPos += 7;
			}else if (instruction == "PRINT") {
				cpu.cache.memory[currentPos + wordsize] = 53;

				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];
				std::string arg4 = code[i + 4];
				std::string arg5 = code[i + 5];
				std::string arg6 = code[i + 6];

				int size = arg1.size();
				int Arg2 = std::stoi(arg2);
				int Arg3 = std::stoi(arg3);
				int Arg4 = std::stoi(arg4);
				int Arg5 = std::stoi(arg5);
				int Arg6 = std::stoi(arg6);

				cpu.cache.memory[currentPos + wordsize + 1] = size;
				cpu.cache.memory[currentPos + wordsize + 2] = Arg2;
				cpu.cache.memory[currentPos + wordsize + 3] = Arg3;
				cpu.cache.memory[currentPos + wordsize + 4] = Arg4;
				cpu.cache.memory[currentPos + wordsize + 5] = Arg5;
				cpu.cache.memory[currentPos + wordsize + 6] = Arg6;

				for (int j = 0; j < size; j++) {
					byte cC;
					for (int k = 0; k < charactersA.size(); k++) {
						if (arg1[j] == charactersA[k]) {
							cC = k;
							break;
						}
					}
					cpu.cache.memory[currentPos + wordsize + 7 + j] = cC;
				}

				wordsize += size;
			}
			currentPos++;
		}
	}
}