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
	unsigned int position;
	int sDepth;
};

struct jump_pos {
	std::string name;
	unsigned int position;
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

void convertByte4(unsigned int& number, byte& b1, byte& b2, byte& b3, byte& b4) {
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
	}else{
		tmp = std::stoi(argument);
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
		return std::stoi(argument);
	}
}

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

void Compile(std::vector<std::string>& code, std::vector<byte>& compiled, int& startPos, int& interPos, int& interFinishPos, int& interTartgetPos) {
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

	unsigned int currentPos = 0;
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
			}else if (instruction == "d16") {
				currentPos += 2;
				i += 2;
				continue;
			}else if (instruction == "l32") {
				currentPos += 4;
				i += 2;
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
		}else if(instruction == "set") {
			currentPos += 6;
			i += 2;
			line++;
		}else if(instruction == "setPC") {
			currentPos += 2;
			i++;
			line++;
		}else if(instruction == "cpy.rv") {
			currentPos += 3;
			i +=3;
			line++;
		}else if(instruction == "cpy.vr") {
			currentPos += 3;
			i +=3;
			line++;
		}else if(instruction == "cpy.rh") {
			currentPos += 3;
			i +=3;
			line++;
		}else if(instruction == "cpy.hr") {
			currentPos += 3;
			i +=3;
			line++;
		}else if(instruction == "cpy.rr") {
			currentPos += 3;
			i +=3;
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
				compiled[currentPos++] = b1;
				compiled[currentPos++] = b2;
				i += 2;
				line++;
				continue;
			}else if (instruction == "l32") {
				std::string arg1 = code[i + 1];
				std::string arg2 = code[i + 2];

				unsigned int Arg2;

				if(arg2 == "ffb"){
					ffbID = variables.size() - 1;
					hasffb = true;
					Arg2 = 0;
				}else{
					Arg2 = std::stoi(arg2);
				}

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
			}
		}

		if(instruction.compare(0, 5, "move.") == 0 && instruction.length() == 6) {
			if(instruction[5] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x02;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x1 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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
		}else if(instruction.compare(0, 8, "moveP.g.") == 0 && instruction.length() == 9) {
			if(instruction[8] == 's'){
				if(isRegister(code, i)) {
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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
					compiled[currentPos++] = 0x2C;

					byte arg = indexRegister(code, i, 1);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = checkArgType(code[i + 2], variables, scopeDepth);

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

					unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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
				std::cout << "Uknwown data type at instruction >moveP.g< at line " << line << '\n';
				break;
			}
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

				unsigned int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth) - currentPos + 2;

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

				unsigned int arg2 = checkJmpPos(code[i + 2], jumpPositions, scopeDepth) - currentPos + 2;

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

				unsigned int arg2 = checkJmpPos(code[i + 1], jumpPositions, scopeDepth) - currentPos + 2;

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
		}else if(instruction == "load.data") {
			compiled[currentPos++] = 0x2D;

			unsigned int arg2 = checkArgType(code[i + 1], variables, scopeDepth);

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

			unsigned int arg = checkArgType(code[i + 1], variables, scopeDepth);

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

			unsigned int arg = checkArgType(code[i + 1], variables, scopeDepth);

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
		}else if(instruction == "set") {
			compiled[currentPos++] = 0x32;

			byte reg = indexRegister(code, i, 1);
			compiled[currentPos++] = reg;

			unsigned int arg = std::stoi(code[i + 2]);

			byte b1, b2, b3, b4;
			convertByte4(arg, b1, b2, b3, b4);
			compiled[currentPos++] = b1;
			compiled[currentPos++] = b2;
			compiled[currentPos++] = b3;
			compiled[currentPos++] = b4;

			i += 2;
			line++;
		}else if(instruction == "setPC") {
			compiled[currentPos++] = 0x33;

			byte reg = indexRegister(code, i, 1);
			compiled[currentPos++] = reg;

			i++;
			line++;
		}else if(instruction == "cpy.rv") {
			compiled[currentPos++] = 0x34;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "cpy.vr") {
			compiled[currentPos++] = 0x35;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "cpy.rh") {
			compiled[currentPos++] = 0x36;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "cpy.hr") {
			compiled[currentPos++] = 0x37;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "cpy.rr") {
			compiled[currentPos++] = 0x38;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
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
	int startPos = 0;
	int interPos = 0;
	int interFinishPos = 0;
	int interTartgetPos = 0;
	Compile(code, compiledC, startPos, interPos, interFinishPos, interTartgetPos);
	std::ofstream compiledOut;
	std::string name(argv[2]);
	//.cbc - Compiled Byte Code
	std::string name1 = name + ".cbc";
	compiledOut.open(name1);
	compiledOut << startPos << '\n';
	compiledOut << interPos << '\n';
	compiledOut << interFinishPos << '\n';
	compiledOut << interTartgetPos << '\n';
	for(int i = 0; i < compiledC.size(); i++) { compiledOut << (unsigned int)compiledC[i]; compiledOut << '\n'; }
	compiledOut.close();
	return 0;
}
