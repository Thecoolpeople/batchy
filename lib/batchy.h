#ifndef BATCHY_H
#define BATCHY_H

#include "batchy_defines.h"

#if defined(__AVR__)
#include <ArduinoSTL.h>
#endif

#include <map>
#include <cstring>		//needed for memset

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
struct parameterFunction{
	unsigned char p1;
	unsigned char p2;
	unsigned char p3;
	unsigned char p4;
	
	bool operator < (const parameterFunction &c2) const{
		return p1 < c2.p2 || 
		(p1 == c2.p1 && p2 < c2.p2) ||
		(p2 == c2.p2 && p3 < c2.p3) ||
		(p3 == c2.p3 && p4 < c2.p4);
	}
};

class BATCHY{
	private:
		typedef void (BATCHY::*FnPtr)(unsigned char, unsigned char*);
		FnPtr internalArray[BATCHY_CORE_MAX];
		
		Register batchyStack[StackCount];
		int batchyStackNr = 0;
		Register batchyCommandNr;
		
		unsigned char* eepromInternal;
		
		void clear_register(unsigned char reg, unsigned char* nr2);
		void set_register(unsigned char reg, unsigned char* parameter);
		void add_register(unsigned char reg, unsigned char* parameter);
		void sub_register(unsigned char reg, unsigned char* parameter);
		void mul_register(unsigned char reg, unsigned char* parameter);
		void div_register(unsigned char reg, unsigned char* parameter);
		void push_reg_stack(unsigned char reg, unsigned char* parameter);
		void pop_reg_stack(unsigned char reg, unsigned char* parameter);
		void call(unsigned char reg, unsigned char* parameter);
		void jump(unsigned char reg, unsigned char* parameter);
		void jal(unsigned char reg, unsigned char* parameter);
		void jal_return(unsigned char reg, unsigned char* parameter);
		void core_if(unsigned char reg, unsigned char* parameter);
		
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
		typedef std::map<parameterFunction, FnPtrBATCHY> BATCHYMapDef;
		BATCHYMapDef BATCHYMap;
};


#include "batchy_namespacefuncs.h"

//core functions
void BATCHY::clear_register(){
	for(int i = 0; i < RegCount; i++)
		batchyReg[i].number = 0;
}
void BATCHY::clear_register(unsigned char reg, unsigned char* a){
	if(reg < RegCount)
		batchyReg[reg].number = 0;
}
void BATCHY::set_register(unsigned char reg, unsigned char* parameter){
	if(reg < RegCount){
		memcpy(batchyReg[reg].byte, parameter, RegSize);
	}
}
void BATCHY::add_register(unsigned char reg, unsigned char* parameter){	//reg = parameter[0] + parameter[1]
	batchyReg[reg].number = batchyReg[parameter[0]].number + batchyReg[parameter[1]].number;
}
void BATCHY::sub_register(unsigned char reg, unsigned char* parameter){	//reg = parameter[0] - parameter[1]
	batchyReg[reg].number = batchyReg[parameter[0]].number - batchyReg[parameter[1]].number;
}
void BATCHY::mul_register(unsigned char reg, unsigned char* parameter){	//reg = parameter[0] - parameter[1]
	batchyReg[reg].number = batchyReg[parameter[0]].number * batchyReg[parameter[1]].number;
}
void BATCHY::div_register(unsigned char reg, unsigned char* parameter){
	batchyReg[reg].number = batchyReg[parameter[0]].number / batchyReg[parameter[1]].number;
}
void BATCHY::push_reg_stack(unsigned char reg, unsigned char* parameter){
	if((batchyStackNr+1) < StackCount){
		batchyStack[batchyStackNr].number = batchyReg[parameter[0]].number;
		batchyStackNr += 1;
	}
}
void BATCHY::pop_reg_stack(unsigned char reg, unsigned char* parameter){
	if(batchyStackNr > 0){
		batchyStackNr -= 1;
		batchyReg[parameter[0]].number = batchyStack[batchyStackNr].number;
	}
}
void BATCHY::call(unsigned char reg, unsigned char* parameter){
	BATCHYMapDef::iterator x = BATCHYMap.find({parameter[0], parameter[1], parameter[2], parameter[3]});
	
    if (x != BATCHYMap.end()) {
        (*(x->second))(*this, reg);
    }
}
void BATCHY::jump(unsigned char reg, unsigned char* parameter){
	memcpy(batchyCommandNr.byte, parameter, RegSize);
}
void BATCHY::jal(unsigned char reg, unsigned char* parameter){	//jump and link
	//push_stack()
	
	//memcpy(batchyCommandNr.byte, parameter, RegSize);
}
void BATCHY::jal_return(unsigned char reg, unsigned char* parameter){	//jump and link back
	//memcpy: batchyCommandNr.byte <- pop_stack()
}
void BATCHY::core_if(unsigned char reg, unsigned char* parameter){
	
}

//public functions
BATCHY::BATCHY(int eepromInternalSize = 1000){
	clear_register();		//clear_register all
	
	//new
	internalArray[BATCHY_CORE_CLEAR_REGISTER] = &BATCHY::clear_register;
	internalArray[BATCHY_CORE_SET_REGISTER] = &BATCHY::set_register;
	internalArray[BATCHY_CORE_ADD_REGISTER] = &BATCHY::add_register;
	internalArray[BATCHY_CORE_SUB_REGISTER] = &BATCHY::sub_register;
	internalArray[BATCHY_CORE_MUL_REGISTER] = &BATCHY::mul_register;
	internalArray[BATCHY_CORE_DIV_REGISTER] = &BATCHY::div_register;
	internalArray[BATCHY_CORE_PUSH_REG_STACK] = &BATCHY::push_reg_stack;
	internalArray[BATCHY_CORE_POP_REG_STACK] = &BATCHY::pop_reg_stack;
	internalArray[BATCHY_CORE_CALL] = &BATCHY::call;
	internalArray[BATCHY_CORE_JUMP] = &BATCHY::jump;
	internalArray[BATCHY_CORE_JAL] = &BATCHY::jal;
	internalArray[BATCHY_CORE_JAL_RETURN] = &BATCHY::jal_return;
	internalArray[BATCHY_CORE_CORE_IF] = &BATCHY::core_if;
	
	//BATCHY MAP INIT
	BATCHYMap[{0,0,0,0}] = BATCHY_FUNCTIONS::tempi;
	
	BATCHY_FUNCTIONS_INIT
	
	eepromInternal = new unsigned char[eepromInternalSize];
	std::memset(eepromInternal, 0, sizeof eepromInternal);
}
BATCHY::~BATCHY(){
	delete[] eepromInternal;
}

unsigned char* BATCHY::getInternalEEPROM(int start, int end){
	unsigned char value[end-start+1];
	//memcpy(value, (*eepromInternal)+start, end-start+1);	--> TODO Test
	for(int i=start; i<=end; i++)
		value[i-start] = eepromInternal[i];
	return value;
}
void BATCHY::setInternalEEPROM(int start, int end, unsigned char* value){
	//memcpy((*eepromInternal)+start, value, end-start+1);	--> TODO Test
	for(int i=start; i<=end; i++)
		eepromInternal[i] = value[i-start];
}

void BATCHY::runCommandLink(cmd& command){
	(*this.*(internalArray[command.splited.id]))(command.splited.reg, (unsigned char*)command.splited.parameter);
}
void BATCHY::runCommand(cmd command){
	runCommandLink(command);
}
void BATCHY::runCommandString(char* cmdstr, uint32_t length){
	
	cmd a;
	for(batchyCommandNr.number = 0; batchyCommandNr.number < length; batchyCommandNr.number+=6){
		memcpy(a.full, cmdstr+batchyCommandNr.number, 6);
		runCommandLink(a);
	}
}


#endif
