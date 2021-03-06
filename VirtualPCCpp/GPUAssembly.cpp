#include "GPUAssembly.h"

namespace GPUAssembly {

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

	void checkArgType(int& arg, std::string& argument, GPU& gpu, std::vector<std::string>& vars, std::vector<int>& varValues) {
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
			if (argument == "reg0") {
				arg = 256 + 1;
			}else if (argument == "reg1") {
				arg = 256 + 2;
			}else if (argument == "idx") {
				arg = 256 + 3;
			}else if (argument == "idy") {
				arg = 256 + 4;
			}else {
				arg = std::stoi(argument);
			}
		}	
	}

	void checkArgTypeV(int& arg, std::string& argument, GPU& gpu, std::vector<std::string>& vars, std::vector<int>& varValues) {
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

	void Compile(std::vector<std::string>& code, GPU& gpu, std::vector<int>& vValues, std::vector<std::string>& vNames, int newCP) {
		

		int currentPos = newCP;
		int additionalMemory = 0;
		int line = 0;
		std::vector<int> jumpPositions;
		std::vector<std::string> vars;
		std::vector<std::string> jumpNames;
		std::vector<int> varValues;

		bool dataM = false;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if(instruction == "data_s") {
				dataM = true;
				i++;
				instruction = code[i];
				line++;
			}

			if(instruction == "data_e") {
				dataM = false;
				currentPos += additionalMemory;
				i++;
				instruction = code[i];
				line++;
			}

			if (dataM == true) {
				if (instruction == "u8") {
					additionalMemory++;
					i += 2;
					line++;
				}else if (instruction == "u16") {
					additionalMemory += 2;
					line++;
					i += 2;
				}else {
					std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
					std::cout << '\n';
					std::cout << lines[line] << '\n';
					std::cout << "^" << '\n';
					std::cout << '\n';
					std::cout << "Compilation terminated." << '\n';
					gpu.vRam.Clear();
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
				}else if (instruction == "reg0_be_s") {
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
				}else if (instruction == "get_idx") {
					currentPos++;
					line++;
				}else if (instruction == "get_idy") {
					currentPos++;
					line++;
				}else if (instruction == "p_out") {
					currentPos++;
					line++;
				}else if (instruction == "loadR") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "loadG") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "loadB") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "kernel_size") {
					i += 2;
					line++;
				}else if (instruction == "load0_8g") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load1_8g") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load0_16g") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "load1_16g") {
					currentPos += 3;
					i++;
					line++;
				}else if (instruction == "max") {
					currentPos += 7;
					i += 3;
					line++;
				}else if (instruction == "min") {
					currentPos += 7;
					i += 3;
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
						gpu.vRam.Clear();
						return;
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
				gpu.programCounter = currentPos;
				i++;
				line++;
				instruction = code[i];
			}

			if (dataM == true) {
				if (instruction == "u8") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(additionalMemory);

					for (int k = 0; k < gpu.cores.size(); k++) {
						gpu.cores[k].intMem.memory[additionalMemory] = (byte)Arg2;
					}

					additionalMemory++;
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

					for (int k = 0; k < gpu.cores.size(); k++) {
						gpu.cores[k].intMem.memory[additionalMemory] = b1;
						gpu.cores[k].intMem.memory[additionalMemory] = b2;
					}

					additionalMemory += 2;
					i += 2;
					line++;
				}else{
					std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
					std::cout << '\n';
					std::cout << lines[line] << '\n';
					std::cout << "^" << '\n';
					std::cout << '\n';
					std::cout << "Compilation terminated." << '\n';
					gpu.vRam.Clear();
					break;
				}
			}else if (dataM == false) {
				if (instruction == "load0_8") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 3;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_8") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 4;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load0_16") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 1;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_16") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 2;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt0_16") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 5;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt1_16") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 6;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt0_8") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 7;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "wrt1_8") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 8;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "sum") {
					gpu.vRam.memory[currentPos++] = 10;
					line++;
				}else if (instruction == "sub") {
					gpu.vRam.memory[currentPos++] = 11;
					line++;
				}else if (instruction == "mlt") {
					gpu.vRam.memory[currentPos++] = 12;
					line++;
				}else if (instruction == "div") {
					gpu.vRam.memory[currentPos++] = 13;
					line++;
				}else if (instruction == "reg0_b") {
					gpu.vRam.memory[currentPos++] = 14;
					line++;
				}else if (instruction == "reg0_be") {
					gpu.vRam.memory[currentPos++] = 15;
					line++;
				}else if (instruction == "reg0_be_s") {
					gpu.vRam.memory[currentPos++] = 29;
					line++;
				}else if (instruction == "reg1_b") {
					gpu.vRam.memory[currentPos++] = 16;
					line++;
				}else if (instruction == "reg1_be") {
					gpu.vRam.memory[currentPos++] = 17;
					line++;
				}else if (instruction == "reg_eql") {
					gpu.vRam.memory[currentPos++] = 18;
					line++;
				}else if (instruction == "reg_dif") {
					gpu.vRam.memory[currentPos++] = 19;
					line++;
				}else if (instruction == "jmp") {
					std::string arg1 = code[i + 1];
					int pos;

					gpu.vRam.memory[currentPos++] = 20;

					checkJmpPos(pos, arg1, jumpNames, jumpPositions);

					byte b1, b2, b3;
					convertByte(pos, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "cmp") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					gpu.vRam.memory[currentPos++] = 21;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					int pos;

					checkJmpPos(pos, arg2, jumpNames, jumpPositions);

					gpu.vRam.memory[currentPos++] = (byte)Arg1;
					byte b1, b2, b3;
					convertByte(pos, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i += 2;
					line++;
				}else if(instruction == "get_idx") {
					gpu.vRam.memory[currentPos++] = 31;
					line++;
				}else if(instruction == "get_idy") {
					gpu.vRam.memory[currentPos++] = 32;
					line++;
				}else if(instruction == "p_out") {
					gpu.vRam.memory[currentPos++] = 30;
					line++;
				}else if (instruction == "loadR") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 26;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "loadG") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 27;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "loadB") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 28;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "kernel_size") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];
					
					int xSize = std::stoi(arg1);
					int ySize = std::stoi(arg2);

					for (int y = 0; y < ySize; y++) {
						for (int x = 0; x < xSize; x++) {
							task_id tmp;
							tmp.x = x;
							tmp.y = y;
							gpu.tasks.push_back(tmp);
						}
					}

					auto rng = std::default_random_engine {};
					std::shuffle(gpu.tasks.begin(), gpu.tasks.end(), rng);

					i += 2;
					line++;
				}else if (instruction == "load0_8g") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 35;

					int Arg1;
					checkArgTypeV(Arg1, arg1, gpu, vNames, vValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_8g") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 36;

					int Arg1;
					checkArgTypeV(Arg1, arg1, gpu, vNames, vValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load0_16g") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 33;

					int Arg1;
					checkArgTypeV(Arg1, arg1, gpu, vNames, vValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "load1_16g") {
					std::string arg1 = code[i + 1];

					gpu.vRam.memory[currentPos++] = 34;

					int Arg1;
					checkArgTypeV(Arg1, arg1, gpu, vNames, vValues);

					if(checkArgSize(Arg1, 1, instruction, i, 65535) == true) { break; }

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					i++;
					line++;
				}else if (instruction == "max") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];
					std::string arg3 = code[i + 3];

					gpu.vRam.memory[currentPos++] = 37;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					int Arg2;
					checkArgType(Arg2, arg2, gpu, vars, varValues);

					int Arg3;
					checkArgType(Arg3, arg3, gpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					gpu.vRam.memory[currentPos++] = b4;
					gpu.vRam.memory[currentPos++] = b5;

					byte b7, b8, b9;
					convertByte(Arg3, b7, b8, b9);
					gpu.vRam.memory[currentPos++] = b7;
					gpu.vRam.memory[currentPos++] = b8;

					i += 3;
					line++;
				}else if (instruction == "min") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];
					std::string arg3 = code[i + 3];

					gpu.vRam.memory[currentPos++] = 38;

					int Arg1;
					checkArgType(Arg1, arg1, gpu, vars, varValues);

					int Arg2;
					checkArgType(Arg2, arg2, gpu, vars, varValues);

					int Arg3;
					checkArgType(Arg3, arg3, gpu, vars, varValues);

					byte b1, b2, b3;
					convertByte(Arg1, b1, b2, b3);
					gpu.vRam.memory[currentPos++] = b1;
					gpu.vRam.memory[currentPos++] = b2;

					byte b4, b5, b6;
					convertByte(Arg2, b4, b5, b6);
					gpu.vRam.memory[currentPos++] = b4;
					gpu.vRam.memory[currentPos++] = b5;

					byte b7, b8, b9;
					convertByte(Arg3, b7, b8, b9);
					gpu.vRam.memory[currentPos++] = b7;
					gpu.vRam.memory[currentPos++] = b8;

					i += 3;
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
						gpu.vRam.Clear();
						break;
					}	
				}

			}

		}

		std::cout << "GPU program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}
}