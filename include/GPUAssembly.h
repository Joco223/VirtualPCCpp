#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <random>

#include "CPU.h"
#include "GPU.h"
#include "GPUCore.h"

typedef unsigned char byte;

namespace GPUAssembly {

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

	void Compile(std::vector<std::string>& code, GPU& gpu, std::vector<variable>& variables);
	void readFile(std::string path, std::vector<std::string>& code);
};
