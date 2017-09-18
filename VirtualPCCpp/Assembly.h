#pragma once

#include <vector>
#include <string>

#include "CPU.h"

namespace Assembly {
	void Compile(std::vector<std::string>& code, CPU& cpu);
}