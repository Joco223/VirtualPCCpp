#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "CPU.h"

typedef unsigned char byte;

namespace Assembly {
	void Compile(std::vector<std::string>& code, CPU& cpu);
	void readFile(std::string path, std::vector<std::string>& code);
}