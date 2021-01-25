#ifndef BATCHY_H
#define BATCHY_H

#include "batchy_defines.h"

/*
#if RegSize == 1

#elif RegSize == 2

#elif RegSize == 3

#elif RegSize == 4

#endif
*/

#if RegSize == 2
	struct uint24_t{
		uint32_t value:24;
	};
#endif

struct cmd_char{
	unsigned char id;
	unsigned char reg;
	unsigned char parameter[RegSize];
};
union cmd{
	unsigned char full[RegSize+2];
	cmd_char splited;
};
union Register{
	unsigned char byte[RegSize];
	#if RegSize == 1
		uint8_t number;
	#elif RegSize == 2
		uint16_t number;
	#elif RegSize == 3
		uint24_t number;
	#elif RegSize == 4
		uint32_t number;
	#endif
};

class BATCHY{
	private:
		Register batchyStack[StackCount];
		int batchyStackNr = 0;
		Register batchyCommandNr;
		
		unsigned char* eepromInternal;
		
	public:
		BATCHY(int eepromInternalSize);
		~BATCHY();
		Register* getRegister(){return batchyReg;}
		void clear_register();
		void runCommandLink(cmd& command);
		void runCommand(cmd command);
		void runCommandString(char* cmdstr, uint32_t length);
		
		
		unsigned char* getInternalEEPROM(int start, int end);
		void setInternalEEPROM(int start, int end, unsigned char* value);
		
		Register batchyReg[RegCount];
		
		typedef void (*FnPtrBATCHY)(BATCHY&, unsigned char);
		FnPtrBATCHY BATCHYArray[BATCHYArrayMax];
};


#include "batchy_namespacefuncs.h"

//public functions
BATCHY::BATCHY(int eepromInternalSize = 1000){
	//TODO	//clear_register();		//clear_register all
	for(int i = 0; i < RegCount; i++)
		batchyReg[i].number = 0;
	
	//BATCHY MAP INIT
	BATCHYArray[0] = BATCHY_FUNCTIONS::tempi;
	BATCHY_FUNCTIONS_INIT	//Preprocessor define
	
	eepromInternal = new unsigned char[eepromInternalSize];
	for(int i=0; i < sizeof eepromInternal; i++)
		eepromInternal[i] = 0;
}
BATCHY::~BATCHY(){
	delete[] eepromInternal;
}

unsigned char* BATCHY::getInternalEEPROM(int start, int end){
	unsigned char value[end-start+1];
	for(int i=start; i<=end; i++)
		value[i-start] = eepromInternal[i];
	return value;
}
void BATCHY::setInternalEEPROM(int start, int end, unsigned char* value){
	for(int i=start; i<=end; i++)
		eepromInternal[i] = value[i-start];
}

inline void BATCHY::runCommandLink(cmd& command){
	switch(command.splited.id){
		case 0:	//clear all register
			for(int i = 0; i < RegCount; i++)
				batchyReg[i].number = 0;
			break;
		case 1:	//clear 1 register
			if(command.splited.reg < RegCount)
				batchyReg[command.splited.reg].number = 0;
			break;
		case 2:	//set
			if(command.splited.reg < RegCount){
				memcpy(batchyReg[command.splited.reg].byte, command.splited.parameter, RegSize);
			}
			break;
		case 3:	//add
			batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number + batchyReg[command.splited.parameter[1]].number;
			break;
		case 4:	//sub
			batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number - batchyReg[command.splited.parameter[1]].number;
			break;
		case 5:	//div
			batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number * batchyReg[command.splited.parameter[1]].number;
			break;
		case 6:	//mul
			batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number / batchyReg[command.splited.parameter[1]].number;
			break;
		case 7:	//push reg stack
			if((batchyStackNr+1) < StackCount){
				batchyStack[batchyStackNr].number = batchyReg[command.splited.parameter[0]].number;
				batchyStackNr += 1;
			}
			break;
		case 8:	//pop reg stack
			if(batchyStackNr > 0){
				batchyStackNr -= 1;
				batchyReg[command.splited.parameter[0]].number = batchyStack[batchyStackNr].number;
			}
			break;
		case 9:	//call
			(BATCHYArray[((Register*)(command.splited.parameter))->number])(*this, command.splited.reg);
			break;
		case 10: //jump
			memcpy(batchyCommandNr.byte, command.splited.parameter, RegSize);
			break;
		case 11: //jal
			
			break;
		case 12: //jal return
			
			break;
		case 13: //if
			
			break;
	}
}
void BATCHY::runCommand(cmd command){
	runCommandLink(command);
}
void BATCHY::runCommandString(char* cmdstr, uint32_t length){
	for(batchyCommandNr.number = 0; batchyCommandNr.number < length; batchyCommandNr.number+=6){
		runCommandLink(*((cmd*)(cmdstr+batchyCommandNr.number)));
	}
}

#endif
