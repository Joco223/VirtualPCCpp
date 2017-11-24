#include "Assembly.h"

namespace Assembly {

	std::vector<std::string> lines;

	void convertByte(int& number, byte& b1, byte& b2, byte& b3) {
		b1 = number & 0xff;
		b2 = (byte)(number >> 8);
		b3 = (byte)(number >> 16);
	}

	bool checkArgSize(int& arg, int argc, std::string& instruction, unsigned int& instructionC, int maxSize) {
		if (arg > maxSize) {
			std::cout << "Error: Argument " << argc << " exceeds size limit of " << maxSize << " at instruction " << instruction << " at position " << instructionC + 1 + argc << '\n';
			return true;
		}else{
			return false;
		}
	}

	void checkArgType(int& arg, std::string& argument, CPU& cpu, std::vector<std::string>& vars, std::vector<int>& varValues) {
		bool varExists = false;
		int id = 0;

		for (unsigned int i = 0; i < vars.size(); i++) {
			if (argument == vars[i]) {
				varExists = true;
				id = i;
				break;
			}
		}

		if (varExists == true) {
			arg = varValues[id];
		}else if (varExists == false) {
			if (argument == "REG0") {
				arg = cpu.register0;
			}else if (argument == "REG1") {
				arg = cpu.register1;
			}else{
				arg = std::stoi(argument);
			}
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
		int additionalMemory = 0;
		int line = 0;
		std::vector<int> jumpPositions;
		std::vector<std::string> vars;
		std::vector<int> varValues;

		bool dataM = false;
		bool interM = false;
		bool hasStop = false;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if(instruction == "data_s") {
				dataM = true;
				i++;
				instruction = code[i];
			}

			if(instruction == "data_e") {
				dataM = false;
				currentPos += additionalMemory;
				cpu.programCounter = currentPos;
				i++;
				instruction = code[i];
			}

			if (instruction == "inter_s") {
				interM = true;
				cpu.interPos = currentPos;
				i++;
				instruction = code[i];
			}

			if (instruction == "inter_e") {
				interM = false;
				cpu.ram.memory[currentPos++] = 30;
				cpu.interTartgetPos = currentPos;
				cpu.ram.memory[currentPos++] = 0;
				cpu.ram.memory[currentPos++] = 0;
				cpu.programCounter = currentPos;
				i++;
				instruction = code[i];
			}

			if (dataM == true && interM == false) {
				if (instruction == "u8") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					cpu.ram.memory[additionalMemory++] = (byte)Arg2;
					i += 2;
				}else if (instruction == "u16") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					byte b1, b2, b3;
					convertByte(Arg2, b1, b2, b3);
					cpu.ram.memory[additionalMemory++] = b1;
					cpu.ram.memory[additionalMemory++] = b2;
					i += 2;
				}else if (instruction == "u24") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					byte b1, b2, b3;
					convertByte(Arg2, b1, b2, b3);
					cpu.ram.memory[additionalMemory++] = b1;
					cpu.ram.memory[additionalMemory++] = b2;
					cpu.ram.memory[additionalMemory++] = b3;
					i += 2;
				}else{
					std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
					std::cout << '\n';
					std::cout << lines[line] << '\n';
					std::cout << "^" << '\n';
					std::cout << '\n';
					std::cout << "Compilation terminated." << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (dataM == false) {
				if (instruction == "load0_8") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 6;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_8") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 7;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load0_16") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 4;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_16") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 5;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load0_24") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 1;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_24") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 2;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "clr0") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 4;
					currentPos++;
					line++;
				}else if (instruction == "clr1") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 5;
					line++;
				}else if (instruction == "wrt0_24") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 8;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt1_24") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 9;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt0_16") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 10;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt1_16") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 11;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt0_8") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 12;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt1_8") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 13;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrtR_H") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 16;
					line++;
				}else if (instruction == "wrtH_R") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 17;
					line++;
				}else if (instruction == "sum") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 20;
					line++;
				}else if (instruction == "sub") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 21;
					line++;
				}else if (instruction == "mlt") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 22;
					line++;
				}else if (instruction == "div") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 23;
					line++;
				}else if (instruction == "reg0_b") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 24;
					line++;
				}else if (instruction == "reg0_be") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 25;
					line++;
				}else if (instruction == "reg1_b") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 26;
					line++;
				}else if (instruction == "reg1_be") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 27;
					line++;
				}else if (instruction == "reg_eql") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 28;
					line++;
				}else if (instruction == "reg_dif") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 29;
					line++;
				}else if (instruction == "jmp") {
					jumpPositions.push_back(currentPos);

					std::string arg1 = code[i + 1];
					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);
					
					cpu.ram.memory[currentPos++] = 30;

					int pos = (cpu.ram.memory[Arg1 + 1] << 8 | cpu.ram.memory[Arg1]);

					byte b1, b2, b3;
					convertByte(jumpPositions[pos], b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "cmp") {
					std::string arg1 = code[i + 1];

					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 31;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 1) == true) { break; }

					cpu.ram.memory[currentPos++] = (byte)Arg1;

					i++;
					line++;
				}else if (instruction == "cot0") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 40;
					line++;
				}else if (instruction == "cot1") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 41;
					line++;
				}else if (instruction == "drawp") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 52;
					line++;
				}else if (instruction == "cs_out0") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 52;

					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					cpu.ram.memory[currentPos++] = (byte)Arg1;
					cpu.ram.memory[currentPos++] = (byte)Arg2;

					i += 2;
					line++;
				}else if (instruction == "cs_out1") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 53;

					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					cpu.ram.memory[currentPos++] = (byte)Arg1;
					cpu.ram.memory[currentPos++] = (byte)Arg2;

					i += 2;
					line++;
				}else if (instruction == "clr_comm") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 55;
					line++;
				}else if (instruction == "clr_vram") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 56;
					line++;
				}else if (instruction == "wait") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 32;

					std::string arg1 = code[i + 1];

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 255) == true) { break; }

					cpu.ram.memory[currentPos++] = (byte)Arg1;

					i++;
					line++;
				}else if (instruction == "chk_k") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 42;

					std::string arg1 = code[i + 1];

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 255) == true) { break; }

					cpu.ram.memory[currentPos++] = (byte)Arg1;

					i++;
					line++;
				}else if(instruction == "stop") {
					jumpPositions.push_back(currentPos);
					cpu.ram.memory[currentPos++] = 18;
					hasStop = true;
					line++;
				}else{
					std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
					std::cout << '\n';
					std::cout << lines[line] << '\n';
					std::cout << "^" << '\n';
					std::cout << '\n';
					std::cout << "Compilation terminated." << '\n';
					cpu.ram.Clear();
					break;
				}
			}

		}

		if (hasStop == false) {
			std::cout << "Compilation terminated, no > stop < instruction found." << '\n';
			cpu.ram.Clear();
		}


		std::cout << "Program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}
}