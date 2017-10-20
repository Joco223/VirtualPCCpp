#include "Assembly.h"

namespace Assembly {

	std::vector<std::string> lines;

	void convertByte(int& number, byte& b1, byte& b2, byte& b3) {
		b1 = number & 0xff;
		b2 = (byte)(number >> 8);
		b3 = (byte)(number >> 16);
	}

	bool checkArgSize(int& arg, int argc, std::string& instruction, int& instructionC, int maxSize) {
		if (arg > maxSize) {
			std::cout << "Error: Argument " << argc << " exceeds size limit of " << maxSize << " at instruction " << instruction << " at position " << instructionC + 1 + argc << '\n';
			return true;
		}else{
			return false;
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
				if (!(line[0] == '/' && line[1] == '/')) {
					if (line != "") {
						lines.push_back(line);
						std::string buf;
						std::stringstream ss(line);

						while (ss >> buf) {
							if (buf[0] == '/' && buf[1] == '/') {break;}
							code.push_back(buf);
						}
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
		int line = 0;
		std::vector<u16> jumpPositions;

		for (int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if (instruction == "LOAD0_8") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 6;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "LOAD1_8") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 7;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "LOAD0_16") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 4;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "LOAD1_16") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 5;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "LOAD0_24") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 1;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "LOAD1_24") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 2;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "CLR0") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 4;
				currentPos++;
				line++;
			}else if (instruction == "CLR1") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 5;
				line++;
			}else if (instruction == "WRT0_24") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 8;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRT1_24") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 9;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRT0_16") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 10;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRT1_16") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 11;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRT0_8") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 12;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRT1_8") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 13;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg1, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;

				i++;
				line++;
			}else if (instruction == "WRTR_H") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 16;
				line++;
			}else if (instruction == "WRTH_R") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 17;
				line++;
			}else if (instruction == "SUM") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 20;
				line++;
			}else if (instruction == "SUB") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 21;
				line++;
			}else if (instruction == "MLT") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 22;
				line++;
			}else if (instruction == "DIV") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 23;
				line++;
			}else if (instruction == "REG0_B") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 24;
				line++;
			}else if (instruction == "REG0_BE") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 25;
				line++;
			}else if (instruction == "REG1_B") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 26;
				line++;
			}else if (instruction == "REG1_BE") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 27;
				line++;
			}else if (instruction == "REG_EQL") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 28;
				line++;
			}else if (instruction == "REG_DIF") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 29;
				line++;
			}else if (instruction == "JMP") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 30;
				line++;
			}else if (instruction == "CMP") {
				std::string arg1 = code[i + 1];

				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 31;

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 1) == true) { break; }

				cpu.ram.memory[currentPos++] = Arg1;

				i++;
				line++;
			}else if (instruction == "COT0") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 40;
				line++;
			}else if (instruction == "COT1") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 41;
				line++;
			}else if (instruction == "DRAWP") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 52;
				line++;
			}else if (instruction == "PRINT") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 53;
				line++;
			}else if (instruction == "CLR_COMM") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 55;
				line++;
			}else if (instruction == "CLR_VRAM") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 56;
				line++;
			}else if (instruction == "WAIT") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 32;

				std::string arg1 = code[i + 1];

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 255) == true) { break; }

				cpu.ram.memory[currentPos++] = Arg1;

				i++;
				line++;
			}else if (instruction == "CHK_K") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 42;

				std::string arg1 = code[i + 1];

				int Arg1;
				checkArgType(Arg1, arg1, cpu);

				if(checkArgSize(Arg1, 1, instruction, i, 255) == true) { break; }

				cpu.ram.memory[currentPos++] = Arg1;

				i++;
				line++;
			}else if(instruction == "STOP") {
				jumpPositions.push_back(currentPos);
				cpu.ram.memory[currentPos++] = 18;
				line++;
			}else if (instruction == "SET8") {
				std::string arg2 = code[i + 1];
				std::string arg3 = code[i + 2];

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 16777215) == true) { break; }

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 16777215) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg3, b1, b2, b3);
				cpu.ram.memory[Arg2] = b1;

				i += 2;
				line++;
			}else if (instruction == "SET16") {
				std::string arg2 = code[i + 1];
				std::string arg3 = code[i + 2];

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 16777215) == true) { break; }

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 16777215) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg3, b1, b2, b3);
				cpu.ram.memory[Arg2] = b1;
				cpu.ram.memory[Arg2 + 1] = b2;

				i += 2;
				line++;
			}else if (instruction == "SET24") {
				std::string arg2 = code[i + 1];
				std::string arg3 = code[i + 2];

				int Arg2 = std::stoi(arg2);

				if(checkArgSize(Arg2, 2, instruction, i, 16777215) == true) { break; }

				int Arg3 = std::stoi(arg3);

				if(checkArgSize(Arg3, 3, instruction, i, 16777215) == true) { break; }

				byte b1, b2, b3;
				convertByte(Arg3, b1, b2, b3);
				cpu.ram.memory[Arg2] = b1;
				cpu.ram.memory[Arg2 + 1] = b2;
				cpu.ram.memory[Arg2 + 3] = b3;

				i += 2;
				line++;
			}else{
				std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
				std::cout << '\n';
				std::cout << lines[line] << '\n';
				std::cout << "^" << '\n';
				std::cout << '\n';
				std::cout << "Compilation terminated" << '\n';
				cpu.ram.Clear();
				break;
			}
		}

		bool hasStop = false;

		for (int i = 0; i < code.size(); i++) {
			if (code[i] == "STOP") {
				hasStop = true;
				break;
			}
		}

		if (hasStop == false) {
			std::cout << "Compilation terminated, no STOP instruction found." << '\n';
			cpu.ram.Clear();
		}

		std::cout << "Program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}
}