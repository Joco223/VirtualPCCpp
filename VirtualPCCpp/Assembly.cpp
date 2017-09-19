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

	bool checkArgSize(int& arg, int argc, std::string& instruction, int& instructionC) {
		if (arg > 65535) {
			std::cout << "Error: Argument " << argc << " exceeds size limit at instruction " << instruction << " at position " << instructionC + 1 + argc << '\n';
			return true;
		}
	}

	void Assembly::Compile(std::vector<std::string>& code, CPU& cpu) {
		int currentPos = 0;
		int gpuCommand = 0;
		for (int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];
			std::string arg1 = code[i + 1];
			std::string arg2 = code[i + 2];
			std::string arg3 = code[i + 3];

			bool has_only_digits = (instruction.find_first_not_of( "0123456789" )) == std::string::npos;

			if (instruction == "LOAD0_C") {
				cpu.cache.memory[currentPos] = 1;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD1_C") {
				cpu.cache.memory[currentPos] = 2;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD0_C") {
				cpu.cache.memory[currentPos] = 3;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "LOAD1_R") {
				cpu.cache.memory[currentPos] = 4;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

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
				cpu.cache.memory[currentPos] = 7;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT1_C") {
				cpu.cache.memory[currentPos] = 8;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT0_R") {
				cpu.cache.memory[currentPos] = 9;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRT1_R") {
				cpu.cache.memory[currentPos] = 10;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "WRTC_R") {
				cpu.cache.memory[currentPos] = 11;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos + 3] = b3;
				cpu.cache.memory[currentPos + 4] = b4;

				currentPos += 2;
			}else if (instruction == "WRTR_C") {
				cpu.cache.memory[currentPos] = 12;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i) == true) { break; }

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
				cpu.cache.memory[currentPos] = 30;

				int Arg1 = std::stoi(arg1);

				if(checkArgSize(Arg1, 1, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos + 1] = b1;
				cpu.cache.memory[currentPos + 2] = b2;

				currentPos++;
			}else if (instruction == "CMP") {
				cpu.cache.memory[currentPos] = 31;

				int Arg1 =  std::stoi(arg1);

				cpu.cache.memory[currentPos + 1] = Arg1;

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i) == true) { break; }

				byte b1, b2;
				convertByte(Arg2, b1, b2);
				cpu.cache.memory[currentPos + 2] = b1;
				cpu.cache.memory[currentPos + 3] = b2;

				currentPos += 2;
			}else if (instruction == "COT0") {
				cpu.cache.memory[currentPos] = 40;
			}else if (instruction == "COT1") {
				cpu.cache.memory[currentPos] = 41;
			}else if(has_only_digits == false){
				std::cout << "Error: Invalid instruction " << instruction << " at position " << i + 1 << '\n';
				cpu.cache.Clear();
				break;	
			}
			currentPos++;
		}
	}
}