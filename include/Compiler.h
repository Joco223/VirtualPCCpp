#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "CPU.h"

typedef unsigned char byte;

namespace Compiler {
	void Parse(std::vector<std::string>& tokens);
	void splitFile(std::vector<std::string>& code, std::vector<std::string>& tokens);
	void readFile(std::string path, std::vector<std::string>& code);
}
