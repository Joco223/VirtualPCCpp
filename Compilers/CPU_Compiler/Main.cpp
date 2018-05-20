#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

typedef unsigned char byte;

struct variable {
	std::string name;
	int size;
	int position;
	int sDepth;
};

struct jump_pos {
	std::string name;
	int position;
	int sDepth;
};


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

int checkArgType(std::string& argument, std::vector<variable>& vars, int currentDepth) {
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
		if(argument == "regA"	 ) {return 16769025 +  1; };
		if(argument == "regB"	 ) {return 16769025 +  2; };
		if(argument == "regC"	 ) {return 16769025 +  3; };
		if(argument == "regD"	 ) {return 16769025 +  4; };
		if(argument == "regE"	 ) {return 16769025 +  5; };
		if(argument == "regF"	 ) {return 16769025 +  6; };
		if(argument == "regG"	 ) {return 16769025 +  7; };
		if(argument == "regH"	 ) {return 16769025 +  8; };
		if(argument == "regI"	 ) {return 16769025 +  9; };
		if(argument == "regJ"	 ) {return 16769025 + 10; };
		if(argument == "regK"	 ) {return 16769025 + 11; };
		if(argument == "regL"	 ) {return 16769025 + 12; };
		if(argument == "regInter") {return 16769025 + 13; };
		if(argument == "regTimer") {return 16769025 + 14; };
	}
}

/*int checkFunction(std::string function, std::vector<stack_object>& stack) {
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
}*/

void readFile(std::string path, std::vector<std::string>& code) {
	std::string line;
	std::ifstream myfile(path);
	std::string prefix = "//";

	if (myfile.is_open()){
		while (getline (myfile, line)) {
			if (!(line[0] == '/' && line[1] == '/')) {
				if (line != "") {
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
	return code[i + 1] == "regA" 	 || code[i + 1] == "regB" || code[i + 1] == "regC" || code[i + 1] == "regD" ||
	   	   code[i + 1] == "regE" 	 || code[i + 1] == "regF" || code[i + 1] == "regG" || code[i + 1] == "regH" ||
	   	   code[i + 1] == "regI" 	 || code[i + 1] == "regJ" || code[i + 1] == "regK" || code[i + 1] == "regL" ||
		   code[i + 1] == "regInter" || code[i + 1] == "regTimer";
}

int indexRegister(std::vector<std::string>& code, int i, int k){
	if(code[i + k] == "regA"	) {return  0;}
	if(code[i + k] == "regB"	) {return  1;}
	if(code[i + k] == "regC"	) {return  2;}
	if(code[i + k] == "regD"	) {return  3;}
	if(code[i + k] == "regE"	) {return  4;}
	if(code[i + k] == "regF"	) {return  5;}
	if(code[i + k] == "regG"	) {return  6;}
	if(code[i + k] == "regH"	) {return  7;}
	if(code[i + k] == "regI"	) {return  8;}
	if(code[i + k] == "regJ"	) {return  9;}
	if(code[i + k] == "regK"	) {return 10;}
	if(code[i + k] == "regL"	) {return 11;}
	if(code[i + k] == "regInter") {return 12;}
	if(code[i + k] == "regTimer") {return 13;}
	if(code[i + k] == "regFrame") {return 14;}
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
	if(code[i + k] == "interRegI") {return  8;}
	if(code[i + k] == "interRegJ") {return  9;}
	if(code[i + k] == "interRegK") {return 10;}
	if(code[i + k] == "interRegL") {return 11;}
}

void Compile(std::vector<std::string>& code, std::vector<byte>& compiled, std::vector<byte>& GPUcompiled, int& startPos, int& interPos, int& interFinishPos, int& interTartgetPos) {
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
			currentPos += 5;
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
			}else if (instruction == "d16.g") {
				currentPosG += 2;
				i += 2;
				continue;
			}else if (instruction == "l32") {
				currentPos += 4;
				i += 2;
				continue;
			}else if (instruction == "pt") {
				currentPos += 4;
				i += 2;
				continue;
			}else if (instruction == "pt.g") {
				currentPosG += 4;
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
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[5] == 'd') {
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[5] == 'l') {
				currentPos += 6;
				i += 2;
				line++;
			}else{
				std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
			currentPos += 6;
			i += 2;
			line++;
		}else if(instruction.compare(0, 8, "moveP.h.") == 0 && instruction.length() == 9) {
			currentPos += 6;
			i += 2;
			line++;
		}else if(instruction.compare(0, 8, "moveP.g.") == 0 && instruction.length() == 9) {
			currentPos += 6;
			i += 2;
			line++;
		}else if(instruction.compare(0, 6, "moveO.") == 0) {
			if(instruction[6] == 's'){
				currentPos += 10;
				i += 3;
				line++;
			}else if(instruction[6] == 'd') {
				currentPos += 10;
				i += 3;
				line++;
			}else if(instruction[6] == 'l') {
				currentPos += 10;
				i += 3;
				line++;
			}else{
				std::cout << "Uknwown data type at instruction >moveO< at line " << line << '\n';
				break;
			}
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
				break;
			}
		}else if (instruction.compare(0, 4, "jmp.") == 0) {
			if(instruction[4] == 'z') {
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[4] == 'o') {
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[4] == 'u') {
				currentPos += 6;
				i++;
				line++;
			}else{
				std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
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
					break;
				}
			}else {
				std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
				break;
			}
		}else if (instruction == "gpu.str") {
			currentPos += 2;
			i += 2;
			line++;
		}else if(instruction == "stop") {
			hasStop = true;
			currentPos++;
			line++;
		}else if(instruction == "sb.setID") {
			currentPos += 7;
			i += 3;
			line++;
		}else if(instruction == "sb.setCB") {
			currentPos += 7;
			i += 3;
			line++;
		}else if(instruction == "sb.setCF") {
			currentPos += 7;
			i += 3;
			line++;
		}/*else if(!instruction.compare(0, 2, "fn")) {
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
		}*/else if(instruction.compare(0, 7, "move.g.") == 0) {
			if(instruction[7] == 's'){
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[7] == 'd') {
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[7] == 'l') {
				currentPos += 6;
				i += 2;
				line++;
			}else{
				std::cout << "Uknwown data type at instruction >move.g.< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 7, "move.h.") == 0) {
			if(instruction[7] == 's'){
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[7] == 'd') {
				currentPos += 6;
				i += 2;
				line++;
			}else if(instruction[7] == 'l') {
				currentPos += 6;
				i += 2;
				line++;
			}else{
				std::cout << "Uknwown data type at instruction >move.g.< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 8, "moveO.g.") == 0) {
			if(instruction[8] == 's'){
				currentPos += 10;
				i += 3;
				line++;
			}else if(instruction[8] == 'd') {
				currentPos += 10;
				i += 3;
				line++;
			}else if(instruction[8] == 'l') {
				currentPos += 10;
				i += 3;
				line++;
			}else{
				std::cout << "Uknwown data type at instruction >moveO.g.< at line " << line << '\n';
				break;
			}
		}else if(instruction == "load.data") {
			currentPos += 5;
			i++;
			line++;
		}else if(instruction == "load.address") {
			currentPos += 3;
			i += 3;
			line++;
		}else if(instruction == "load.socket"){
			currentPos += 5;
			i++;
			line++;
		}else if(instruction == "toggle.socket"){
			currentPos += 5;
			i++;
			line++;
		}else if(instruction == "send"){
			currentPos++;
			line++;
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
				std::cout << "Compilation terminated." << '\n';
				break;
			}
		}

	}

	compiled.resize(currentPos);

	currentPos = 0;
	currentPosG = 0;
	additionalMemory = 0;
	line = 0;
	dataM = false;
	interM = false;
	scopeDepth = 0;
	startPos = 0;
	interPos = 0;
	interFinishPos = 0;
	interTartgetPos = 0;

	for (unsigned int i = 0; i < code.size(); i++) {

		std::string instruction = code[i];

		if (instruction == "program.start") {
			startPos = currentPos;
			continue;
		}

		if(instruction == "data.") {
			dataM = true;
			continue;
		}

		if (instruction == "interrupt.s") {
			interM = true;
			interPos = currentPos;
			continue;
		}

		if (instruction == "interrupt.e") {
			interM = false;
			compiled[currentPos++] = 0x22;
			interTartgetPos = currentPos;
			compiled[currentPos++] = 0;
			compiled[currentPos++] = 0;
			compiled[currentPos++] = 0;
			compiled[currentPos++] = 0;
			interFinishPos = currentPos;
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

				compiled[currentPos++] = (byte)Arg2;
				i += 2;
				line++;
				continue;
			}/*else if (instruction == "s8.g") {
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
			}*/else if (instruction == "d16") {
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
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				i += 2;
				line++;
				continue;
			}/*else if (instruction == "d16.g") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				int Arg2 = std::stoi(arg2);

				variable temp;
				temp.name = arg1;
				temp.position = currentPosG;
				temp.sDepth = scopeDepth;
				temp.size = 2;
				gVariables.push_back(temp);

				byte b1, b2;
				convertByte2(Arg2, b1, b2);
				gpu.vRam.memory[currentPosG++] = b1;
				gpu.vRam.memory[currentPosG++] = b2;
				i += 2;
				line++;
				continue;
			}*/else if (instruction == "pt") {
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


				byte b1, b2, b3, b4;
				convertByte4(Arg2, b1, b2, b3, b4);
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				compiled[currentPos++] = b3;
				compiled[currentPos++] = b4;
				i += 2;
				line++;
				continue;
			}/*else if (instruction == "pt.g") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				variable temp;
				temp.name = arg1;
				temp.position = currentPos;
				temp.sDepth = scopeDepth;
				temp.size = 3;
				gVariables.push_back(temp);

				int Arg2 = std::stoi(arg2);

				byte b1, b2, b3, b4;
				convertByte4(Arg2, b1, b2, b3, b4);
				gpu.vRam.memory[currentPosG++] = b1;
				gpu.vRam.memory[currentPosG++] = b2;
				gpu.vRam.memory[currentPosG++] = b3;
				gpu.vRam.memory[currentPosG++] = b4;
				i += 2;
				line++;
				continue;
			}*/else if (instruction == "l32") {
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
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				compiled[currentPos++] = b3;
				compiled[currentPos++] = b4;
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
					compiled[currentPos++] = (byte)element;
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
					compiled[currentPos++] = 0x02;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[5] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x02;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[5] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x02;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 6, "moveP.") == 0 && instruction.length() == 7) {
			if(instruction[6] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x28;

					byte arg = indexRegister(code, i, 1);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x27;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[6] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x28;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x27;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[6] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x28;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x27;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >moveP< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 8, "moveP.h.") == 0 && instruction.length() == 9) {
			if(instruction[8] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2A;

					byte arg = indexRegister(code, i, 1);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x29;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[8] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2A;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x29;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[8] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2A;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x29;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >moveP.h< at line " << line << '\n';
				break;
			}
		}/*else if(instruction.compare(0, 8, "moveP.g.") == 0 && instruction.length() == 9) {
			if(instruction[8] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x2B;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], , variables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					currentPos++;

					i += 2;
					line++;
				}
			}else if(instruction[8] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x2B;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					currentPos++;

					i += 2;
					line++;
				}
			}else if(instruction[8] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x2B;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					currentPos++;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >moveP.h< at line " << line << '\n';
				break;
			}
		}*/else if(instruction.compare(0, 6, "moveO.") == 0) {
			if(instruction[6] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x03;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 3], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x04;

					byte arg = indexRegister(code, i, 3);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}
			}else if(instruction[6] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x03;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 3], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x04;

					byte arg = indexRegister(code, i, 3);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}
			}else if(instruction[6] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x03;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 3], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x04;

					byte arg = indexRegister(code, i, 3);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					int arg3 = checkArgType(code[i + 2], variables, scopeDepth);

					byte b5, b6, b7, b8;
					convertByte4(arg3, b5, b6, b7, b8);
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;
					compiled[currentPos++] = b7;
					compiled[currentPos++] = b8;

					i += 3;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >move< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 5, "moveP") == 0 && instruction.length() == 6) {
			if(isRegister(code, i)) {
				std::string regA = code[i + 1];
				std::string paramA = code[i + 2];

				byte arg = indexRegister(code, i, 1);
				arg |= (checkArgType(code[i + 2], variables, scopeDepth) << 4);

				compiled[currentPos++] = 0x19;
				compiled[currentPos++] = arg;
			}else{
				std::string regA = code[i + 2];
				std::string paramA = code[i + 1];

				byte arg = indexRegister(code, i, 2);
				arg |= (checkArgType(code[i + 1], variables, scopeDepth) << 4);

				compiled[currentPos++] = 0x18;
				compiled[currentPos++] = arg;
			}
			i += 2;
			line++;
		}else if(instruction.compare(0, 5, "moveI") == 0) {
			if(!isRegister(code, i)) {
				std::string regA = code[i + 1];
				std::string paramA = code[i + 2];

				byte arg = indexRegister(code, i, 2);
				arg |= (indexInterRegister(code, i, 1) << 4);

				compiled[currentPos++] = 0x21;
				compiled[currentPos++] = arg;
			}else{
				std::cout << "Can't write to interrupt registers at line " << line << '\n';
			}
			i += 2;
			line++;
		}else if(instruction == "inc") {
			compiled[currentPos++] = 0x12;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "dec") {
			compiled[currentPos++] = 0x15;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "cot") {
			compiled[currentPos++] = 0x13;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "clr") {
			compiled[currentPos++] = 0x17;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if (instruction == "add") {
			compiled[currentPos++] = 0x06;

			byte arg = indexRegister(code, i, 1);
			byte arg2 = indexRegister(code, i, 2);
			arg |= (arg2 << 4);

			compiled[currentPos++] = arg;

			i += 2;
			line++;
		}else if (instruction == "mod") {
			compiled[currentPos++] = 0x25;

			byte arg = indexRegister(code, i, 1);
			byte arg2 = indexRegister(code, i, 2);
			arg |= (arg2 << 4);

			compiled[currentPos++] = arg;

			i += 2;
			line++;
		}else if (instruction == "sub") {
			compiled[currentPos++] = 0x07;

			byte arg = indexRegister(code, i, 1);
			byte arg2 = indexRegister(code, i, 2);
			arg |= (arg2 << 4);

			compiled[currentPos++] = arg;

			i += 2;
			line++;
		}else if (instruction.compare(0, 4, "mlt.") == 0) {
			if(instruction[4] == 's'){
				compiled[currentPos++] = 0x08;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				compiled[currentPos++] = arg;

				i += 2;
				line++;
			}else if(instruction[4] == 'u') {
				compiled[currentPos++] = 0x09;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				compiled[currentPos++] = arg;

				i += 2;
				line++;
			}else{
				std::cout << "Uknwown sign type at instruction >mlt< at line " << line << '\n';
				break;
			}
		}else if (instruction.compare(0, 4, "div.") == 0) {
			if(instruction[4] == 's'){
				compiled[currentPos++] = 0x0A;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				compiled[currentPos++] = arg;

				i += 2;
				line++;
			}else if(instruction[4] == 'u') {
				compiled[currentPos++] = 0x0B;

				byte arg = indexRegister(code, i, 1);
				byte arg2 = indexRegister(code, i, 2);
				arg |= (arg2 << 4);

				compiled[currentPos++] = arg;

				i += 2;
				line++;
			}else{
				std::cout << "Uknwown sign type at instruction >div< at line " << line << '\n';
				break;
			}
		}else if (instruction.compare(0, 4, "jmp.") == 0) {
			if(instruction[4] == 'z') {
				compiled[currentPos++] = 0x11;

				byte arg = indexRegister(code, i, 1);
				arg |= (0x0 << 4);

				compiled[currentPos++] = arg;

				int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth);

				byte b1, b2, b3, b4;
				convertByte4(arg2, b1, b2, b3, b4);
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				compiled[currentPos++] = b3;
				compiled[currentPos++] = b4;

				i += 2;
				line++;
			}else if(instruction[4] == 'o') {
				compiled[currentPos++] = 0x11;

				byte arg = indexRegister(code, i, 1);
				arg |= (0x1 << 4);

				compiled[currentPos++] = arg;

				int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth);

				byte b1, b2, b3, b4;
				convertByte4(arg2, b1, b2, b3, b4);
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				compiled[currentPos++] = b3;
				compiled[currentPos++] = b4;

				i += 2;
				line++;
			}else if(instruction[4] == 'u') {
				compiled[currentPos++] = 0x11;

				byte arg = 0;
				arg |= (0x2 << 4);

				compiled[currentPos++] = arg;

				int arg2 = checkJmpPos(code[i + 1], jumpPositions, scopeDepth);

				byte b1, b2, b3, b4;
				convertByte4(arg2, b1, b2, b3, b4);
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				compiled[currentPos++] = b3;
				compiled[currentPos++] = b4;

				i++;
				line++;
			}else{
				std::cout << "Uknwown jump type at instruction >jmp< at line " << line << '\n';
				break;
			}
		}else if (instruction.compare(0, 4, "cmp.") == 0) {
			if(instruction[4] == 's' && instruction[5] == '.') {
				if(instruction[6] == 'b') {
					compiled[currentPos++] = 0x0C;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[6] == 'b' && instruction[7] == 'e') {
					compiled[currentPos++] = 0x0E;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[6] == 'e') {
					compiled[currentPos++] = 0x10;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
					break;
				}
			}else if(instruction[4] == 'u' && instruction[5] == '.') {
				if(instruction[6] == 'b') {
					compiled[currentPos++] = 0x0D;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[6] == 'b' && instruction[7] == 'e') {
					compiled[currentPos++] = 0x0F;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else if(instruction[6] == 'e') {
					compiled[currentPos++] = 0x10;

					byte arg = indexRegister(code, i, 1);
					arg |= (indexRegister(code, i, 2) << 4);

					compiled[currentPos++] = arg;

					i += 2;
					line++;
				}else{
					std::cout << "Uknwown comparison type at instruction >cmp< at line " << line << '\n';
					break;
				}
			}else {
				std::cout << "Uknown sign at instruction >cmp< at line " << line << '\n';
				break;
			}
		}else if (instruction == "gpu.str") {
			compiled[currentPos++] = 0x1C;

			byte arg1 = indexRegister(code, i, 1);
			arg1 |= (indexRegister(code, i , 2) << 4);

			compiled[currentPos++] = arg1;

			i += 2;
			line++;
		}else if(instruction == "stop") {
			compiled[currentPos++] = 0x05;
			line++;
		}else if(instruction == "sb.setID") {
			compiled[currentPos++] = 0x16;

			byte arg1 = indexRegister(code, i, 1);
			arg1 |= (indexRegister(code, i , 2) << 4);

			compiled[currentPos++] = arg1;

			compiled[currentPos++] = 0x0;

			int arg2 = checkArgType(code[i + 3], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg2, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i += 3;
			line++;
		}else if(instruction == "sb.setCB") {
			compiled[currentPos++] = 0x16;

			byte arg1 = indexRegister(code, i, 1);
			arg1 |= (indexRegister(code, i , 2) << 4);

			compiled[currentPos++] = arg1;

			compiled[currentPos++] = 0x1;

			int arg2 = checkArgType(code[i + 3], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg2, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i += 3;
			line++;
		}else if(instruction == "sb.setCF") {
			compiled[currentPos++] = 0x16;

			byte arg1 = indexRegister(code, i, 1);
			arg1 |= (indexRegister(code, i , 2) << 4);

			compiled[currentPos++] = arg1;

			compiled[currentPos++] = 0x2;

			int arg2 = checkArgType(code[i + 3], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg2, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i += 3;
			line++;
		}/*else if(!instruction.compare(0, 2, "fn")) {
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
					compiled[currentPos++] = 0x1E;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x1D;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}
			}else if(instruction[7] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x1E;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x1D;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}
			}else if(instruction[7] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x1E;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x1D;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >move.g< at line " << line << '\n';
				break;
			}
		}else if(instruction.compare(0, 8, "moveO.g.") == 0) {
			if(instruction[8] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x1F;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 3], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x20;

					byte arg = indexRegister(code, i, 3);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}
			}else if(instruction[8] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x1F;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 3], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x20;

					byte arg = indexRegister(code, i, 3);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}
			}else if(instruction[8] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x1F;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 3], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}else{
					compiled[currentPos++] = 0x20;

					byte arg = indexRegister(code, i, 3);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = checkArgType(code[i + 1], gVariables, scopeDepth);

					byte b1, b2, b3;
					convertByte3(arg2, b1, b2, b3);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;

					int arg3 = checkArgType(code[i + 2], gVariables, scopeDepth);

					byte b4, b5, b6;
					convertByte3(arg3, b4, b5, b6);
					compiled[currentPos++] = b4;
					compiled[currentPos++] = b5;
					compiled[currentPos++] = b6;

					i += 3;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >moveO.g< at line " << line << '\n';
				break;
			}
		}*/else if(instruction.compare(0, 7, "move.h.") == 0) {
			if(instruction[7] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x24;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 2]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x23;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 1]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[7] == 'd') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x24;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 2]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x23;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 1]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else if(instruction[7] == 'l') {
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x24;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 2]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}else{
					compiled[currentPos++] = 0x23;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					int arg2 = std::stoi(code[i + 1]);

					byte b1, b2, b3, b4;
					convertByte4(arg2, b1, b2, b3, b4);
					compiled[currentPos++] = b1;
					compiled[currentPos++] = b2;
					compiled[currentPos++] = b3;
					compiled[currentPos++] = b4;

					i += 2;
					line++;
				}
			}else{
				std::cout << "Uknwown data type at instruction >move.h< at line " << line << '\n';
				break;
			}
		}else if(instruction == "load.data") {
			compiled[currentPos++] = 0x2D;

			int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg2, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i++;
			line++;
		}else if(instruction == "load.address") {
			compiled[currentPos++] = 0x2E;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "load.socket"){
			compiled[currentPos++] = 0x30;

			int arg = checkArgType(code[i + 1], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i++;
			line++;
		}else if(instruction == "toggle.socket"){
			compiled[currentPos++] = 0x2F;

			int arg = checkArgType(code[i + 1], variables, scopeDepth);

			byte b1, b2, b3, b4;
			convertByte4(arg, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i++;
			line++;
		}else if(instruction == "send"){
			compiled[currentPos++] = 0x31;
			line++;
		}else{
			if (instruction.back() == ':') {
				line++;
			}else{
				std::cout << "Unknown instruction >" << instruction << "< at line " << line << '\n';
				std::cout << "Compilation terminated." << '\n';
				break;
			}
		}
	}

	if(hasStop == false){
		std::cout << "No >stop< instruction found, aborting compilation..." << '\n';
	}

	if(hasffb == true){
		byte b1, b2, b3, b4;
		convertByte4(currentPos, b1, b2, b3, b4);
		compiled[variables[ffbID].position] = b1;
		compiled[variables[ffbID].position + 1] = b2;
		compiled[variables[ffbID].position + 2] = b3;
		compiled[variables[ffbID].position + 3] = b4;
	}

	std::cout << "CPU program size is: " << currentPos << " bytes." << '\n';
}

int main(int argc, char* argv[]) {
	std::vector<std::string> code;
	readFile(argv[1], code);
	std::vector<byte> compiledC;
	std::vector<byte> GPUcompiled;
	int startPos = 0;
	int interPos = 0;
	int interFinishPos = 0;
	int interTartgetPos = 0;
	Compile(code, compiledC, GPUcompiled, startPos, interPos, interFinishPos, interTartgetPos);
	std::ofstream compiledOut;
	std::ofstream GPUcompiledOut;
	std::string name(argv[2]);
	//.cbc - Compiler Byte Code
	//.gdc - GPU Data Code
	std::string name1 = name + ".cbc";
	std::string name2 = name + "_GPU.gdc";
	compiledOut.open(name1);
	GPUcompiledOut.open(name2);
	compiledOut << startPos << '\n';
	compiledOut << interPos << '\n';
	compiledOut << interFinishPos << '\n';
	compiledOut << interTartgetPos << '\n';
	for(int i = 0; i < compiledC.size(); i++) { compiledOut << (unsigned int)compiledC[i]; compiledOut << '\n'; }
	for(int i = 0; i < GPUcompiled.size(); i++) { GPUcompiledOut << (unsigned int)GPUcompiled[i]; GPUcompiledOut << '\n'; }
	compiledOut.close();
	GPUcompiledOut.close();
	return 0;
}
