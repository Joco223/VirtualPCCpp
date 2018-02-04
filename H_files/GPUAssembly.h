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
	void Compile(std::vector<std::string>& code, GPU& cpu, std::vector<int>& vValues, std::vector<std::string>& vNames, int newCP);
	void readFile(std::string path, std::vector<std::string>& code);
}