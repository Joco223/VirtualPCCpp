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

	int checkArgType(std::string& argument, CPU& cpu, std::vector<variable>& vars, int currentDepth) {
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
			return vars[id].position;
		}else if (varExists == false) {
			if(argument == "regA") {return cpu.ram.memory.size() +  1; };
			if(argument == "regB") {return cpu.ram.memory.size() +  2; };
			if(argument == "regC") {return cpu.ram.memory.size() +  3; };
			if(argument == "regD") {return cpu.ram.memory.size() +  4; };
			if(argument == "regE") {return cpu.ram.memory.size() +  5; };
			if(argument == "regF") {return cpu.ram.memory.size() +  6; };
			if(argument == "regG") {return cpu.ram.memory.size() +  7; };
			if(argument == "regH") {return cpu.ram.memory.size() +  8; };
			if(argument == "regI") {return cpu.ram.memory.size() +  9; };
			if(argument == "regJ") {return cpu.ram.memory.size() + 10; };
			if(argument == "regK") {return cpu.ram.memory.size() + 11; };
			if(argument == "regL") {return cpu.ram.memory.size() + 12; };
			if(argument == "regInter") {return cpu.ram.memory.size() + 13; };
			if(argument == "regTimer") {return cpu.ram.memory.size() + 14; };
		}
	}

	int checkFunction(std::string function, CPU& cpu, std::vector<stack_object>& stack) {
		bool fnExists = false;
		int id = 0;

		for(int i = 0; i < stack.size(); i++){
			if(function == stack[i].name) {
				fnExists = true;
				id = i;
			}
		}

		if(fnExists == true){
			return id;
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
		   	   code[i + 1] == "regI" || code[i + 1] == "regJ" || code[i + 1] == "regK" || code[i + 1] == "regL" ||
			   code[i + 1] == "regInter" || code[i + 1] == "regTimer";
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
		if(code[i + k] == "regInter") {return 12;}
		if(code[i + k] == "regTimer") {return 13;}
	}

	int indexInterRegister(std::vector<std::string>& code, int i, int k){
		if(code[i + k] == "interRegA") {return  0;}
		if(code[i + k] == "interRegB") {return  1;}
		if(code[i + k] == "interRegC") {return  2;}
		if(code[i + k] == "interRegD") {return  3;}
		if(code[i + k] == "interRegE") {return  4;}
		if(code[i + k] == "interRegF") {return  5;}
		if(code[i + k] == "interRegG") {return  6;}
		if(code[i + k] == "interRegH") {return  7;}
	}

	void Compile(std::vector<std::string>& code, CPU& cpu, GPU& gpu, std::vector<variable>& gVariables, int& finalCP) {
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
		int currentPosG = 0;
		int additionalMemory = 0;
		int GadditionalMemory = 0;
		int line = 0;
		int stackPos = 0;
		int ffbID = 0;
		bool hasffb = false;

		int scopeDepth = 0;

		std::vector<jump_pos> jumpPositions;
		std::vector<variable> variables;
		std::vector<int> fnOffset;

		bool dataM = false;
		bool interM = false;
		bool hasStop = false;

		std::cout << '\n' << "Beginning part 1 compilation of vCPU program..." << '\n';

		for (unsigned int i = 0; i < code.size(); i++) {

			std::string instruction = code[i];

			if (instruction == "program.start") {
				currentPos += additionalMemory;
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
				currentPos += 4;
				continue;
			}

			if (dataM == true) {
				if (instruction == "s8") {
					currentPos++;
					i += 2;
					continue;
				}else if (instruction == "s8.g") {
					currentPosG++;
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
				}else if (instruction == "pt") {
					currentPos += 3;
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
							}
						}
						currentPos++;

					}

					i += (2 + endInc);
					line++;
					continue;
				}else if(instruction == "d16.a") {
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					bool ended = false;
					int endInc = Arg2;

					for (int j = 0; j < Arg2; j++) {
						if (ended == false) {
							if(code[i + 3 + j] == "#"){
								ended = true;
								endInc = j + 1;
							}
						}
						currentPos += 2;

					}

					i += (2 + endInc);
					line++;
					continue;
				}
			}

			if(instruction.compare(0, 5, "move.") == 0 && instruction.length() == 6) {
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
			}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
				if(instruction[6] == 's'){
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[6] == 'd') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[6] == 'l') {
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
			}else if(instruction.compare(0, 5, "moveP") == 0) {
				currentPos += 2;
				i += 2;
				line++;
			}else if(instruction.compare(0, 5, "moveI") == 0) {
				currentPos += 2;
				i += 2;
				line++;
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
			}else if (instruction == "mod") {
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
					i++;
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
			}else if (instruction == "gpu.str") {
				currentPos += 2;
				i += 2;
				line++;
			}else if (instruction == "wait") {
				currentPos += 2;
				i++;
				line++;
			}else if(instruction == "stop") {
				hasStop = true;
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
			}else if(instruction == "sb.update") {
				currentPos++;
				line++;
			}else if(!instruction.compare(0, 2, "fn")) {
				int r = 0;

				while (code[i + 2 + r].back() == ',') {
					r++;
				}
				fnOffset.push_back(r + 1);

				i += 2 + r;
			}else if(!instruction.compare(0, 4, "call")) {
				for(int k = 0; k < fnOffset[stackPos]; k++){
					currentPos += 3;
				}
				currentPos += 3;
				i += 2 + fnOffset[stackPos];
			}else if(!instruction.compare(0, 3, "ret")) {
				currentPos += 2;;
				i++;
			}else if(instruction.compare(0, 7, "move.g.") == 0) {
				if(instruction[7] == 's'){
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[7] == 'd') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[7] == 'l') {
					currentPos += 5;
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown data type at instruction >move.g.< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction.compare(0, 7, "move.h.") == 0) {
				if(instruction[7] == 's'){
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[7] == 'd') {
					currentPos += 5;
					i += 2;
					line++;
				}else if(instruction[7] == 'l') {
					currentPos += 5;
					i += 2;
					line++;
				}else{
					std::cout << "Uknwown data type at instruction >move.g.< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction.compare(0, 8, "moveO.g.") == 0) {
				if(instruction[8] == 's'){
					currentPos += 8;
					i += 3;
					line++;
				}else if(instruction[8] == 'd') {
					currentPos += 8;
					i += 3;
					line++;
				}else if(instruction[8] == 'l') {
					currentPos += 8;
					i += 3;
					line++;
				}else{
					std::cout << "Uknwown data type at instruction >moveO.g.< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else{
				if (instruction.back() == ':') {
					std::string tmp = instruction;
					tmp.erase(tmp.end() - 1);
					jump_pos temp;
					temp.sDepth = scopeDepth;
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
					cpu.ram.Clear();
					break;
				}
			}

			std::cout << "vCPU program part 1 compilation " << (int)(((float)(i + 1) / (float)code.size()) * 100) << "% done..." << '\r';

		}

		std::cout << '\n' << "Part 1 of compilation done..." << '\n';

		currentPos = 0;
		currentPosG = 0;
		additionalMemory = 0;
		line = 0;
		dataM = false;
		interM = false;
		scopeDepth = 0;

		std::cout << '\n' << "Beginning part 2 compilation of vCPU program..." << '\n';

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
				cpu.ram.memory[currentPos++] = 0x22;
				cpu.interTartgetPos = currentPos;
				cpu.ram.memory[currentPos++] = 0;
				cpu.ram.memory[currentPos++] = 0;
				cpu.ram.memory[currentPos++] = 0;
				cpu.programCounter = currentPos;
				cpu.interFinishPos = currentPos;
				continue;
			}

			if (dataM == true) {
				if (instruction == "s8") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					variable temp;
					temp.name = arg1;
					temp.position = currentPos;
					temp.sDepth = scopeDepth;
					temp.size = 1;
					variables.push_back(temp);

					cpu.ram.memory[currentPos++] = (byte)Arg2;
					i += 2;
					line++;
					continue;
				}else if (instruction == "s8.g") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					variable temp;
					temp.name = arg1;
					temp.position = currentPosG;
					temp.sDepth = scopeDepth;
					temp.size = 1;
					gVariables.push_back(temp);

					gpu.vRam.memory[currentPosG++] = (byte)Arg2;
					i += 2;
					line++;
					continue;
				}else if (instruction == "d16") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);

					variable temp;
					temp.name = arg1;
					temp.position = currentPos;
					temp.sDepth = scopeDepth;
					temp.size = 2;
					variables.push_back(temp);

					byte b1, b2;
					convertByte2(Arg2, b1, b2);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					i += 2;
					line++;
					continue;
				}else if (instruction == "pt") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					variable temp;
					temp.name = arg1;
					temp.position = currentPos;
					temp.sDepth = scopeDepth;
					temp.size = 3;
					variables.push_back(temp);

					int Arg2;

					if(arg2 == "ffb"){
						ffbID = variables.size() - 1;
						hasffb = true;
						Arg2 = 0;
					}else{
						Arg2 = std::stoi(arg2);
					}


					byte b1, b2, b3;
					convertByte3(Arg2, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;
					i += 2;
					line++;
					continue;
				}else if (instruction == "l32") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);
					variable temp;
					temp.name = arg1;
					temp.position = currentPos;
					temp.sDepth = scopeDepth;
					temp.size = 4;
					variables.push_back(temp);

					byte b1, b2, b3, b4;
					convertByte4(Arg2, b1, b2, b3, b4);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;
					cpu.ram.memory[currentPos++] = b4;
					i += 2;
					line++;
					continue;
				}else if(instruction == "s8.a") {
					std::string arg1 = code[i + 1];
					std::string arg2 = code[i + 2];

					int Arg2 = std::stoi(arg2);
					variable temp;
					temp.name = arg1;
					temp.position = currentPos;
					temp.sDepth = scopeDepth;
					temp.size = 1;
					variables.push_back(temp);

					bool ended = false;
					int endInc = Arg2;

					for (int j = 0; j < Arg2; j++) {
						int element = 0;
						if (ended == false) {
							if(code[i + 3 + j] == "#"){
								ended = true;
								endInc = j + 1;
							}else{
								element = std::stoi(code[i + 3 + j]);
							}
						}
						cpu.ram.memory[currentPos++] = (byte)element;
					}

					i += (2 + endInc);
					line++;
					continue;
				}/*else if(instruction == "d16.a") {
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
						byte b1, b2;
						convertByte2(element, b1, b2);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
					}

					i += (2 + endInc);
					line++;
					continue;
				}*/
			}

			if(instruction.compare(0, 5, "move.") == 0 && instruction.length() == 6) {
				if(instruction[5] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x02;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

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
			}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
				if(instruction[6] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x28;

						byte arg = indexRegister(code, i, 1);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x27;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[6] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x28;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x27;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[6] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x28;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x27;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >moveP< at line " << line << '\n';
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

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

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

						int arg2 = checkArgType(code[i + 1], cpu, variables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, variables, scopeDepth);

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
			}else if(instruction.compare(0, 5, "moveP") == 0) {
				if(isRegister(code, i)) {
					std::string regA = code[i + 1];
					std::string paramA = code[i + 2];

					byte arg = indexRegister(code, i, 1);
					arg |= (checkArgType(code[i + 2], cpu, variables, scopeDepth) << 4);

					cpu.ram.memory[currentPos++] = 0x19;
					cpu.ram.memory[currentPos++] = arg;
				}else{
					std::string regA = code[i + 2];
					std::string paramA = code[i + 1];

					byte arg = indexRegister(code, i, 2);
					arg |= (checkArgType(code[i + 1], cpu, variables, scopeDepth) << 4);

					cpu.ram.memory[currentPos++] = 0x18;
					cpu.ram.memory[currentPos++] = arg;
				}
				i += 2;
				line++;
			}else if(instruction.compare(0, 5, "moveI") == 0) {
				if(!isRegister(code, i)) {
					std::string regA = code[i + 1];
					std::string paramA = code[i + 2];

					byte arg = indexRegister(code, i, 2);
					arg |= indexInterRegister(code, i, 1);

					cpu.ram.memory[currentPos++] = 0x21;
					cpu.ram.memory[currentPos++] = arg;
				}else{
					std::cout << "Can't write to interrupt registers at line " << line << '\n';
				}
				i += 2;
				line++;
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
			}else if(instruction == "clr") {
				cpu.ram.memory[currentPos++] = 0x17;

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
			}else if (instruction == "mod") {
				cpu.ram.memory[currentPos++] = 0x25;

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

					int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth);

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

					int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth);

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

					int arg2 = checkJmpPos(code[i + 1], jumpPositions, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;

					i++;
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
			}else if (instruction == "gpu.str") {
				cpu.ram.memory[currentPos++] = 0x1C;

				byte arg1 = indexRegister(code, i, 1);
				arg1 |= (indexRegister(code, i , 2) << 4);

				cpu.ram.memory[currentPos++] = arg1;

				i += 2;
				line++;
			}else if (instruction == "wait") {
				currentPos += 2;
				i++;
				line++;
			}else if(instruction == "stop") {
				cpu.ram.memory[currentPos++] = 0x05;
				line++;
			}else if(instruction == "sb.setID") {
				cpu.ram.memory[currentPos++] = 0x16;

				byte arg1 = indexRegister(code, i, 1);
				arg1 |= (indexRegister(code, i , 2) << 4);

				cpu.ram.memory[currentPos++] = arg1;

				cpu.ram.memory[currentPos++] = 0x0;

				int arg2 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

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

				int arg2 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

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

				int arg2 = checkArgType(code[i + 3], cpu, variables, scopeDepth);

				byte b1, b2, b3;
				convertByte3(arg2, b1, b2, b3);
				cpu.ram.memory[currentPos++] = b1;
				cpu.ram.memory[currentPos++] = b2;
				cpu.ram.memory[currentPos++] = b3;

				i += 3;
				line++;
			}else if(instruction == "sb.update") {
				cpu.ram.memory[currentPos++] = 0x26;
				line++;
			}else if(!instruction.compare(0, 2, "fn")) {
				std::string name = code[i + 1];
				std::vector<std::string> parameters;
				std::vector<unsigned int> paramVal;
				std::vector<unsigned int> paramSize;

				int r = 0;

				scopeDepth++;

				while (code[i + 2 + r].back() == ',') {
					std::string tempo = code[i + 2 + r];
					if(tempo.back() == ',') {
						tempo.erase(tempo.end() - 1);
					}
					parameters.push_back(tempo);

					variable temp;
					temp.name = tempo;
					temp.position = r;
					temp.sDepth = scopeDepth;
					temp.size = 4;
					variables.push_back(temp);
					paramSize.push_back(4);

					r++;
				}
				std::string tempo = code[i + 2 + r];
				parameters.push_back(tempo);
				variable temp2;
				temp2.name = tempo;
				temp2.position = r;
				temp2.sDepth = scopeDepth;
				temp2.size = 1;
				variables.push_back(temp2);
				paramSize.push_back(4);

				paramVal.resize(parameters.size());

				stack_object temp;
				temp.name = name;
				temp.parameters.resize(parameters.size());
				temp.parametersSize = paramSize;
				temp.BP = currentPos;
				temp.PP = 0;
				cpu.stack.push_back(temp);

				i += 2 + r;
			}else if(!instruction.compare(0, 4, "call")) {
				std::string function = code[i + 2];

				stackPos = checkFunction(function, cpu, cpu.stack);

				cpu.ram.memory[currentPos++] = 0x1A;
				cpu.ram.memory[currentPos++] = stackPos;
				cpu.ram.memory[currentPos++] = indexRegister(code, i, 1);

				for(int k = 0; k < cpu.stack[stackPos].parameters.size(); k++){
					std::string parameter = code[i + 3 + k];

					int param = checkArgType(parameter, cpu, variables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(param, b1, b2, b3);
					cpu.ram.memory[currentPos++] = b1;
					cpu.ram.memory[currentPos++] = b2;
					cpu.ram.memory[currentPos++] = b3;
				}

				i += cpu.stack[stackPos].parameters.size() + 2;
			}else if(!instruction.compare(0, 3, "ret")) {
				cpu.ram.memory[currentPos++] = 0x1B;
				cpu.ram.memory[currentPos++] = checkArgType(code[i + 1], cpu, variables, scopeDepth);
				scopeDepth--;
				i++;
			}else if(instruction.compare(0, 7, "move.g.") == 0) {
				if(instruction[7] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1E;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x1D;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[7] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1E;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x1D;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[7] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1E;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x1D;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move.g< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction.compare(0, 8, "moveO.g.") == 0) {
				if(instruction[8] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1F;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x20;

						byte arg = indexRegister(code, i, 3);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[8] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1F;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x20;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else if(instruction[8] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x1F;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 3], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x20;

						byte arg = indexRegister(code, i, 3);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = checkArgType(code[i + 1], cpu, gVariables, scopeDepth);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						int arg3 = checkArgType(code[i + 2], cpu, gVariables, scopeDepth);

						byte b4, b5, b6;
						convertByte3(arg3, b4, b5, b6);
						cpu.ram.memory[currentPos++] = b4;
						cpu.ram.memory[currentPos++] = b5;
						cpu.ram.memory[currentPos++] = b6;

						i += 3;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >moveO.g< at line " << line << '\n';
					cpu.ram.Clear();
					break;
				}
			}else if(instruction.compare(0, 7, "move.h.") == 0) {
				if(instruction[7] == 's'){
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x24;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 2]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x23;

						byte arg = indexRegister(code, i, 2);
						arg |= (1 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 1]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[7] == 'd') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x24;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 2]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x23;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x2 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 1]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else if(instruction[7] == 'l') {
					if(isRegister(code, i)) {
						cpu.ram.memory[currentPos++] = 0x24;

						byte arg = indexRegister(code, i, 1);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 2]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}else{
						cpu.ram.memory[currentPos++] = 0x23;

						byte arg = indexRegister(code, i, 2);
						arg |= (0x3 << 4);

						cpu.ram.memory[currentPos++] = arg;

						int arg2 = std::stoi(code[i + 1]);

						byte b1, b2, b3;
						convertByte3(arg2, b1, b2, b3);
						cpu.ram.memory[currentPos++] = b1;
						cpu.ram.memory[currentPos++] = b2;
						cpu.ram.memory[currentPos++] = b3;

						i += 2;
						line++;
					}
				}else{
					std::cout << "Uknwown data type at instruction >move.h< at line " << line << '\n';
					cpu.ram.Clear();
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
					cpu.ram.Clear();
					break;
				}
			}
			std::cout << "vCPU program part 2 compilation " << (int)(((float)(i + 1) / (float)code.size()) * 100) << "% done..." << '\r';
		}

		std::cout << '\n' << "Part 2 of compilation done..." << '\n' << '\n';

		if(hasStop == false){
			std::cout << "No >stop< instruction found, aborting compilation..." << '\n';
			cpu.ram.Clear();
		}

		if(hasffb == true){
			//std::cout << ffbID << '\n';
			byte b1, b2, b3;
			convertByte3(currentPos, b1, b2, b3);
			cpu.ram.memory[variables[ffbID].position] = b1;
			cpu.ram.memory[variables[ffbID].position + 1] = b2;
			cpu.ram.memory[variables[ffbID].position + 2] = b3;
		}


		finalCP = GadditionalMemory;

		std::cout << "CPU program size is: " << currentPos << " bytes." << '\n';
		std::cout << "Free RAM left: " << (cpu.ram.memory.size() - currentPos) << " bytes." << '\n';
		std::cout << "vPC output:" << '\n' << '\n';
	}
}
