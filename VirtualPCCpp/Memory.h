#pragma once

typedef unsigned char byte;

#include <vector>

class Memory {
public:
	std::vector<byte> memory;

	Memory();
	Memory(int);

	void Clear();
	bool Empty();
};