#include "Assembly.h"

namespace Assembly {

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

	int checkJmpPos(std::string argument, std::vector<std::string>& jumpNames, std::vector<int> jumpPos) {
		bool varExists = false;
		int id = 0;
		int tmp = 0;

		for (unsigned int i = 0; i < jumpNames.size(); i++) {
			if (argument == jumpNames[i]) {
				varExists = true;
				id = i;
				break;
			}
		}

		if (varExists == true) {
			tmp = jumpPos[id];
		}

		return tmp;
	}

	int checkArgType(std::string& argument, CPU& cpu, std::vector<std::string>& vars, std::vector<int>& varValues) {
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
			return varValues[id];
		}else if (varExists == false) {
			if (argument == "REG0") {
				return cpu.ram.memory.size() + 1;
			}else if (argument == "REG1") {
				return cpu.ram.memory.size() + 2;
			}else if (argument == "REG_INT") {
				return cpu.ram.memory.size() + 3;
			}else {
				return std::stoi(argument);
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

	void Compile(std::vector<std::string>& code, CPU& cpu, std::vector<int>& vValues, std::vector<std::string>& vNames, int& finalCP) {
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
		int additionalMemory = 0;
		int GadditionalMemory = 0;
		int line = 0;

		std::vector<int> jumpPositions;
		std::vector<std::string> jumpNames;
		std::vector<int> varValues;
		std::vector<std::string> vars;

		bool dataM = false;
		bool interM = false;
		bool hasStop = false;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if (instruction == "program.start") {
				continue;
			}

			if(instruction == "data.") {
				dataM = true;
				continue;
			}

			if (instruction == "interrupt.s") {
				interM = true;
				continue;
			}

			if (instruction == "interrupt.e") {
				interM = false;
				currentPos += 3;
				continue;
			}

			if (dataM == true) {
				if (instruction == "s8") {
					currentPos++;
					i += 2;
					continue;
				}else if (instruction == "d16") {
					currentPos += 2;
					i += 2;
					continue;
				}else if (instruction == "l32") {
					currentPos += 4;
					i += 2;
					continue;
				}else if(instruction == "s8.a") {
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					bool ended = false;
					int endInc = Arg2;

					for (int j = 0; j < Arg2; j++) {
						if (ended == false) {
							if(code[i + 3 + j] == "#"){
								ended = true;
								endInc = j + 1;
							}else{
								currentPos++;
							}
						}

					}

					i += (2 + endInc);
					line++;
					continue;
				}
			}

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
					cpu.ram.Clear();
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
					cpu.ram.Clear();
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
				currentPos += 3;
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
					cpu.ram.Clear();
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
					cpu.ram.Clear();
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
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
					cpu.ram.Clear();
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
						cpu.ram.Clear();
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
						cpu.ram.Clear();
						break;
					}
				}else {
					std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (instruction == "gpu_str") {
				currentPos++;
				line++;
			}else if (instruction == "wait") {
				currentPos += 2;
				i++;
				line++;
			}else if(instruction == "stop") {
				currentPos++;
				line++;
			}else if(instruction == "sb.setID") {
				currentPos += 6;	
				i += 3;
				line++;
			}else if(instruction == "sb.setCB") {
				currentPos += 6;	
				i += 3;
				line++;
			}else if(instruction == "sb.setCF") {
				currentPos += 6;	
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
					cpu.ram.Clear();
					break;
				}
			}

		}

		currentPos = 0;
		additionalMemory = 0;
		line = 0;
		dataM = false;
		interM = false;

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if (instruction == "program.start") {
				cpu.programCounter = currentPos;
				continue;
			}

			if(instruction == "data.") {
				dataM = true;
				continue;
			}

			if (instruction == "interrupt.s") {
				interM = true;
				cpu.interPos = currentPos;
				continue;
			}

			if (instruction == "interrupt.e") {
				interM = false;
				cpu.ram.memory[currentPos++] = 30;
				cpu.interTartgetPos = currentPos;
				cpu.ram.memory[currentPos++] = 0;
				cpu.ram.memory[currentPos++] = 0;
				cpu.programCounter = currentPos;
				continue;
			}

			if (dataM == true) {
				if (instruction == "s8") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(currentPos);

					cpu.ram.memory[currentPos++] = (byte)Arg2;
					i += 2;
					line++;
					continue;
				}else if (instruction == "d16") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(currentPos);

					byte b1, b2;
					convertByte2(Arg2, b1, b2);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					i += 2;
					line++;
				}else if (instruction == "l32") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(currentPos);

					byte b1, b2, b3, b4;
					convertByte4(Arg2, b1, b2, b3, b4);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;
					cpu.ram.memory[currentPos++] = b4;
					i += 2;
					line++;
				}else if(instruction == "s8.a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					vars.push_back(arg1);
					int Arg2 = std::stoi(arg2);
					varValues.push_back(currentPos);

					bool ended = false;
					int endInc = Arg2;

					for (int j = 0; j < Arg2; j++) {
						int element = 0;
						if (ended == false) {
							if(code[i + 3 + j] == "#"){
								ended = true;
								endInc = j + 1;
							}else{
								element = checkArgType(code[i + 3 + j], cpu, vars, varValues);
							}
						}
						cpu.ram.memory[currentPos++] = (byte)element;
					}

					i += (2 + endInc);
					line++;
					continue;
				}
			}

			if(instruction.compare(0, 5, "move.") == 0) {
				if(instruction[5] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x02;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);
						
						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[5] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x02;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[5] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x02;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x01;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction.compare(0, 6, "moveO.") == 0) {
				if(instruction[6] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);
						
						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[6] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[6] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x03;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x04;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, vars, varValues);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, vars, varValues);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction == "inc") {
				cpu.ram.memory[currentPos++] = 0x12;

				byte arg = indexRegister(code, i, 1);

				cpu.ram.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "dec") {
				cpu.ram.memory[currentPos++] = 0x15;

				byte arg = indexRegister(code, i, 1);

				cpu.ram.memory[currentPos++] = arg;

				i++;
				line++;
			}else if(instruction == "cot") {
				cpu.ram.memory[currentPos++] = 0x13;

				byte arg = indexRegister(code, i, 1);

				cpu.ram.memory[currentPos++] = arg;

				i++;
				line++;
			}else if (instruction == "add") {
				cpu.ram.memory[currentPos++] = 0x06;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				cpu.ram.memory[currentPos++] = arg;

				i += 2;
				line++;
			}else if (instruction == "sub") {
				cpu.ram.memory[currentPos++] = 0x07;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				cpu.ram.memory[currentPos++] = arg;

				i += 2;
				line++;
			}else if (instruction.compare(0, 4, "mlt.") == 0) {
				if(instruction[4] == 's'){
					cpu.ram.memory[currentPos++] = 0x08;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					cpu.ram.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					cpu.ram.memory[currentPos++] = 0x09;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					cpu.ram.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >mlt< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "div.") == 0) {
				if(instruction[4] == 's'){
					cpu.ram.memory[currentPos++] = 0x0A;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					cpu.ram.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					cpu.ram.memory[currentPos++] = 0x0B;

					byte arg = indexRegister(code, i, 1);
					byte arg2 = indexRegister(code, i, 2);
					arg |= (arg2 << 4);

					cpu.ram.memory[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown sign type at instruction >div< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "jmp.") == 0) {
				if(instruction[4] == 'z') {
					cpu.ram.memory[currentPos++] = 0x11;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x0 << 4);

					cpu.ram.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 2], jumpNames, jumpPositions);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;

					i += 2;
					line++;
				}else if(instruction[4] == 'o') {
					cpu.ram.memory[currentPos++] = 0x11;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					cpu.ram.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 2], jumpNames, jumpPositions);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;

					i += 2;
					line++;
				}else if(instruction[4] == 'u') {
					cpu.ram.memory[currentPos++] = 0x11;

					byte arg = 0;
					arg |= (0x2 << 4);

					cpu.ram.memory[currentPos++] = arg;

					int arg2 = checkJmpPos(code[i + 1], jumpNames, jumpPositions);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (instruction.compare(0, 4, "cmp.") == 0) {
				if(instruction[4] == 's' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						cpu.ram.memory[currentPos++] = 0x0C;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						cpu.ram.memory[currentPos++] = 0x0E;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						cpu.ram.memory[currentPos++] = 0x10;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						cpu.ram.Clear();
						break;
					}
				}else if(instruction[4] == 'u' && instruction[5] == '.') {
					if(instruction[6] == 'b') {
						cpu.ram.memory[currentPos++] = 0x0D;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'b' && instruction[7] == 'e') {
						cpu.ram.memory[currentPos++] = 0x0F;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else if(instruction[6] == 'e') {
						cpu.ram.memory[currentPos++] = 0x10;

						byte arg = indexRegister(code, i, 1);
						arg |= (indexRegister(code, i, 2) << 4);

						cpu.ram.memory[currentPos++] = arg;

						i += 2;
						line++;
					}else{
						std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
						cpu.ram.Clear();
						break;
					}
				}else {
					std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if (instruction == "gpu_str") {
				currentPos++;
				line++;
			}else if (instruction == "wait") {
				currentPos += 2;
				i++;
				line++;
			}else if(instruction == "stop") {
				currentPos++;
				line++;
			}else if(instruction == "sb.setID") {
				cpu.ram.memory[currentPos++] = 0x16;

				byte arg1 = indexRegister(code, i, 1);
				arg1 |= (indexRegister(code, i , 2) << 4);

				cpu.ram.memory[currentPos++] = arg1;

				cpu.ram.memory[currentPos++] = 0x0;

				int arg2 = checkArgType(code[i + 3], cpu, vars, varValues);
						
				byte b1, b2, b3;
				convertByte3(arg2, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;
				cpu.ram.memory[currentPos++] = b3;

				i += 3;
				line++;
			}else if(instruction == "sb.setCB") {
				cpu.ram.memory[currentPos++] = 0x16;

				byte arg1 = indexRegister(code, i, 1);
				arg1 |= (indexRegister(code, i , 2) << 4);

				cpu.ram.memory[currentPos++] = arg1;

				cpu.ram.memory[currentPos++] = 0x1;

				int arg2 = checkArgType(code[i + 3], cpu, vars, varValues);
						
				byte b1, b2, b3;
				convertByte3(arg2, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;
				cpu.ram.memory[currentPos++] = b3;

				i += 3;
				line++;
			}else if(instruction == "sb.setCF") {
				cpu.ram.memory[currentPos++] = 0x16;

				byte arg1 = indexRegister(code, i, 1);
				arg1 |= (indexRegister(code, i , 2) << 4);

				cpu.ram.memory[currentPos++] = arg1;

				cpu.ram.memory[currentPos++] = 0x2;

				int arg2 = checkArgType(code[i + 3], cpu, vars, varValues);
						
				byte b1, b2, b3;
				convertByte3(arg2, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;
				cpu.ram.memory[currentPos++] = b3;

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
					cpu.ram.Clear();
					break;
				}
			}

		}

		finalCP = GadditionalMemory;

		std::cout << "CPU program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}
}
