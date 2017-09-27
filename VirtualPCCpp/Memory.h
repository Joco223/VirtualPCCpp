#pragma once

#include <cstdint>
#include <vector>

typedef unsigned char byte;
typedef uint16_t u16;

class Memory {
public:
	std::vector<byte> memory;

	Memory();
	Memory(int);

	void Clear();
	bool Empty();
};