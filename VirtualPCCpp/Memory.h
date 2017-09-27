#pragma once

#include <cstdint>
#include <vector>

typedef unsigned char byte;
typedef uint16_t u16;

class Memory {
private:
	std::vector<byte> memory;
public:

	Memory();
	Memory(int);

	void Clear();
	bool Empty();

	byte read(u16);
	void write(u16, byte);
};