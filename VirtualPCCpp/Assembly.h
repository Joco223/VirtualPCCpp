#pragma once

#include <vector>
#include <string>

#include "CPU.h"

typedef unsigned char byte;

namespace Assembly {
	void Compile(std::vector<std::string>& code, CPU& cpu);
}