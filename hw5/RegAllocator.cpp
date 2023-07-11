#include "RegAllocator.hpp"

#include <stdlib.h>

using namespace std;

RegAllocator::RegAllocator() : allocated_regs(vector<bool>()), reg_prefix("reg"){};

int RegAllocator::GetRegister(){
	int chosen_reg = 0;
	int allocated_size = allocated_regs.size();
	if(allocated_size == 0){
		allocated_regs.push_back(true);
		return chosen_reg = 0;
	}

	while(!(chosen_reg == allocated_size)){
		if(allocated_regs[chosen_reg] == false){
			allocated_regs[chosen_reg] = true;
			return chosen_reg;
		}
		else{
			chosen_reg++;
		}
	}

	allocated_regs.push_back(true);
	return chosen_reg;
}

void RegAllocator::FreeRegister(int reg_to_remove){
	allocated_regs[reg_to_remove] = false;
}

string RegAllocator::GetPrintedRegister(int reg_to_print){
	return reg_prefix + to_string(reg_to_print);
}