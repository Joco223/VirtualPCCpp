#include "Memory.h"

Memory::Memory(int size) {
	memory.resize(size);
	Clear();
}

void Memory::Clear() {
	for (unsigned int i = 0; i < memory.size(); i++) {
		memory[i] = 0;
	}
}

bool Memory::Empty() {
	for (unsigned int i = 0; i < memory.size(); i++) {
		if (memory[i] != 0) {
			return false;
		}
	}
	return true;
}
