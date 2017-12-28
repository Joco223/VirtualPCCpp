#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "CPU.h"
#include "GPU.h"
#include "GPUCore.h"

typedef unsigned char byte;

namespace GPUAssembly {
	void Compile(std::vector<std::string>& code, GPU& cpu);
	void readFile(std::string path, std::vector<std::string>& code);
}