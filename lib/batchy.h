#ifndef BATCHY_H
#define BATCHY_H

#define RegCount 10
#define RegSize 4        //4Byte*8 = 32 bit
#define StackCount 128    //128*4 = 512Byte*8 = 4096 bit

#define BATCHYFuncsMax 15       //must be a minimum of 1 bigger than the maximum value of BATCHY_NR_ (see in batchy_namespacefuncs.h)

//Here are the defines which functions should be included into BATCHY
//comment defines, which you do not need
#define BATCHY_DEF_GPIO        //enable GPIO
#define BATCHY_DEF_TIMER    //enable timer, sleepms,..
#define BATCHY_DEF_SERIAL    //enable Serial: USART
#define BATCHY_DEF_I2C        //enable I2C

//batchy define functions
#define pushfoo _Pragma("push_macro(\"BATCHY_FUNCTIONS_INIT\")") //for convenience
#define popfoo  _Pragma("pop_macro(\"BATCHY_FUNCTIONS_INIT\")")
#define BATCHY_FUNCTIONS_INIT


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

#ifdef BATCHY_SD
  #include <SD.h>
#endif

class BATCHY{
    private:
        Register batchyStack[StackCount];
        int batchyStackNr = 0;
        Register batchyCommandNr;
        
        unsigned char* eepromInternal;
        void getInternalEEPROM(unsigned char *value, int start, int end);
        void setInternalEEPROM(int start, int end, unsigned char* value);
        
    public:
        Register batchyReg[RegCount];
        typedef void (*FnPtrBATCHY)(BATCHY&, unsigned char);
        FnPtrBATCHY BATCHYArray[BATCHYFuncsMax];
        
        BATCHY(int eepromInternalSize);
        ~BATCHY();
        
        Register* getRegister(){return batchyReg;}
        void runCommandLink(cmd& command);
        void runCommand(cmd command);
        void runCommandString(char* cmdstr, uint32_t length);
        #ifdef BATCHY_SD
            void runCommandSD(int cs_pin, String filename, int bufferSize);
        #endif
};

#include "batchy_namespacefuncs.h"

//public functions
BATCHY::BATCHY(int eepromInternalSize = 1000){
    for(int i = 0; i < RegCount; i++)
        batchyReg[i].number = 0;
    
    //BATCHY MAP INIT
    BATCHYArray[0] = BATCHY_FUNCTIONS::tempi;
    BATCHY_FUNCTIONS_INIT    //Preprocessor define
    
    eepromInternal = new unsigned char[eepromInternalSize];
    for(int i=0; i < sizeof eepromInternal; i++)
        eepromInternal[i] = 0;
}
BATCHY::~BATCHY(){
    delete[] eepromInternal;
}

inline void BATCHY::getInternalEEPROM(unsigned char *value, int start, int end){
    for(int i=start; i<=end; i++)
        value[i-start] = eepromInternal[i];
}
inline void BATCHY::setInternalEEPROM(int start, int end, unsigned char* value){
    for(int i=start; i<=end; i++)
        eepromInternal[i] = value[i-start];
}

inline void BATCHY::runCommandLink(cmd& command){
    #define paraNumber ((Register*)(command.splited.parameter))->number
    switch(command.splited.id){
		//////////////////////////////////////
		//			REGISTER
		//////////////////////////////////////
        case 0:    //clear all register
            for(int i = 0; i < RegCount; i++)
                batchyReg[i].number = 0;
            break;
        case 1:    //clear one register (with the number command.splited.reg; starting with 0)
            if(command.splited.reg < RegCount)
                batchyReg[command.splited.reg].number = 0;
            break;
        case 2:    //set
            if(command.splited.reg < RegCount){
                memcpy(batchyReg[command.splited.reg].byte, command.splited.parameter, RegSize);
            }
            break;

		//////////////////////////////////////
		//			Basic Math work
		//////////////////////////////////////
        case 3:    //add
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number + batchyReg[command.splited.parameter[1]].number;
            break;
        case 4:    //sub
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number - batchyReg[command.splited.parameter[1]].number;
            break;
        case 5:    //div
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number * batchyReg[command.splited.parameter[1]].number;
            break;
        case 6:    //mul
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number / batchyReg[command.splited.parameter[1]].number;
            break;
		case 7:    //modulo
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number % batchyReg[command.splited.parameter[1]].number;
            break;
        case 8:    //equal
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number == batchyReg[command.splited.parameter[1]].number;
            break;
        case 9:    //not equal
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number != batchyReg[command.splited.parameter[1]].number;
            break;
        case 10:    //and
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number && batchyReg[command.splited.parameter[1]].number;
            break;
        case 11:    //or
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number || batchyReg[command.splited.parameter[1]].number;
            break;
        case 12:    //greater
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number > batchyReg[command.splited.parameter[1]].number;
            break;
        case 13:    //smaller
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number < batchyReg[command.splited.parameter[1]].number;
            break;
        case 14:    //greater or equal
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number >= batchyReg[command.splited.parameter[1]].number;
            break;
        case 15:    //smaller or equal
            batchyReg[command.splited.reg].number = batchyReg[command.splited.parameter[0]].number <= batchyReg[command.splited.parameter[1]].number;
            break;

		//////////////////////////////////////
		//			STACK
		//////////////////////////////////////
        case 16:    //push reg stack
            if((batchyStackNr+1) < StackCount){
                batchyStack[batchyStackNr].number = batchyReg[command.splited.parameter[0]].number;
                batchyStackNr += 1;
            }else{
                //TODO stack error
            }
            break;
        case 17:    //pop reg stack
            if(batchyStackNr > 0){
                batchyStackNr -= 1;
                batchyReg[command.splited.parameter[0]].number = batchyStack[batchyStackNr].number;
            }else{
                //TODO stack error
            }
            break;
        case 18:    //put variable to n'th height of stack from top   (put a register to n'th height of stack from top)
            if((batchyStackNr-paraNumber) >= 0){
                batchyStack[batchyStackNr-paraNumber].number = batchyReg[command.splited.reg].number;
            }else{
                //TODO stack error
            }
            break;
        case 19:    //get variable from n'th height of stack from top (put n'th height of stack from top to a register)
            if((batchyStackNr-paraNumber) >= 0){
                batchyReg[command.splited.reg].number = batchyStack[batchyStackNr-paraNumber].number;
            }else{
                //TODO stack error
            }
            break;

		//////////////////////////////////////
		//			Jumps
		//////////////////////////////////////
		case 20: //jump
            batchyCommandNr.number = paraNumber - 6;        //minus 6 because of the for loop. it will add +6 before next execution
            break;
        case 21: //jal: this command calls a "function"
            if((batchyStackNr+1) < StackCount){
                batchyStack[batchyStackNr].number = batchyCommandNr.number;
                batchyStackNr += 1;
                
                batchyCommandNr.number = paraNumber - 6;        //minus 6 because of the for loop. it will add +6 before next execution
            }else{
                //TODO stack error
            }
            break;
        case 22: //jal return: this command returns from a "function"
            if(batchyStackNr > 0){
                batchyStackNr -= 1;
                batchyCommandNr.number = batchyStack[batchyStackNr].number;
            }else{
                //TODO stack error
            }
            break;


		//////////////////////////////////////
		//			BATCHY Function
		//////////////////////////////////////
        case 30:    //call function from namespace
            (BATCHYArray[paraNumber])(*this, command.splited.reg);
            break;
        
		//////////////////////////////////////
		//			Conditions
		//////////////////////////////////////
        case 31: //if
            if(batchyReg[command.splited.parameter[0]].number){
                //do nothing. the true condition is the next command
            }else{
                //make a step of +1 Command. the false condition is the upper-next command
                batchyCommandNr.number += 6;
            }
            break;
    }
}
inline void BATCHY::runCommand(cmd command){
    runCommandLink(command);
}
inline void BATCHY::runCommandString(char* cmdstr, uint32_t length){
    for(batchyCommandNr.number = 0; batchyCommandNr.number < length; batchyCommandNr.number+=6){
        runCommandLink(*((cmd*)(cmdstr+batchyCommandNr.number)));
    }
}

#ifdef BATCHY_SD
inline void BATCHY::runCommandSD(int cs_pin, String filename, int bufferSize){
    if (!SD.begin(cs_pin)){
        return;
    }
    File file = SD.open("batchy.txt", FILE_READ);
    if(file){
        char buf[bufferSize];
        Register bufferStart;
        bufferStart.number = 0;
        
        int length = file.size();
        file.seek(bufferStart.number);
        file.read(buf, bufferSize);

        for(batchyCommandNr.number = 0; batchyCommandNr.number < length; batchyCommandNr.number+=6){
            
            if( !((bufferStart.number <= batchyCommandNr.number) && ((bufferStart.number + bufferSize) > batchyCommandNr.number))){
                bufferStart.number = batchyCommandNr.number;
                file.seek(bufferStart.number);
                file.read(buf, bufferSize);
            }

            runCommandLink(*((cmd*)(buf+(batchyCommandNr.number-bufferStart.number))));
        }
    }
    file.close();
}
#endif

#endif
