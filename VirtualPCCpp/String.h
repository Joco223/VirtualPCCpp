#pragma once

typedef unsigned char byte;

class String {
public:
	int memPos;
	byte& c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15, c16;

	String();
	String(int, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&, byte&);
};