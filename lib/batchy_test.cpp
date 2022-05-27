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
		 20,   0, 120,   0,   0,   0,
		  2,   0,  10,   0,   0,   0,
		 16,   0,   0,   0,   0,   0,
		 19,   0,   1,   0,   0,   0,
		 16,   0,   0,   0,   0,   0,
		 19,   1,   2,   0,   0,   0,
		 19,   2,   1,   0,   0,   0,
		  5,   1,   1,   2,   0,   0,
		 19,   2,   1,   0,   0,   0,
		  5,   1,   1,   2,   0,   0,
		 19,   2,   2,   0,   0,   0,
		  4,   0,   1,   2,   0,   0,
		 16,   0,   0,   0,   0,   0,
		  2,   1,   4,   0,   0,   0,
		  2,   2,   1,   0,   0,   0,
		 30,   0,   1,   0,   0,   0,
		 17,   0,   0,   0,   0,   0,
		 17,   0,   0,   0,   0,   0,
		 17,   0,   0,   0,   0,   0,
		 22,   0,   0,   0,   0,   0,
		  2,   1, 128,  37,   0,   0,
		 30,   0,  12,   0,   0,   0,
		  2,   0,  10,   0,   0,   0,
		 16,   0,   0,   0,   0,   0,
		 19,   0,   1,   0,   0,   0,
		 16,   0,   0,   0,   0,   0,
		 30,   0,  13,   0,   0,   0,
		 21,   0,   6,   0,   0,   0,
		 30,   0,  13,   0,   0,   0,
		  2,   1,   4,   0,   0,   0,
		  2,   2,   1,   0,   0,   0,
		 30,   0,   2,   0,   0,   0,
		  2,   1, 232,   3,   0,   0,
		 30,   0,  10,   0,   0,   0,
		  2,   1,   4,   0,   0,   0,
		  2,   2,   0,   0,   0,   0,
		 30,   0,   2,   0,   0,   0,
		  2,   1, 232,   3,   0,   0,
		 30,   0,  10,   0,   0,   0,
		 30,   0,  13,   0,   0,   0, 
	};
	batchy.runCommandString(cmdstr, sizeof(cmdstr));
	
	
	
	getchar();
}
