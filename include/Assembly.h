#pragma once

#include <string>
#include <algorithm>

#include "CPU.h"

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

	void Compile(std::vector<std::string>& code, CPU& cpu, GPU& gpu, std::vector<variable>& gVariables, int& finalCP);
	void readFile(std::string path, std::vector<std::string>& code);
};
