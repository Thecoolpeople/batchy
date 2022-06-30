#undef _GLIBCXX_DEBUG                // disable run-time bound checking, etc
#pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

#pragma GCC target("bmi,bmi2,lzcnt,popcnt")                      // bit manipulation
#pragma GCC target("aes,pclmul,rdrnd")                           // encryption
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD

#include "iostream"
#include <iomanip>

#include "batchy.h"
#include <chrono>

std::ostream hexcout (std::cout.rdbuf());

void printHEX(unsigned char& c){
	hexcout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex;
	hexcout << (0xFF & c);
}

void printCMD(cmd command){
	for (int i = 0;i < sizeof command.full / sizeof command.full[0]; i++) {
		hexcout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex;
    	hexcout << (0xFF & command.full[i]) << " ";
	}
	
}

void printRegister(BATCHY &b){
	Register* reg = b.getRegister();
	std::cout << "Register Batchy: ";
	for(int i = 0; i < RegCount; i++){
		for(int z = RegSize-1; z >= 0; z--){
			hexcout << std::setfill('0') << std::setw(2) << std::uppercase << std::hex;
			hexcout << (0xFF & reg[i].byte[z]);
		}
		hexcout << " ";
	}
	std::cout << std::endl;
}

int main(){
	std::cout << "BATCHY Test Environment" << std::endl;
	BATCHY batchy;
	
	char cmdstr[] = {
/*   0*/  20,   0,   6,   0,   0,   0, // JUMP_NORMAL
/*   6*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/*  12*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  18*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/*  24*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  30*/  19,   1,   1,   0,   0,   0, // STACK_GET
/*  36*/   2,   2,  16,  39,   0,   0, // REG_SET_SINGLE
/*  42*/  13,   0,   1,   2,   0,   0, // MATH_SMALLER
/*  48*/  31,   0,   0,   0,   0,   0, // CONDITION
/*  54*/  20,   0,  66,   0,   0,   0, // JUMP_NORMAL
/*  60*/  20,   0,  96,   0,   0,   0, // JUMP_NORMAL
/*  66*/  19,   2,   1,   0,   0,   0, // STACK_GET
/*  72*/   2,   3,   1,   0,   0,   0, // REG_SET_SINGLE
/*  78*/   3,   2,   2,   3,   0,   0, // MATH_ADD
/*  84*/  18,   2,   1,   0,   0,   0, // STACK_PUT
/*  90*/  20,   0,  30,   0,   0,   0, // JUMP_NORMAL
/*  96*/  17,   0,   0,   0,   0,   0, // STACK_POP
	};
	
	auto start_time = std::chrono::high_resolution_clock::now();
	batchy.runCommandString(cmdstr, sizeof(cmdstr));
	auto end_time = std::chrono::high_resolution_clock::now();
	
	auto time = end_time - start_time;
	std::cout << time/std::chrono::microseconds(1) << "microseconds";
	
	getchar();
}
