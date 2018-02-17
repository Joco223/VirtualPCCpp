#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "CPU.h"

typedef unsigned char byte;

namespace Assembly {

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

	void Compile(std::vector<std::string>& code, CPU& cpu, std::vector<int>& vValues, std::vector<std::string>& vNames, int& finalCP);
	void readFile(std::string path, std::vector<std::string>& code);
}
