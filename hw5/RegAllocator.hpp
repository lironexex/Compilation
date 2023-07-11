#ifndef REG_ALLOCATOR_H
#define REG_ALLOCATOR_H
#include <stdlib.h>
#include "vector"
#include "bp.hpp"
#include <iostream>

using namespace std;

class RegAllocator{
	vector<bool> allocated_regs;
	string reg_prefix;
public:
	RegAllocator();
	int GetRegister();
	void FreeRegister(int reg);
	string GetPrintedRegister(int reg);
};

#endif