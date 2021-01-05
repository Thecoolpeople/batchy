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
	
	cmd uno = {0,1,0,1};
		
	printRegister(batchy);
	//set 1
	batchy.runCommand((cmd){2,1,1,0,0,0});
	printRegister(batchy);
	//set 2
	batchy.runCommand((cmd){2,2,2,1,0,0});
	printRegister(batchy);
	//add_register
	batchy.runCommand((cmd){3,0,1,2,0,0});
	printRegister(batchy);
	//sub_register
	batchy.runCommand((cmd){4,0,1,2,0,0});
	printRegister(batchy);
	//clear 1
	batchy.runCommand((cmd){1,1,0,0,0,0});
	printRegister(batchy);
	//clear 2
	batchy.runCommand((cmd){1,2,0,0,0,0});
	printRegister(batchy);
	//clear 0
	batchy.runCommand((cmd){1,0,0,0,0,0});
	printRegister(batchy);
	
	char cmdstr[] = {
		2,1,255,0,0,0,	//set 1
		2,2,2,1,0,0,	//set 2
		3,0,1,2,0,0,	//add_register
		4,0,1,2,0,0,	//sub_register
		1,1,0,0,0,0,	//clear 1
		1,2,0,0,0,0		//clear 0
	};
	batchy.runCommandString(cmdstr, sizeof(cmdstr));
	printRegister(batchy);
	
	
	//function tempi
	batchy.runCommand((cmd){9,0,0,0,0,0});
	printRegister(batchy);
	
	//function GPIO digital mode
	batchy.runCommand((cmd){2,1,1,0,0,1});	//set reg1 = 1
	batchy.runCommand((cmd){2,2,1,0,0,1});	//set reg2 = 1
	batchy.runCommand((cmd){9,0,1,0,0,1});
	printRegister(batchy);
	//function GPIO digital set
	batchy.runCommand((cmd){9,0,0,0,0,2});
	printRegister(batchy);
	//function GPIO digital read
	batchy.runCommand((cmd){9,0,0,0,0,3});
	printRegister(batchy);
	
	getchar();
}
