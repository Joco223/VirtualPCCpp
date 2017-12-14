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

	void checkJmpPos(int& arg, std::string argument, std::vector<std::string>& jumpNames, std::vector<int> jumpPos) {
		bool varExists = false;
		int id = 0;

		for (unsigned int i = 0; i < jumpNames.size(); i++) {
			if (argument == jumpNames[i]) {
				varExists = true;
				id = i;
				break;
			}
		}

		if (varExists == true) {
			arg = jumpPos[id];
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
				arg = cpu.ram.memory.size() + 1;
			}else if (argument == "REG1") {
				arg = cpu.ram.memory.size() + 2;
			}else if (argument == "REG_INT") {
				arg = cpu.ram.memory.size() + 3;
			}else {
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
		std::vector<std::string> jumpNames;
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
				i++;
				instruction = code[i];
			}

			if (instruction == "inter_s") {
				interM = true;
				i++;
				instruction = code[i];
			}

			if (instruction == "inter_e") {
				interM = false;
				currentPos += 3;
				i++;
				instruction = code[i];
			}

			if (dataM == true && interM == false) {
				if (instruction == "u8") {
					additionalMemory++;
					i += 2;
				}else if (instruction == "u16") {
					additionalMemory += 2;
					i += 2;
				}else if (instruction == "u24") {
					additionalMemory += 3;
					i += 2;
				}else if(instruction == "u8a") {
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					for (int j = 0; j < Arg2; j++) {
						additionalMemory++;
					}

					i += (2 + Arg2);
				}else if(instruction == "u16a") {
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					for (int j = 0; j < Arg2; j++) {
						additionalMemory += 2;
					}

					i += 2 + Arg2;
				}else if(instruction == "u24a") {
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					for (int j = 0; j < Arg2; j++) {
						additionalMemory += 3;
					}

					i += 2 + Arg2;
				}else {
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
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load1_8") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load0_16") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load1_16") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load0_24") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load1_24") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load0_8a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "load1_8a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "load0_16a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "load1_16a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "load0_24a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "load1_24a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "clr0") {
					currentPos++;
					line++;
				}else if (instruction == "clr1") {
					currentPos++;
					line++;
				}else if (instruction == "wrt0_24") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt1_24") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt0_16") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt1_16") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt0_8") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt1_8") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "wrt0_24a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrt1_24a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrt0_16a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrt1_16a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrt0_8a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrt1_8a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "wrtR_H") {
					currentPos++;
					line++;
				}else if (instruction == "wrtH_R") {
					currentPos++;
					line++;
				}else if (instruction == "sum") {
					currentPos++;
					line++;
				}else if (instruction == "sub") {
					currentPos++;
					line++;
				}else if (instruction == "mlt") {
					currentPos++;
					line++;
				}else if (instruction == "div") {
					currentPos++;
					line++;
				}else if (instruction == "reg0_b") {
					currentPos++;
					line++;
				}else if (instruction == "reg0_be") {
					currentPos++;
					line++;
				}else if (instruction == "reg1_b") {
					currentPos++;
					line++;
				}else if (instruction == "reg1_be") {
					currentPos++;
					line++;
				}else if (instruction == "reg_eql") {
					currentPos++;
					line++;
				}else if (instruction == "reg_dif") {
					currentPos++;
					line++;
				}else if (instruction == "jmp") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "cmp") {
					currentPos += 4;
					i += 2;
					line++;
				}else if (instruction == "cmp_a") {
					currentPos += 5;
					i += 2;
					line++;
				}else if (instruction == "cot0") {
					currentPos++;
					line++;
				}else if (instruction == "cot1") {
					currentPos++;
					line++;
				}else if (instruction == "cs_out0") {
					currentPos += 3;
					i += 2;
					line++;
				}else if (instruction == "cs_out1") {
					currentPos += 3;
					i += 2;
					line++;
				}else if (instruction == "clr_comm") {
					currentPos++;
					line++;
				}else if (instruction == "clr_vram") {
					currentPos++;
					line++;
				}else if (instruction == "wait") {
					currentPos += 2;
					i++;
					line++;
				}else if(instruction == "stop") {
					currentPos++;
					line++;
				}else{
					if (instruction.back() == ':') {
						std::string tmp = instruction;
						tmp.erase(tmp.end() - 1);
						jumpNames.push_back(tmp);
						jumpPositions.push_back(currentPos);
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

		}

		currentPos = 0;
		additionalMemory = 0;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if(instruction == "data_s") {
				dataM = true;
				i++;
				line++;
				instruction = code[i];
			}

			if(instruction == "data_e") {
				dataM = false;
				currentPos += additionalMemory;
				cpu.programCounter = currentPos;
				i++;
				line++;
				instruction = code[i];
			}

			if (instruction == "inter_s") {
				interM = true;
				cpu.interPos = currentPos;
				i++;
				line++;
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
				line++;
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
					line++;
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
					line++;
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
					line++;
				}else if(instruction == "u8a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					for (int j = 0; j < Arg2; j++) {
						int element;
						checkArgType(element, code[i + 3 + j], cpu, vars, varValues);
						cpu.ram.memory[additionalMemory++] = (byte)element;
					}

					i += (2 + Arg2);
					line++;
				}else if(instruction == "u16a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					for (int j = 0; j < Arg2; j++) {
						int element;
						checkArgType(element, code[i + 3 + j], cpu, vars, varValues);
						byte b1, b2, b3;
						convertByte(element, b1, b2, b3);
						cpu.ram.memory[additionalMemory++] = b1;
						cpu.ram.memory[additionalMemory++] = b2;
					}

					i += 2 + Arg2;
					line++;
				}else if(instruction == "u24a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					for (int j = 0; j < Arg2; j++) {
						int element;
						checkArgType(element, code[i + 3 + j], cpu, vars, varValues);
						byte b1, b2, b3;
						convertByte(element, b1, b2, b3);
						cpu.ram.memory[additionalMemory++] = b1;
						cpu.ram.memory[additionalMemory++] = b2;
						cpu.ram.memory[additionalMemory++] = b3;
					}

					i += 2 + Arg2;
					line++;
				}else {
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
				}else if (instruction == "load0_8a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 46;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "load1_8a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 47;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "load0_16a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 44;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "load1_16a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 45;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "load0_24a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 42;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "load1_24a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 43;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "clr0") {
					cpu.ram.memory[currentPos++] = 4;
					line++;
				}else if (instruction == "clr1") {
					cpu.ram.memory[currentPos++] = 5;
					line++;
				}else if (instruction == "wrt0_24") {
					std::string arg1 = code[i + 1];

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
				}else if (instruction == "wrt0_24a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 48;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrt1_24a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 49;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrt0_16a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 50;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrt1_16a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 51;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrt0_8a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 52;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrt1_8a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 53;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "wrtR_H") {
					cpu.ram.memory[currentPos++] = 16;
					line++;
				}else if (instruction == "wrtH_R") {
					cpu.ram.memory[currentPos++] = 17;
					line++;
				}else if (instruction == "sum") {
					cpu.ram.memory[currentPos++] = 20;
					line++;
				}else if (instruction == "sub") {
					cpu.ram.memory[currentPos++] = 21;
					line++;
				}else if (instruction == "mlt") {
					cpu.ram.memory[currentPos++] = 22;
					line++;
				}else if (instruction == "div") {
					cpu.ram.memory[currentPos++] = 23;
					line++;
				}else if (instruction == "reg0_b") {
					cpu.ram.memory[currentPos++] = 24;
					line++;
				}else if (instruction == "reg0_be") {
					cpu.ram.memory[currentPos++] = 25;
					line++;
				}else if (instruction == "reg1_b") {
					cpu.ram.memory[currentPos++] = 26;
					line++;
				}else if (instruction == "reg1_be") {
					cpu.ram.memory[currentPos++] = 27;
					line++;
				}else if (instruction == "reg_eql") {
					cpu.ram.memory[currentPos++] = 28;
					line++;
				}else if (instruction == "reg_dif") {
					cpu.ram.memory[currentPos++] = 29;
					line++;
				}else if (instruction == "jmp") {
					std::string arg1 = code[i + 1];
					int pos;
					
					cpu.ram.memory[currentPos++] = 30;

					checkJmpPos(pos, arg1, jumpNames, jumpPositions);

					byte b1, b2, b3;
					convertByte(pos, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "cmp") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 31;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					int pos;

					checkJmpPos(pos, arg2, jumpNames, jumpPositions);

					cpu.ram.memory[currentPos++] = (byte)Arg1;
					byte b1, b2, b3;
					convertByte(pos, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					i += 2;
					line++;
				}else if (instruction == "cmp_a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					cpu.ram.memory[currentPos++] = 54;

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					int Arg2;
					checkArgType(Arg2, arg2, cpu, vars, varValues);					

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					cpu.ram.memory[currentPos++] = b4;
					cpu.ram.memory[currentPos++] = b5;

					i += 2;
					line++;
				}else if (instruction == "cot0") {
					cpu.ram.memory[currentPos++] = 40;
					line++;
				}else if (instruction == "cot1") {
					cpu.ram.memory[currentPos++] = 41;
					line++;
				}else if (instruction == "cs_out0") {
					cpu.ram.memory[currentPos++] = 62;

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
					cpu.ram.memory[currentPos++] = 63;

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
					cpu.ram.memory[currentPos++] = 65;
					line++;
				}else if (instruction == "clr_vram") {
					cpu.ram.memory[currentPos++] = 66;
					line++;
				}else if (instruction == "wait") {
					cpu.ram.memory[currentPos++] = 32;

					std::string arg1 = code[i + 1];

					int Arg1;
					checkArgType(Arg1, arg1, cpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 255) == true) { break; }

					cpu.ram.memory[currentPos++] = (byte)Arg1;

					i++;
					line++;
				}else if(instruction == "stop") {
					cpu.ram.memory[currentPos++] = 18;
					hasStop = true;
					line++;
				}else{
					if (instruction.back() == ':') {
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

		}

		if (hasStop == false) {
			std::cout << "Compilation terminated, no > stop < instruction found." << '\n';
			cpu.ram.Clear();
		}


		std::cout << "Program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}
}