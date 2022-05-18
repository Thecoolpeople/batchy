#ifndef BATCHY_NAMESPACEFUNCS
#define BATCHY_NAMESPACEFUNCS

#if defined(__AVR__) || defined(ESP32) || defined(CubeCell_BoardPlus)/*AB02*/
    #define uC
#endif

namespace BATCHY_FUNCTIONS{
    void tempi(BATCHY &b, unsigned char regReturn){
        b.batchyReg[7].byte[0] = 255;
    }
    

    /*
          _____ _____ _____ ____  
         / ____|  __ \_   _/ __ \ 
        | |  __| |__) || || |  | |
        | | |_ |  ___/ | || |  | |
        | |__| | |    _| || |__| |
         \_____|_|   |_____\____/ 
    */
    #ifdef BATCHY_DEF_GPIO
    #define BATCHY_NR_GPIO_DIGITAL_MODE 1
    void GPIO_DIGITAL_MODE(BATCHY &b, unsigned char regReturn){
        /*
            pinNR: b.batchyReg[1].byte[0]
            mode: b.batchReg[2].byte[0]: 0x00=INPUT 0x01=OUTPUT 0x02=INPUT_PULLUP
        */
        /*JS{
            args:["PIN", "VALUE"],
            name: "gpio.digital.mode",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                a = a.concat([2,2]).concat(intTo4(arg[1].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            switch(b.batchyReg[2].byte[0]){
                case 0x00:      //INPUT
                    pinMode(b.batchyReg[1].byte[0], INPUT);
                    break;
                case 0x01:      //OUTPUT
                    pinMode(b.batchyReg[1].byte[0], OUTPUT);
                    break;
                case 0x02:      //INPUT_PULLUP
                    pinMode(b.batchyReg[1].byte[0], INPUT_PULLUP);
                    break;
            }
        #else
            std::cout << "Set Pin " << std::to_string(b.batchyReg[1].byte[0]) << " to mode ";
            switch(b.batchyReg[2].byte[0]){
                case 0x00:      //INPUT
                    std::cout << "INPUT" << std::endl;
                    break;
                case 0x01:      //OUTPUT
                    std::cout << "OUTPUT" << std::endl;
                    break;
                case 0x02:      //INPUT_PULLUP
                    std::cout << "INPUT_PULLUP" << std::endl;
                    break;
            }
        #endif
    }
    #define BATCHY_NR_GPIO_DIGITAL_WRITE 2
    void GPIO_DIGITAL_WRITE(BATCHY &b, unsigned char regReturn){
        /*
            pinNR: b.batchyReg[1].byte[0]
            high/low: b.batchReg[2].byte[0] 0x01
        */
        /*JS{
            args:["PIN", "VALUE"],
            name: "gpio.digital.write",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                a = a.concat([2,2]).concat(intTo4(arg[1].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            digitalWrite(b.batchyReg[1].byte[0], b.batchyReg[2].byte[0]&0x01);
        #else
            std::cout << "Write Pin " << std::to_string(b.batchyReg[1].byte[0]) << " to value " << (b.batchyReg[2].byte[0]&0x01) << std::endl;
        #endif
    }
    #define BATCHY_NR_GPIO_DIGITAL_READ 3
    void GPIO_DIGITAL_READ(BATCHY &b, unsigned char regReturn){
        /*
            pinNR: b.batchyReg[1].byte[0]
        */
        /*JS{
            args:["PIN"],
            name: "gpio.digital.read",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            b.batchyReg[0].byte[0] = digitalRead(b.batchyReg[1].byte[0]);
        #else
            std::cout << "Read Pin " << std::to_string(b.batchyReg[1].byte[0]) << std::endl;
        #endif
    }
    #define BATCHY_NR_GPIO_ANALOG_MODE 4
    void GPIO_ANALOG_MODE(BATCHY &b, unsigned char regReturn){
        /*
            pinNR: b.batchyReg[1].byte[0]
            mode: b.batchReg[2].byte[0]
        */
        /*JS{
            args:["PIN", "VALUE"],
            name: "gpio.analog.mode",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                a = a.concat([2,2]).concat(intTo4(arg[1].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        GPIO_DIGITAL_MODE(b, regReturn);
    }
    #define BATCHY_NR_GPIO_ANALOG_WRITE 5
    void GPIO_ANALOG_WRITE(BATCHY &b, unsigned char regReturn){  //TODO
        /*
            pinNR: b.batchyReg[1].byte[0]
            value: b.batchReg[2].byte[0]
        */
        /*JS{
            args:["PIN", "VALUE"],
            name: "gpio.analog.write",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                a = a.concat([2,2]).concat(intTo4(arg[1].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            analogWrite(b.batchyReg[1].byte[0], b.batchyReg[2].number);
        #else
            std::cout << "Write Pin " << std::to_string(b.batchyReg[1].byte[0]) << " to value " << (b.batchyReg[2].byte[0]&0x01) << std::endl;
        #endif
    }
    #define BATCHY_NR_GPIO_ANALOG_READ 6
    void GPIO_ANALOG_READ(BATCHY &b, unsigned char regReturn){  //TODO
        /*
            pinNR: b.batchyReg[1].byte[0]
            regReturn.byte[0]: value
        */
        /*JS{
            args:["PIN"],
            name: "gpio.analog.read",
            func: function(arg){
                return []
            }
        }*/
    }
    #endif
    
    
    /*
          _______ _____ __  __ ______ _____  
         |__   __|_   _|  \/  |  ____|  __ \ 
            | |    | | | \  / | |__  | |__) |
            | |    | | | |\/| |  __| |  _  / 
            | |   _| |_| |  | | |____| | \ \ 
            |_|  |_____|_|  |_|______|_|  \_\
    */
    #ifdef BATCHY_DEF_TIMER
    #define BATCHY_NR_TIMER_SLEEP_MS 10
    void TIMER_SLEEP_MS(BATCHY &b, unsigned char regReturn){
        /*
            ms: b.batchyReg[1]
        */
        /*JS{
            args:["Duration ms"],
            name: "delay.ms",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            delay(b.batchyReg[1].number);
        #else
            std::cout << "TIMER SLEEP ms " << b.batchyReg[1].number << std::endl;
        #endif
    }
    #define BATCHY_NR_TIMER_SLEEP_US 11
    void TIMER_SLEEP_US(BATCHY &b, unsigned char regReturn){
        /*
            us: b.batchyReg[1]
        */
        /*JS{
            args:["Duration us"],
            name: "delay.us",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            delayMicroseconds(b.batchyReg[1].number);
        #else
            std::cout << "TIMER SLEEP us " << b.batchyReg[1].number << std::endl;
        #endif
    }
    #endif
    

    /*
          _____ ______ _____  _____          _      
         / ____|  ____|  __ \|_   _|   /\   | |     
        | (___ | |__  | |__) | | |    /  \  | |     
         \___ \|  __| |  _  /  | |   / /\ \ | |     
         ____) | |____| | \ \ _| |_ / ____ \| |____ 
        |_____/|______|_|  \_\_____/_/    \_\______|
    */
    #ifdef BATCHY_DEF_SERIAL
    #define BATCHY_NR_SERIAL_INIT 12
    void SERIAL_INIT(BATCHY &b, unsigned char regReturn){
        /*
            serialSpeed: b.batchyReg[1]
        */
        /*JS{
            args:["speed"],
            name: "serial.init",
            func: function(arg){
                let a = [2,1].concat(intTo4(arg[0].value))
                let c = [9,0,BATCHY,0,0,0]
                return [].concat(a,c)
            }
        }*/
        #if defined(uC)
            Serial.begin(b.batchyReg[1].number);
        #else
            std::cout << "Serial begin with speed " << b.batchyReg[1].number << std::endl;
        #endif
    }
    #define BATCHY_NR_SERIAL_GET_ALL_REG 13
    void SERIAL_GET_ALL_REG(BATCHY &b, unsigned char regReturn){
        /*
            no register needed
        */
        /*JS{
            args:[],
            name: "serial.readAllRegister",
            func: function(arg){
                let c = [9,0,BATCHY,0,0,0]
		        return c
            }
        }*/
        #if defined(uC)
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
            #if defined(CubeCell_BoardPlus) || defined(ESP32)
                Serial.write((const unsigned char*)buf, RegCount*(RegSize*2+1)+1);
            #else
                
                Serial.write(buf, RegCount*(RegSize*2+1)+1);
            #endif
        #else
            std::cout << "Register Batchy: ";
            for(int i=0; i < RegCount; i++)
                std::cout << b.batchyReg[i].number << " ";
            std::cout << std::endl;
        #endif
    }
    #define BATCHY_NR_SERIAL_SET_ALL_REG 14
    void SERIAL_SET_ALL_REG(BATCHY &b, unsigned char regReturn){    //TODO
        /*JS{
            args:[],
            name: "serial.writeAllRegister",
            func: function(arg){
                let c = [9,0,BATCHY,0,0,0]
		        return c
            }
        }*/
        #if defined(uC)
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
    

    /*
         _____ ___   _____ 
        |_   _|__ \ / ____|
          | |    ) | |     
          | |   / /| |     
         _| |_ / /_| |____ 
        |_____|____|\_____|
    */
    #ifdef BATCHY_DEF_I2C
    
    
    #endif
}

#endif
