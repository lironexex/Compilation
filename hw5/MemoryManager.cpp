#include "MemoryManager.hpp"
#include "RegAllocator.hpp"
#include "bp.hpp"
#include "vector"
#include "string"
#include <stdlib.h>

using namespace std;
bool memory_debug = false;

MemoryManager::MemoryManager() : memory_vector(vector<int>(50)) {
	if (memory_debug)
		cout << "MemoryManager Init" << endl;

	for (int i = 0; i < 50; i++)
		memory_vector[i] = -1;
};

void MemoryManager::allocaMemory(int offset, CodeBuffer& code_buffer, RegAllocator& reg_allocator){
	if (memory_debug)
		cout << "allocaMemory freeMemory" << endl;

	memory_vector[offset] = reg_allocator.GetRegister();
	code_buffer.emit("%" + getMemoryPtr(offset, reg_allocator) + " = alloca i32");
}

void MemoryManager::freeMemory(RegAllocator& reg_allocator){
	if (memory_debug) cout << "MemoryManager freeMemory" << endl;
	
	for(int i = 0; i < 50; i++){
		if(memory_vector[i] == -1){
			continue;
		}

		if (memory_debug)
			cout << "freed index: " << i << "with value of: " << memory_vector[i] << endl;

		reg_allocator.FreeRegister(memory_vector[i]);
		memory_vector[i] = -1;
	}
}

string MemoryManager::getMemoryPtr(int offset, RegAllocator& reg_allocator){
	return reg_allocator.GetPrintedRegister(memory_vector[offset]);
}

void MemoryManager::loadINT(string varToLoad, int offset, CodeBuffer &code_buffer, RegAllocator &reg_allocator) {
    code_buffer.emit("%" + getMemoryPtr(offset, reg_allocator) + " = load i32, i32* %" + varToLoad);
}

void MemoryManager::storeINT(string value, int offset, CodeBuffer &code_buffer, RegAllocator &reg_allocator) {
    code_buffer.emit("store i32 " + value + ", i32* %" + getMemoryPtr(offset, reg_allocator));
}
