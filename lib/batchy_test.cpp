#include "iostream"
#include <iomanip>

#include "batchy.h"

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
/*   6*/   2,   0,   2,   0,   0,   0, // REG_SET_SINGLE
/*  12*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  18*/  19,   1,   1,   0,   0,   0, // STACK_GET
/*  24*/   2,   2,   1,   0,   0,   0, // REG_SET_SINGLE
/*  30*/  30,   0,   1,   0,   0,   0, // BATCHY_CMD
/*  36*/  19,   1,   1,   0,   0,   0, // STACK_GET
/*  42*/   2,   2,   1,   0,   0,   0, // REG_SET_SINGLE
/*  48*/  30,   0,   2,   0,   0,   0, // BATCHY_CMD
/*  54*/   2,   1, 244,   1,   0,   0, // REG_SET_SINGLE
/*  60*/  30,   0,  10,   0,   0,   0, // BATCHY_CMD
/*  66*/  19,   1,   1,   0,   0,   0, // STACK_GET
/*  72*/   2,   2,   0,   0,   0,   0, // REG_SET_SINGLE
/*  78*/  30,   0,   2,   0,   0,   0, // BATCHY_CMD
/*  84*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/*  90*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  96*/   2,   0,   1,   0,   0,   0, // REG_SET_SINGLE
/* 102*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/* 108*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/* 114*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/* 120*/  19,   1,   1,   0,   0,   0, // STACK_GET
/* 126*/   2,   2,   0,   0,   0,   0, // REG_SET_SINGLE
/* 132*/   8,   0,   1,   2,   0,   0, // MATH_EQ
/* 138*/  31,   0,   0,   0,   0,   0, // CONDITION
/* 144*/  20,   0, 156,   0,   0,   0, // JUMP_NORMAL
/* 150*/  20,   0, 158,   1,   0,   0, // JUMP_NORMAL
/* 156*/  19,   1,   4,   0,   0,   0, // STACK_GET
/* 162*/   2,   2,   1,   0,   0,   0, // REG_SET_SINGLE
/* 168*/  30,   0,   2,   0,   0,   0, // BATCHY_CMD
/* 174*/  19,   1,   3,   0,   0,   0, // STACK_GET
/* 180*/  30,   0,  10,   0,   0,   0, // BATCHY_CMD
/* 186*/  19,   1,   4,   0,   0,   0, // STACK_GET
/* 192*/   2,   2,   0,   0,   0,   0, // REG_SET_SINGLE
/* 198*/  30,   0,   2,   0,   0,   0, // BATCHY_CMD
/* 204*/   2,   1, 232,   3,   0,   0, // REG_SET_SINGLE
/* 210*/  19,   2,   3,   0,   0,   0, // STACK_GET
/* 216*/   4,   1,   1,   2,   0,   0, // MATH_SUB
/* 222*/  30,   0,  10,   0,   0,   0, // BATCHY_CMD
/* 228*/  19,   1,   3,   0,   0,   0, // STACK_GET
/* 234*/   2,   2, 232,   3,   0,   0, // REG_SET_SINGLE
/* 240*/  14,   0,   1,   2,   0,   0, // MATH_GREATEREQ
/* 246*/  31,   0,   0,   0,   0,   0, // CONDITION
/* 252*/  20,   0,   8,   1,   0,   0, // JUMP_NORMAL
/* 258*/  20,   0,  50,   1,   0,   0, // JUMP_NORMAL
/* 264*/  19,   2,   2,   0,   0,   0, // STACK_GET
/* 270*/   2,   3,   1,   0,   0,   0, // REG_SET_SINGLE
/* 276*/   3,   2,   2,   3,   0,   0, // MATH_ADD
/* 282*/   2,   3,   2,   0,   0,   0, // REG_SET_SINGLE
/* 288*/   7,   2,   2,   3,   0,   0, // MATH_MOD
/* 294*/  18,   2,   2,   0,   0,   0, // STACK_PUT
/* 300*/  20,   0,  50,   1,   0,   0, // JUMP_NORMAL
/* 306*/  19,   1,   2,   0,   0,   0, // STACK_GET
/* 312*/   2,   2,   1,   0,   0,   0, // REG_SET_SINGLE
/* 318*/   8,   0,   1,   2,   0,   0, // MATH_EQ
/* 324*/  31,   0,   0,   0,   0,   0, // CONDITION
/* 330*/  20,   0,  86,   1,   0,   0, // JUMP_NORMAL
/* 336*/  20,   0, 116,   1,   0,   0, // JUMP_NORMAL
/* 342*/  19,   2,   3,   0,   0,   0, // STACK_GET
/* 348*/   2,   3,  10,   0,   0,   0, // REG_SET_SINGLE
/* 354*/   3,   2,   2,   3,   0,   0, // MATH_ADD
/* 360*/  18,   2,   3,   0,   0,   0, // STACK_PUT
/* 366*/  20,   0, 140,   1,   0,   0, // JUMP_NORMAL
/* 372*/  19,   2,   3,   0,   0,   0, // STACK_GET
/* 378*/   2,   3,  10,   0,   0,   0, // REG_SET_SINGLE
/* 384*/   4,   2,   2,   3,   0,   0, // MATH_SUB
/* 390*/  18,   2,   3,   0,   0,   0, // STACK_PUT
/* 396*/  19,   2,   1,   0,   0,   0, // STACK_GET
/* 402*/  18,   2,   1,   0,   0,   0, // STACK_PUT
/* 408*/  20,   0, 120,   0,   0,   0, // JUMP_NORMAL
/* 414*/  17,   0,   0,   0,   0,   0, // STACK_POP
	};
	batchy.runCommandString(cmdstr, sizeof(cmdstr));
	
	
	
	getchar();
}
