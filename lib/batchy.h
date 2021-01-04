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
	#endif<
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


namespace BATCHY_FUNCTIONS{
	void tempi(BATCHY &b, unsigned char regReturn){
		b.batchyReg[7].byte[0] = 255;
	}
	
	//GPIO
	#ifdef BATCHY_DEF_GPIO
	void GPIO_DIGITAL_MODE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
			mode: b.batchReg[2].byte[0]: 0x00=INPUT 0x01=OUTPUT 0x02=INPUT_PULLUP
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			if(b.batchyReg[2].byte[0] == 0x00)	//INPUT
				pinMode(b.batchyReg[1].byte[0], INPUT);
			else if(b.batchyReg[2].byte[0] == 0x01)	//OUTPUT
				pinMode(b.batchyReg[1].byte[0], OUTPUT);
			else if(b.batchyReg[2].byte[0] == 0x02)	//INPUT_PULLUP
				pinMode(b.batchyReg[1].byte[0], INPUT_PULLUP);
		//#elif 
		#else
			std::cout << "Set Pin " << std::to_string(b.batchyReg[1].byte[0]) << " to mode ";
			if(b.batchyReg[2].byte[0] == 0x00)	//INPUT
				std::cout << "INPUT" << std::endl;
			else if(b.batchyReg[2].byte[0] == 0x01)	//OUTPUT
				std::cout << "OUTPUT" << std::endl;
			else if(b.batchyReg[2].byte[0] == 0x02)	//INPUT_PULLUP
				std::cout << "INPUT_PULLUP" << std::endl;
		#endif
	}
	void GPIO_DIGITAL_WRITE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
			high/low: b.batchReg[2].byte[0] 0x01
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			digitalWrite(b.batchyReg[1].byte[0], b.batchyReg[2].byte[0]&0x01);
		//#elif
		#else
			std::cout << "Write Pin " << std::to_string(b.batchyReg[1].byte[0]) << " to value " << (b.batchyReg[2].byte[0]&0x01) << std::endl;
		#endif
	}
	void GPIO_DIGITAL_READ(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
			regReturn.byte[0]: value
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			b.batchyReg[1].byte[0] = digitalRead(b.batchyReg[1].byte[0]);
		//#elif
		#else
			std::cout << "Read Pin " << std::to_string(b.batchyReg[1].byte[0]) << std::endl;
		#endif
	}
	void GPIO_ANALOG_MODE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
			mode: b.batchReg[2].byte[0]
		*/
		
	}
	void GPIO_ANALOG_WRITE(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
			value: b.batchReg[2].byte[0]
		*/
		
	}
	void GPIO_ANALOG_READ(BATCHY &b, unsigned char regReturn){
		/*
			pinNR: b.batchyReg[1].byte[0]
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
			delay(b.batchyReg[1].number);
		//#elif
		#else
			std::cout << "TIMER SLEEP ms " << b.batchyReg[1].number << std::endl;
		#endif
	}
	void TIMER_SLEEP_US(BATCHY &b, unsigned char regReturn){
		/*
			us: b.batchyReg[1]
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			delayMicroseconds(b.batchyReg[1].number);
		//#elif
		#else
			std::cout << "TIMER SLEEP us " << b.batchyReg[1].number << std::endl;
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
			Serial.begin(b.batchyReg[1].number);
		#else
			std::cout << "Serial begin with speed " << b.batchyReg[1].number << std::endl;
		#endif
	}
	void SERIAL_WRITE_ALL_REG(BATCHY &b, unsigned char regReturn){
		/*
			
		*/
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			char buf[RegCount*(RegSize*2+1)+1];
		    const char * hex = "0123456789ABCDEF";
		    char pos = 0;
		    for(char i=0; i < RegCount*RegSize; i++){
		        buf[pos] =   hex[(b.batchyReg[i/RegSize].byte[RegSize-1-i%RegSize]>>4) & 0xF];
		        buf[pos+1] = hex[ b.batchyReg[i/RegSize].byte[RegSize-1-i%RegSize]     & 0xF];
		        if(i%RegSize+1 == RegSize){
		        	buf[pos+2] = ';';
		        	pos = pos+3;
				}else{
					pos = pos+2;
				}
		    }
		    buf[RegCount*(RegSize*2+1)] = '\n';
			Serial.write(buf, RegCount*(RegSize*2+1)+1);
		#else
			for(int i=0; i < RegCount; i++)
				std::cout << "Write Register(" << i << "):" << b.batchyReg[i].number << std::endl;
		#endif
	}
	void SERIAL_READ_ALL_REG(BATCHY &b, unsigned char regReturn){
		#if defined(__AVR__) || defined(CubeCell_BoardPlus) /*AB02*/
			//for(int i=0; i < RegCount; i++)
				//Serial.read(b.batchyReg[i].number);
		#else
			for(int i=0; i < RegCount; i++){
				std::cout << "Read Register(" << i << "):";
				std:: cin >> b.batchyReg[i].number;
			}
		#endif
	}
	#endif
	
	//I2C
	#ifdef BATCHY_DEF_I2C
	
	
	#endif
}

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
	
	internalMap[BATCHY_CORE_CLEAR_REGISTER] = &BATCHY::clear_register;
	internalMap[BATCHY_CORE_SET_REGISTER] = &BATCHY::set_register;
	internalMap[BATCHY_CORE_ADD_REGISTER] = &BATCHY::add_register;
	internalMap[BATCHY_CORE_SUB_REGISTER] = &BATCHY::sub_register;
	internalMap[BATCHY_CORE_MUL_REGISTER] = &BATCHY::mul_register;
	internalMap[BATCHY_CORE_DIV_REGISTER] = &BATCHY::div_register;
	internalMap[BATCHY_CORE_PUSH_REG_STACK] = &BATCHY::push_reg_stack;
	internalMap[BATCHY_CORE_POP_REG_STACK] = &BATCHY::pop_reg_stack;
	internalMap[BATCHY_CORE_CALL] = &BATCHY::call;
	internalMap[BATCHY_CORE_JUMP] = &BATCHY::jump;
	internalMap[BATCHY_CORE_JAL] = &BATCHY::jal;
	internalMap[BATCHY_CORE_JAL_RETURN] = &BATCHY::jal_return;
	internalMap[BATCHY_CORE_CORE_IF] = &BATCHY::core_if;
	
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
	internalMapDef::iterator x = internalMap.find(command.splited.id);
	
    if (x != internalMap.end()) {
        (*this.*(x->second))(command.splited.reg, (unsigned char*)command.splited.parameter);
    }
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
