#ifndef BATCHY_NAMESPACEFUNCS
#define BATCHY_NAMESPACEFUNCS

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

#endif
