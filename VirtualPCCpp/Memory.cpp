#include "Memory.h"

#include <iostream> 
#include <new>  

Memory::Memory() {
	memory.resize(64);
	Clear();
}

Memory::Memory(int size) {
	memory.resize(size);
	Clear();
}

void Memory::Clear() {
	for (int i = 0; i < memory.size(); i++) {
		memory[i] = 0;
	}
}

bool Memory::Empty() {
	for (int i = 0; i < memory.size(); i++) {
		if (memory[i] != 0) {
			return false;
		}
	}
	return true;
}

byte Memory::read(u16 position) {
	return memory[position];
}

void Memory::write(u16 position, byte value) {
	memory[position] = value;
}
