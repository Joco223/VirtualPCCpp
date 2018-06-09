#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>

typedef unsigned char byte;

struct variable {
	std::string name;
	int size;
	int value;
	int sDepth;
};

struct jump_pos {
	std::string name;
	int position;
	int sDepth;
};

std::vector<std::string> lines;

void convertByte2(unsigned int& number, byte& b1, byte& b2) {
	b1 = number & 0xff;
	b2 = (byte)(number >> 8);
}

void convertByte3(unsigned int& number, byte& b1, byte& b2, byte& b3) {
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

std::vector<byte> Compile(std::vector<std::string>& code) {
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

	std::vector<byte> compiled;

	for (unsigned int i = 0; i < code.size(); i++) {

		std::string instruction = code[i];

		if(instruction.compare(0, 5, "move.") == 0) {
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
		}else if (instruction.compare(0, 5, "displ") == 0) {
			i += 3;
			currentPos += 3;
			line++;
		}else if(instruction == "moveP.s.r") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "moveP.d.r") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "moveP.l.r") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "moveP.s.w") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "moveP.d.w") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "moveP.l.w") {
			currentPos += 3;
			i += 2;
			line++;
		}else if(instruction == "set") {
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
				break;
			}
		}

	}

	compiled.resize(currentPos);
	currentPos = 0;
	line = 0;

	for (unsigned int i = 0; i < code.size(); i++) {

		std::string instruction = code[i];

		if(instruction.compare(0, 5, "move.") == 0) {
			if(instruction[5] == 's'){
				if(isRegister(code, i)) {
					//No write to single memory position in vram
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (1 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = std::stoi(code[i + 1]);

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
					//No write to single memory position in vram
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x2 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = std::stoi(code[i + 1]);

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
					//No write to single memory position in vram
				}else{
					compiled[currentPos++] = 0x01;

					byte arg = indexRegister(code, i, 2);
					arg |= (0x3 << 4);

					compiled[currentPos++] = arg;

					unsigned int arg2 = std::stoi(code[i + 1]);

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
		}else if(instruction == "inc") {
			compiled[currentPos++] = 0x12;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "dec") {
			compiled[currentPos++] = 0x13;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "clr") {
			compiled[currentPos++] = 0x14;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "g.IDX") {
			compiled[currentPos++] = 0x16;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "g.IDY") {
			compiled[currentPos++] = 0x17;

			byte arg = indexRegister(code, i, 1);

			compiled[currentPos++] = arg;

			i++;
			line++;
		}else if(instruction == "g.ID") {
			compiled[currentPos++] = 0x18;

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
			compiled[currentPos++] = 0x19;

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

				unsigned int arg2 = checkJmpPos(code[i + 2], jumpPositions, 0);

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

				unsigned int arg2 = checkJmpPos(code[i + 2], jumpPositions, 0);

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

				unsigned int arg2 = checkJmpPos(code[i + 1], jumpPositions, 0);

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
		}else if (instruction.compare(0, 5, "displ") == 0) {
			compiled[currentPos++] = 0x15;

			byte arg = indexRegister(code, i, 1);
			arg |= (indexRegister(code, i, 2) << 4);
			byte arg2 = indexRegister(code, i, 3);

			compiled[currentPos++] = arg;
			compiled[currentPos++] = arg2;

			i += 3;
			line++;
		}else if(instruction == "moveP.s.r") {
			compiled[currentPos++] = 0x1A;
			byte arg = indexRegister(code, i, 1);
			arg |= (1 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "moveP.d.r") {
			compiled[currentPos++] = 0x1A;
			byte arg = indexRegister(code, i, 1);
			arg |= (2 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "moveP.l.r") {
			compiled[currentPos++] = 0x1A;
			byte arg = indexRegister(code, i, 1);
			arg |= (3 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "moveP.s.w") {
			compiled[currentPos++] = 0x1B;
			byte arg = indexRegister(code, i, 1);
			arg |= (1 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "moveP.d.w") {
			compiled[currentPos++] = 0x1B;
			byte arg = indexRegister(code, i, 1);
			arg |= (2 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "moveP.l.w") {
			compiled[currentPos++] = 0x1B;
			byte arg = indexRegister(code, i, 1);
			arg |= (3 << 4);
			compiled[currentPos++] = arg;
			compiled[currentPos++] = indexRegister(code, i, 2);
			i += 2;
			line++;
		}else if(instruction == "set") {
			compiled[currentPos++] = 0x1C;

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
				break;
			}
		}

	}

	if(currentPos > 0){
		std::cout << "GPU program size is: " << currentPos << " bytes" << '\n';
		std::cout << '\n';
	}

	return compiled;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> code;
	readFile(argv[1], code);
	std::vector<byte> compiledC = Compile(code);
	std::ofstream compiledOut;
	std::string name(argv[2]);
	//.cgbc - Compiled GPU Byte Code
	std::string name1 = name + ".cgbc";
	compiledOut.open(name1);
	for(int i = 0; i < compiledC.size(); i++) { compiledOut << (unsigned int)compiledC[i]; compiledOut << '\n'; }
	compiledOut.close();
	return 0;
}
