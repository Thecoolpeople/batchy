#ifndef BATCHY_H
#define BATCHY_H

#include "batchy_defines.h"

#if defined(__AVR__)
#include <ArduinoSTL.h>
#endif

#include <map>
#include <cstring>		//needed for memset

#define RegCount 10
#define RegSize 4		//4*8 = 32 bit
#define StackCount 8	//8*4*8 = 256 bit

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
	//bool bits[RegSize*8];			//bools not working
	unsigned char byte[RegSize];
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
		typedef std::map<unsigned char, FnPtr> internalMapDef;
		internalMapDef internalMap;
		
		
		Register batchyStack[StackCount];
		int batchyStackNr = 0;
		
		unsigned char* eepromInternal;
		
		void clear_register(unsigned char reg, unsigned char* nr2);
		void set_register(unsigned char reg, unsigned char* parameter);
		void add_register(unsigned char reg, unsigned char* parameter);
		void sub_register(unsigned char reg, unsigned char* parameter);
		void mul_register(unsigned char reg, unsigned char* parameter);
		void div_register(unsigned char reg, unsigned char* parameter);
		void push_stack(unsigned char reg, unsigned char* parameter);
		void pop_stack(unsigned char reg, unsigned char* parameter);
		void call(unsigned char reg, unsigned char* parameter);
		
	public:
		BATCHY(int eepromInternalSize);
		~BATCHY();
		Register* getRegister(){return batchyReg;}
		void clear_register();
		void runCommandLink(cmd& command);
		void runCommand(cmd command);
		void runCommandString(char* cmdstr, int length);
		
		
		unsigned char* getInternalEEPROM(int start, int end);
		void setInternalEEPROM(int start, int end, unsigned char* value);
		
		Register batchyReg[RegCount];
		
		typedef void (*FnPtrBATCHY)(BATCHY&, unsigned char);
		typedef std::map<parameterFunction, FnPtrBATCHY> BATCHYMapDef;
		BATCHYMapDef BATCHYMap;
};


namespace BATCHY_FUNCTIONS{
	void tempi(BATCHY &b, unsigned char regReturn){
		b.batchyReg[7].byte[3] = 255;
	}
	
	//GPIO
	#ifdef BATCHY_DEF_GPIO
	void GPIO_DIGITAL_MODE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			mode: b.batchReg[2].byte[3]: 0x00=INPUT 0x01=OUTPUT 0x02=INPUT_PULLUP
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			if(b.batchyReg[2].byte[3] == 0x00)	//INPUT
				pinMode(b.batchyReg[1].byte[3], INPUT);
			else if(b.batchyReg[2].byte[3] == 0x01)	//OUTPUT
				pinMode(b.batchyReg[1].byte[3], OUTPUT);
			else if(b.batchyReg[2].byte[3] == 0x02)	//INPUT_PULLUP
				pinMode(b.batchyReg[1].byte[3], INPUT_PULLUP);
		//#elif 
		#else
			std::cout << "Set Pin " << std::to_string(b.batchyReg[1].byte[3]) << " to mode ";
			if(b.batchyReg[2].byte[3] == 0x00)	//INPUT
				std::cout << "INPUT" << std::endl;
			else if(b.batchyReg[2].byte[3] == 0x01)	//OUTPUT
				std::cout << "OUTPUT" << std::endl;
			else if(b.batchyReg[2].byte[3] == 0x02)	//INPUT_PULLUP
				std::cout << "INPUT_PULLUP" << std::endl;
		#endif
	}
	void GPIO_DIGITAL_WRITE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			high/low: b.batchReg[2].byte[3] 0x01
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			digitalWrite(b.batchyReg[1].byte[3], b.batchyReg[2].byte[3]&0x01);
		//#elif
		#else
			std::cout << "Write Pin " << std::to_string(b.batchyReg[1].byte[3]) << " to value " << (b.batchyReg[2].byte[3]&0x01) << std::endl;
		#endif
	}
	void GPIO_DIGITAL_READ(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			regReturn.byte[0]: value
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			b.batchyReg[1].byte[0] = digitalRead(b.batchyReg[1].byte[3]);
		//#elif
		#else
			std::cout << "Read Pin " << std::to_string(b.batchyReg[1].byte[3]) << std::endl;
		#endif
	}
	void GPIO_ANALOG_MODE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			mode: b.batchReg[2].byte[3]
		*/
		
	}
	void GPIO_ANALOG_WRITE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			value: b.batchReg[2].byte[3]
		*/
		
	}
	void GPIO_ANALOG_READ(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[3]
			regReturn.byte[0]: value
		*/
		
	}
	#endif
	
	//timer
	#ifdef BATCHY_DEF_TIMER
	void TIMER_SLEEP_MS(BATCHY &b, unsigned char regReturn){
		/*
			ms: b.batchyReg[1]
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			delay(b.batchyReg[1].byte[0]*256*256*256);
			delay(b.batchyReg[1].byte[1]*256*256);
			delay(b.batchyReg[1].byte[2]*256);
			delay(b.batchyReg[1].byte[3]);
		//#elif
		#else
			std::cout << "TIMER SLEEP ms " << std::to_string(b.batchyReg[1].byte[3]) << std::endl;
		#endif
	}
	void TIMER_SLEEP_US(BATCHY &b, unsigned char regReturn){
		/*
			us: b.batchyReg[1]
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			delayMicroseconds(b.batchyReg[1].byte[0]*256*256*256);
			delayMicroseconds(b.batchyReg[1].byte[1]*256*256);
			delayMicroseconds(b.batchyReg[1].byte[2]*256);
			delayMicroseconds(b.batchyReg[1].byte[3]);
		//#elif
		#else
			std::cout << "TIMER SLEEP us " << std::to_string(b.batchyReg[1].byte[3]) << std::endl;
		#endif
	}
	#endif
	
	//Serial
	#ifdef BATCHY_DEF_SERIAL
	void SERIAL_INIT(BATCHY &b, unsigned char regReturn){
		/*
			serialSpeed: b.batchyReg[1]
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			uint32_t speed = b.batchyReg[1].byte[0] << 24 | b.batchyReg[1].byte[1] << 16 | b.batchyReg[1].byte[2] << 8 | b.batchyReg[1].byte[3];
			Serial.begin(speed);
		#else
			uint32_t speed = b.batchyReg[1].byte[0] << 24 | b.batchyReg[1].byte[1] << 16 | b.batchyReg[1].byte[2] << 8 | b.batchyReg[1].byte[3];
			std::cout << "Serial begin with speed " << speed << std::endl;
		#endif
	}
	void SERIAL_WRITE_REG(BATCHY &b, unsigned char regReturn){
		
	}
	void SERIAL_READ_REG(BATCHY &b, unsigned char regReturn){
		
	}
	#endif
	
	//I2C
	#ifdef BATCHY_DEF_I2C
	
	
	#endif
}

//core functions
void BATCHY::clear_register(){
	for(int i = 0; i < RegCount; i++)
		for(int z = 0; z < RegSize; z++)
			batchyReg[i].byte[z] = 0;
}
void BATCHY::clear_register(unsigned char reg, unsigned char* a){
	if(reg < RegCount)
		for(int z = 0; z < RegSize; z++)
			batchyReg[reg].byte[z] = 0;
}
void BATCHY::set_register(unsigned char reg, unsigned char* parameter){
	if(reg < RegCount){
		for(int z = 0; z < RegSize; z++)
			batchyReg[reg].byte[z] = parameter[z];
	}
}
void BATCHY::add_register(unsigned char reg, unsigned char* parameter){	//reg = parameter[0] + parameter[1]
	char buff = 0;
	for(int z = RegSize-1; z >= 0; z--){
		batchyReg[reg].byte[z] = batchyReg[parameter[0]].byte[z] + batchyReg[parameter[1]].byte[z] + buff;
		buff = batchyReg[reg].byte[z] < (batchyReg[parameter[0]].byte[z] + batchyReg[parameter[1]].byte[z] + buff);
	}
}
void BATCHY::sub_register(unsigned char reg, unsigned char* parameter){	//reg = parameter[0] - parameter[1]
	char buff = 0;
	for(int z = RegSize-1; z >= 0; z--){
		batchyReg[reg].byte[z] = batchyReg[parameter[0]].byte[z] - batchyReg[parameter[1]].byte[z] - buff;
		buff = batchyReg[reg].byte[z] != (batchyReg[parameter[0]].byte[z] - batchyReg[parameter[1]].byte[z] - buff);
	}
}
void BATCHY::mul_register(unsigned char reg, unsigned char* parameter){
	
}
void BATCHY::div_register(unsigned char reg, unsigned char* parameter){
	
}
void BATCHY::push_stack(unsigned char reg, unsigned char* parameter){
	if((batchyStackNr+1) < StackCount){
		for(int z = 0; z < RegSize; z++)
			batchyStack[batchyStackNr].byte[z] = parameter[z];
		batchyStackNr += 1;
	}
}
void BATCHY::pop_stack(unsigned char reg, unsigned char* parameter){
	if(batchyStackNr > 0){
		batchyStackNr -= 1;
	}
}
void BATCHY::call(unsigned char reg, unsigned char* parameter){
	BATCHYMapDef::iterator x = BATCHYMap.find({parameter[0], parameter[1], parameter[2], parameter[3]});
	
    if (x != BATCHYMap.end()) {
        (*(x->second))(*this, reg);
    }
}

//public functions
BATCHY::BATCHY(int eepromInternalSize = 1000){
	clear_register();		//clear_register all
	
	internalMap[BATCHY_CORE_CLEAR_REGISTER] = &BATCHY::clear_register;
	internalMap[BATCHY_CORE_SET_REGISTER] = &BATCHY::set_register;
	internalMap[BATCHY_CORE_ADD_REGISTER] = &BATCHY::add_register;
	internalMap[BATCHY_CORE_SUB_REGISTER] = &BATCHY::sub_register;
	internalMap[BATCHY_CORE_MUL_REGISTER] = &BATCHY::mul_register;
	internalMap[BATCHY_CORE_DIV_REGISTER] = &BATCHY::div_register;
	internalMap[BATCHY_CORE_PUSH_STACK] = &BATCHY::push_stack;
	internalMap[BATCHY_CORE_POP_STACK] = &BATCHY::pop_stack;
	internalMap[BATCHY_CORE_CALL] = &BATCHY::call;
	
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
	for(int i=start; i<=end; i++)
		value[i-start] = eepromInternal[i];
	return value;
}
void BATCHY::setInternalEEPROM(int start, int end, unsigned char* value){
	for(int i=start; i<=end; i++)
		eepromInternal[i] = value[i-start];
}

void BATCHY::runCommandLink(cmd& command){
	internalMapDef::iterator x = internalMap.find(command.splited.id);
	
    if (x != internalMap.end()) {
        (*this.*(x->second))(command.splited.reg, (unsigned char*)command.splited.parameter);
    }
}
void BATCHY::runCommand(cmd command){
	runCommandLink(command);
}
void BATCHY::runCommandString(char* cmdstr, int length){
	cmd a;
	for(int i=0; i < length; i+=6){
		memcpy(a.full, cmdstr+i, 6);
		runCommandLink(a);
	}
}


#endif
