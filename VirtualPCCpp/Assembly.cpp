#include "Assembly.h"

namespace Assembly {

	void convertByte(int& number, byte& b1, byte& b2) {
		if (number > 255) {
			b1 = number & 0xff;
			b2 = (byte)(number >> 8);
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

	void checkArgType(int& arg, std::string& argument, CPU& cpu) {
		if (argument == "REG0") {
			arg = cpu.register0;
		}else if (argument == "REG1") {
			arg = cpu.register1;
		}else{
			arg = std::stoi(argument);
		}
	}

	void readFile(std::string path, std::vector<std::string>& code) {
		std::string line;
		std::ifstream myfile(path);
		std::string prefix = "//";

		if (myfile.is_open()){
			while (getline (myfile,line)) {
				if (!equal(prefix.begin(), prefix.end(), line.begin())) {
					std::string buf;
					std::stringstream ss(line);

					while (ss >> buf) {
						code.push_back(buf);
					}
				}
			}
		}
	}

	void Compile(std::vector<std::string>& code, CPU& cpu) {
		std::vector<char> charactersA = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', '-', ',', '.', '/', '\\', ':', ';', '<', '>', '=', '?', '[', ']', '{', '}', '`', '^', '|', '~', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', ' ', '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

		int currentPos = 0;
		int gpuCommand = 0;
		int additionalMemory = 0;
		std::vector<u16> jumpPositions;

		for (int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			bool has_only_digits = (instruction.find_first_not_of("0123456789")) == std::string::npos;

			if (instruction == "LOAD0_C") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 1;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "LOAD1_C") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 2;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "LOAD0_R") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 3;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "LOAD1_R") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 4;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "CLR0") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 4;
				currentPos++;
			}else if (instruction == "CLR1") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 5;
			}else if (instruction == "WRT0_C") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 7;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "WRT1_C") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 8;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "WRT0_R") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 9;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "WRT1_R") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 10;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "WRTC_R") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 11;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos++] = b3;
				cpu.cache.memory[currentPos++] = b4;

				i += 2;
			}else if (instruction == "WRTR_C") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 12;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos++] = b3;
				cpu.cache.memory[currentPos++] = b4;

				i += 2;
			}else if (instruction == "SUM") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 20;
			}else if (instruction == "SUB") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 21;
			}else if (instruction == "MLT") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 22;
			}else if (instruction == "DIV") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 23;
			}else if (instruction == "REG0_B") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 24;
			}else if (instruction == "REG0_BE") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 25;
			}else if (instruction == "REG1_B") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 26;
			}else if (instruction == "REG1_BE") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 27;
			}else if (instruction == "REG_EQL") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 28;
			}else if (instruction == "REG_DIF") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 29;
			}else if (instruction == "JMP") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 30;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				int pos = jumpPositions[Arg1];
				convertByte(pos, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i++;
			}else if (instruction == "CMP") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 31;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				cpu.cache.memory[currentPos++] = Arg1;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);
				int pos = jumpPositions[Arg2];

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(pos, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				i += 2;
			}else if (instruction == "COT0") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 40;
			}else if (instruction == "COT1") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 41;
			}else if (instruction == "WRT_R_VR") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 50;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos++] = b3;
				cpu.cache.memory[currentPos++] = b4;

				int Arg3;
				checkArgType(Arg3, arg3, cpu);

				if(checkArgSize(Arg3, 3, instruction, i, 65535) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos++] = b5;
				cpu.cache.memory[currentPos++] = b6;

				i += 3;
			}else if (instruction == "WRT_VR_R") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 51;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				if(checkArgSize(Arg2, 2, instruction, i, 65535) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos++] = b3;
				cpu.cache.memory[currentPos++] = b4;

				int Arg3;
				checkArgType(Arg3, arg3, cpu);

				if(checkArgSize(Arg3, 3, instruction, i, 65535) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos++] = b5;
				cpu.cache.memory[currentPos++] = b6;

				i += 3;
			}else if (instruction == "DRAWP") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];
				std::string arg4 = code[i + 4];
				std::string arg5 = code[i + 5];
				std::string arg6 = code[i + 6];
				std::string arg7 = code[i + 7];

				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 52;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 320) == true) { break; }

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				if(checkArgSize(Arg2, 2, instruction, i, 240) == true) { break; }

				byte b3, b4;
				convertByte(Arg2, b3, b4);
				cpu.cache.memory[currentPos++] = b3;
				cpu.cache.memory[currentPos++] = b4;

				int Arg3;
				checkArgType(Arg3, arg3, cpu);

				if(checkArgSize(Arg3, 3, instruction, i, 320) == true) { break; }

				byte b5, b6;
				convertByte(Arg3, b5, b6);
				cpu.cache.memory[currentPos++] = b5;
				cpu.cache.memory[currentPos++] = b6;

				int Arg4;
				checkArgType(Arg4, arg4, cpu);

				if(checkArgSize(Arg4, 4, instruction, i, 240) == true) { break; }

				byte b7, b8;
				convertByte(Arg4, b7, b8);
				cpu.cache.memory[currentPos++] = b7;
				cpu.cache.memory[currentPos++] = b8;

				int Arg5;
				checkArgType(Arg5, arg5, cpu);

				if(checkArgSize(Arg5, 5, instruction, i, 63) == true) { break; }

				cpu.cache.memory[currentPos++] = Arg5;

				int Arg6;
				checkArgType(Arg6, arg6, cpu);

				if(checkArgSize(Arg6, 6, instruction, i, 63) == true) { break; }

				cpu.cache.memory[currentPos++] = Arg6;

				int Arg7;
				checkArgType(Arg7, arg7, cpu);

				if(checkArgSize(Arg7, 7, instruction, i, 63) == true) { break; }

				cpu.cache.memory[currentPos++] = Arg7;

				i += 7;
			}else if (instruction == "PRINT") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 53;

				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];
				std::string arg4 = code[i + 4];
				std::string arg5 = code[i + 5];
				std::string arg6 = code[i + 6];

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				int Arg2;
				checkArgType(Arg2, arg2, cpu);

				int Arg3;
				checkArgType(Arg3, arg3, cpu);

				int Arg4;
				checkArgType(Arg4, arg4, cpu);

				int Arg5;
				checkArgType(Arg5, arg5, cpu);

				int Arg6;
				checkArgType(Arg6, arg6, cpu);

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;

				cpu.cache.memory[currentPos++] = Arg2;
				cpu.cache.memory[currentPos++] = Arg3;
				cpu.cache.memory[currentPos++] = Arg4;
				cpu.cache.memory[currentPos++] = Arg5;
				cpu.cache.memory[currentPos++] = Arg6;

				i += 6;
			}else if (instruction == "CLR_COMM") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 55;
				cpu.firstAvailable++;
			}else if (instruction == "CLR_VRAM") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 56;
				cpu.firstAvailable++;
			}else if (instruction == "WAIT") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 32;

				std::string arg1 = code[i + 1];

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				cpu.cache.memory[currentPos++] = Arg1;

				i++;
			}else if (instruction == "CHK_K") {
				jumpPositions.push_back(currentPos);
				cpu.cache.memory[currentPos++] = 42;

				std::string arg1 = code[i + 1];

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				byte b1, b2;
				convertByte(Arg1, b1, b2);
				cpu.cache.memory[currentPos++] = b1;
				cpu.cache.memory[currentPos++] = b2;
				i++;
			}else if (instruction == "SET") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];
				std::string arg3 = code[i + 3];

				int Arg2 = std::stoi(arg2);

				int Arg3 = std::stoi(arg3);

				if (arg1 == "C") {
					cpu.cache.memory[Arg2] = Arg3;
				}else if (arg1 == "R") {
					cpu.ram.memory[Arg2] = Arg3;
				}

				i += 3;
			}
		}
	}
}