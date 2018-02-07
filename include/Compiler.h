#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "CPU.h"

typedef unsigned char byte;

namespace Compiler {
	void Compile(std::vector<std::string>& code, std::vector<std::string>& assembly_code);
	void readFile(std::string path, std::vector<std::string>& code);
}
