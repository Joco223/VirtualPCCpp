#include "GPUAssembly.h"

namespace GPUAssembly {

	std::vector<std::string> lines;

	void convertByte2(int& number, byte& b1, byte& b2) {
		b1 = number & 0xff;
		b2 = (byte)(number >> 8);
	}

	void convertByte3(int& number, byte& b1, byte& b2, byte& b3) {
		b1 = number & 0xff;
		b2 = (byte)(number >> 8);
		b3 = (byte)(number >> 16);
	}

	void convertByte4(int& number, byte& b1, byte& b2, byte& b3, byte& b4) {
		b1 = number & 0xff;
		b2 = (byte)(number >> 8);
		b3 = (byte)(number >> 16);
		b4 = (byte)(number >> 24);
	}

	bool checkArgSize(int& arg, int argc, std::string& instruction, unsigned int& instructionC, int maxSize) {
		if (arg > maxSize) {
			std::cout << "Error: Argument " << argc << " exceeds size limit of " << maxSize << " at instruction " << instruction << " at position " << instructionC + 1 + argc << '\n';
			return true;
		}else{
			return false;
		}
	}

	int checkJmpPos(std::string argument, std::vector<jump_pos>& jmps, int currentDepth) {
		bool varExists = false;
		int id = 0;
		int tmp = 0;

		for (unsigned int i = 0; i < jmps.size(); i++) {
			if (argument == jmps[i].name && currentDepth <= jmps[i].sDepth) {
				varExists = true;
				id = i;
				break;
			}
		}

		if (varExists == true) {
			tmp = jmps[id].position;
		}

		return tmp;
	}

	int checkArgType(std::string& argument, GPU& gpu, std::vector<variable>& vars, int currentDepth) {
		bool varExists = false;
		int id = 0;

		for (unsigned int i = 0; i < vars.size(); i++) {
			if (argument == vars[i].name && currentDepth <= vars[i].sDepth) {
				varExists = true;
				id = i;
				break;
			}
		}

		if (varExists == true) {
			return vars[id].value;
		}else if (varExists == false) {
			if(argument == "regA") {return gpu.vRam.memory.size() +  1; };
			if(argument == "regB") {return gpu.vRam.memory.size() +  2; };
			if(argument == "regC") {return gpu.vRam.memory.size() +  3; };
			if(argument == "regD") {return gpu.vRam.memory.size() +  4; };
			if(argument == "regE") {return gpu.vRam.memory.size() +  5; };
			if(argument == "regF") {return gpu.vRam.memory.size() +  6; };
			if(argument == "regG") {return gpu.vRam.memory.size() +  7; };
			if(argument == "regH") {return gpu.vRam.memory.size() +  8; };
			if(argument == "regI") {return gpu.vRam.memory.size() +  9; };
			if(argument == "regJ") {return gpu.vRam.memory.size() + 10; };
			if(argument == "regK") {return gpu.vRam.memory.size() + 11; };
			if(argument == "regL") {return gpu.vRam.memory.size() + 12; };
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

	bool isRegister(std::vector<std::string>& code, int i){
		return code[i + 1] == "regA" || code[i + 1] == "regB" || code[i + 1] == "regC" || code[i + 1] == "regD" ||
		   	   code[i + 1] == "regE" || code[i + 1] == "regF" || code[i + 1] == "regG" || code[i + 1] == "regH" ||
		   	   code[i + 1] == "regI" || code[i + 1] == "regJ" || code[i + 1] == "regK" || code[i + 1] == "regL";
	}

	int indexRegister(std::vector<std::string>& code, int i, int k){
		if(code[i + k] == "regA") {return  0;}
		if(code[i + k] == "regB") {return  1;}
		if(code[i + k] == "regC") {return  2;}
		if(code[i + k] == "regD") {return  3;}
		if(code[i + k] == "regE") {return  4;}
		if(code[i + k] == "regF") {return  5;}
		if(code[i + k] == "regG") {return  6;}
		if(code[i + k] == "regH") {return  7;}
		if(code[i + k] == "regI") {return  8;}
		if(code[i + k] == "regJ") {return  9;}
		if(code[i + k] == "regK") {return 10;}
		if(code[i + k] == "regL") {return 11;}
	}

	void Compile(std::vector<std::string>& code, GPU& gpu, std::vector<variable>& variables) {
		std::vector<char> charactersA = {'0',  '1', '2', '3', '4',  '5',  '6', '7', '8', '9',
										 '!', '\"', '#', '$', '%',  '&', '\'', '(', ')', '*',
										 '+',  '-', ',', '.', '/', '\\',  ':', ';', '<', '>',
										 '=',  '?', '[', ']', '{',  '}',  '`', '^', '|', '~',
										 'A',  'B', 'C', 'D', 'E',  'F',  'G', 'H', 'I', 'J',
										 'K',  'L', 'M', 'N', 'O',  'P',  'Q', 'R', 'S', 'T',
										 'U',  'V', 'W', 'X', 'Y',  'Z',  '_', ' ', '@', 'a',
										 'b',  'c', 'd', 'e', 'f',  'g',  'h', 'i', 'j', 'k',
										 'l',  'm', 'n', 'o', 'p',  'q',  'r', 's', 't', 'u',
										 'v',  'w', 'x', 'y', 'z'};

		int currentPos = 0;
		int line = 0;

		std::vector<jump_pos> jumpPositions;


		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];


			if(instruction.compare(0, 5, "move.") == 0) {
				if(instruction[5] == 's'){
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[5] == 'd') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[5] == 'l') {
					currentPos += 5;
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if(instruction.compare(0, 6, "moveO.") == 0) {
				if(instruction[6] == 's'){
					currentPos += 8;
					i += 3;
					line++;
				}else if(instruction[6] == 'd') {
					currentPos += 8;
					i += 3;
					line++;
				}else if(instruction[6] == 'l') {
					currentPos += 8;
					i += 3;
					line++;
				}else{
					std::cout << "Uknwown data type at instruction >moveO< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if(instruction == "clr") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "inc") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "dec") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "g.IDX") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "g.IDY") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "g.ID") {
				i++;
				currentPos += 2;
				line++;
			}else if(instruction == "cot") {
				i++;
				currentPos += 2;
				line++;
			}else if (instruction == "add") {
				i += 2;
				currentPos += 2;
				line++;
			}else if (instruction == "sub") {
				i += 2;
				currentPos += 2;
				line++;
			}else if (instruction.compare(0, 4, "mlt.") == 0) {
				if(instruction[4] == 's'){
					currentPos += 2;
					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					currentPos += 2;
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >mlt< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "div.") == 0) {
				if(instruction[4] == 's'){
					currentPos += 2;
					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					currentPos += 2;
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >div< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "jmp.") == 0) {
				if(instruction[4] == 'z') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[4] == 'o') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					currentPos += 5;
					i++;
					line++;
				}else{
					std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "cmp.") == 0) {
				if(instruction[4] == 's' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						currentPos += 2;
						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						currentPos += 2;
						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						currentPos += 2;
						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						gpu.progMem.Clear();
						break;
					}
				}else if(instruction[4] == 'u' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						currentPos += 2;
						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						currentPos += 2;
						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						currentPos += 2;
						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						gpu.progMem.Clear();
						break;
					}
				}else {
					std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 5, "displ") == 0) {
				i += 3;
				currentPos += 3;
				line++;
			}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
				currentPos += 6;
				i += 2;
				line++;
			}else{
				if (instruction.back() == ':') {
					std::string tmp = instruction;
					tmp.erase(tmp.end() - 1);
					jump_pos temp;
					temp.sDepth = 0;
					temp.name = tmp;
					temp.position = currentPos;
					jumpPositions.push_back(temp);
					line++;
				}else{
					std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
					std::cout << '\n';
					std::cout << lines[line] << '\n';
					std::cout << "^" << '\n';
					std::cout << '\n';
					std::cout << "Compilation terminated." << '\n';
					gpu.progMem.Clear();
					break;
				}
			}

		}

		gpu.progMem.memory.resize(currentPos + 2);
		currentPos = 0;
		line = 0;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if(instruction.compare(0, 5, "move.") == 0) {
				if(instruction[5] == 's'){
					if(isRegister(code, i)) {
						//No write to single memory position in vram
					}else{
						gpu.progMem.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[5] == 'd') {
					if(isRegister(code, i)) {
						//No write to single memory position in vram
					}else{
						gpu.progMem.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[5] == 'l') {
					if(isRegister(code, i)) {
						//No write to single memory position in vram
					}else{
						gpu.progMem.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if(instruction.compare(0, 6, "moveO.") == 0) {
				if(instruction[6] == 's'){
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (1 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[6] == 'd') {
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x2 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[6] == 'l') {
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x3 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						gpu.progMem.memory[currentPos++] = b4;
						gpu.progMem.memory[currentPos++] = b5;
						gpu.progMem.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if(instruction == "inc") {
				gpu.progMem.memory[currentPos++] = 0x12;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "dec") {
				gpu.progMem.memory[currentPos++] = 0x13;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "clr") {
				gpu.progMem.memory[currentPos++] = 0x14;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "g.IDX") {
				gpu.progMem.memory[currentPos++] = 0x16;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "g.IDY") {
				gpu.progMem.memory[currentPos++] = 0x17;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "g.ID") {
				gpu.progMem.memory[currentPos++] = 0x18;

				byte arg = indexRegister(code, i, 1);

				gpu.progMem.memory[currentPos++] = arg;

				i++;
				line++;
			}else if (instruction == "add") {
				gpu.progMem.memory[currentPos++] = 0x06;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				gpu.progMem.memory[currentPos++] = arg;

				i += 2;
				line++;
			}else if (instruction == "mod") {
				gpu.progMem.memory[currentPos++] = 0x19;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				gpu.progMem.memory[currentPos++] = arg;

				i += 2;
				line++;
			}else if (instruction == "sub") {
				gpu.progMem.memory[currentPos++] = 0x07;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				gpu.progMem.memory[currentPos++] = arg;

				i += 2;
				line++;
			}else if (instruction.compare(0, 4, "mlt.") == 0) {
				if(instruction[4] == 's'){
					gpu.progMem.memory[currentPos++] = 0x08;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					gpu.progMem.memory[currentPos++] = 0x09;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >mlt< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "div.") == 0) {
				if(instruction[4] == 's'){
					gpu.progMem.memory[currentPos++] = 0x0A;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					gpu.progMem.memory[currentPos++] = 0x0B;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >div< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "jmp.") == 0) {
				if(instruction[4] == 'z') {
					gpu.progMem.memory[currentPos++] = 0x11;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x0 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 2], jumpPositions, 0);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					gpu.progMem.memory[currentPos++] = b1;
					gpu.progMem.memory[currentPos++] = b2;
					gpu.progMem.memory[currentPos++] = b3;

					i += 2;
					line++;
				}else if(instruction[4] == 'o') {
					gpu.progMem.memory[currentPos++] = 0x11;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 2], jumpPositions, 0);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					gpu.progMem.memory[currentPos++] = b1;
					gpu.progMem.memory[currentPos++] = b2;
					gpu.progMem.memory[currentPos++] = b3;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					gpu.progMem.memory[currentPos++] = 0x11;

					byte arg = 0;
					arg |= (0x2 << 4);

					gpu.progMem.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 1], jumpPositions, 0);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					gpu.progMem.memory[currentPos++] = b1;
					gpu.progMem.memory[currentPos++] = b2;
					gpu.progMem.memory[currentPos++] = b3;

					i++;
					line++;
				}else{
					std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "cmp.") == 0) {
				if(instruction[4] == 's' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						gpu.progMem.memory[currentPos++] = 0x0C;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						gpu.progMem.memory[currentPos++] = 0x0E;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						gpu.progMem.memory[currentPos++] = 0x10;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						gpu.progMem.Clear();
						break;
					}
				}else if(instruction[4] == 'u' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						gpu.progMem.memory[currentPos++] = 0x0D;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						gpu.progMem.memory[currentPos++] = 0x0F;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						gpu.progMem.memory[currentPos++] = 0x10;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						gpu.progMem.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						gpu.progMem.Clear();
						break;
					}
				}else {
					std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
			}else if (instruction.compare(0, 5, "displ") == 0) {
				gpu.progMem.memory[currentPos++] = 0x15;

				byte arg = indexRegister(code, i, 1);
				arg |= (indexRegister(code, i, 2) << 4);
				byte arg2 = indexRegister(code, i, 3);

				gpu.progMem.memory[currentPos++] = arg;
				gpu.progMem.memory[currentPos++] = arg2;

				i += 3;
				line++;
			}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
				if(instruction[6] == 's'){
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x1B;

						byte arg = indexRegister(code, i, 1);
						arg |= (1 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3, b4;
						convertByte4(arg2, b1, b2, b3, b4);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						gpu.progMem.memory[currentPos++] = b4;

						i += 2;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x1A;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						currentPos++;

						i += 2;
						line++;
					}
				}else if(instruction[6] == 'd') {
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x1B;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3, b4;
						convertByte4(arg2, b1, b2, b3, b4);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						gpu.progMem.memory[currentPos++] = b4;

						i += 2;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x1A;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						currentPos++;

						i += 2;
						line++;
					}
				}else if(instruction[6] == 'l') {
					if(isRegister(code, i)) {
						gpu.progMem.memory[currentPos++] = 0x1B;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], gpu, variables, 0);

						byte b1, b2, b3, b4;
						convertByte4(arg2, b1, b2, b3, b4);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						gpu.progMem.memory[currentPos++] = b4;

						i += 2;
						line++;
					}else{
						gpu.progMem.memory[currentPos++] = 0x1A;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						gpu.progMem.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], gpu, variables, 0);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						gpu.progMem.memory[currentPos++] = b1;
						gpu.progMem.memory[currentPos++] = b2;
						gpu.progMem.memory[currentPos++] = b3;
						currentPos++;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >moveP< at line " << line << '\n';
					gpu.progMem.Clear();
					break;
				}
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
					gpu.progMem.Clear();
					break;
				}
			}

		}

		if(currentPos > 0){
			std::cout << "GPU program size is: " << currentPos << " bytes" << '\n';
			std::cout << '\n';
		}
	}
}
