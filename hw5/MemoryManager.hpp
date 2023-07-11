#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <stdlib.h>
#include "vector"
#include <iostream>
#include "RegAllocator.hpp"
#include "bp.hpp"

using namespace std;

class MemoryManager{
	vector<int> memory_vector;

public:
	MemoryManager();
	void freeMemory(RegAllocator& reg_allocator);
	void allocaMemory(int offset, CodeBuffer& code_buffer, RegAllocator& reg_allocator);
	string getMemoryPtr(int offset, RegAllocator& reg_allocator);
	void loadINT(string varToLoad, int offset, CodeBuffer& code_buffer, RegAllocator& reg_allocator);
	void storeINT(string value, int offset, CodeBuffer& code_buffer, RegAllocator& reg_allocator);
	int offset_zero;
};

#endif