/*
	Here are the defines which functions should be included into BATCHY
	comment defines, which you do not need
*/

#define RegCount 10
#define RegSize 4		//4*8 = 32 bit
#define StackCount 8	//8*4*8 = 256 bit



#define BATCHY_DEF_GPIO		//enable GPIO
#define BATCHY_DEF_TIMER	//enable timer, sleepms,..
#define BATCHY_DEF_SERIAL	//enable Serial: USART
#define BATCHY_DEF_I2C		//enable I2C

//batchy define functions
#define pushfoo _Pragma("push_macro(\"BATCHY_FUNCTIONS_INIT\")") //for convenience
#define popfoo  _Pragma("pop_macro(\"BATCHY_FUNCTIONS_INIT\")")
#define BATCHY_FUNCTIONS_INIT

//GPIO
#ifdef BATCHY_DEF_GPIO
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYArray[1] = BATCHY_FUNCTIONS::GPIO_DIGITAL_MODE; \
		BATCHYArray[2] = BATCHY_FUNCTIONS::GPIO_DIGITAL_WRITE; \
		BATCHYArray[3] = BATCHY_FUNCTIONS::GPIO_DIGITAL_READ; \
		BATCHYArray[4] = BATCHY_FUNCTIONS::GPIO_ANALOG_MODE; \
		BATCHYArray[5] = BATCHY_FUNCTIONS::GPIO_ANALOG_WRITE; \
		BATCHYArray[6] = BATCHY_FUNCTIONS::GPIO_ANALOG_READ;
#endif

//timer
#ifdef BATCHY_DEF_TIMER
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYArray[10] = BATCHY_FUNCTIONS::TIMER_SLEEP_MS; \
		BATCHYArray[11] = BATCHY_FUNCTIONS::TIMER_SLEEP_US;
#endif

//Serial
#ifdef BATCHY_DEF_SERIAL
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYArray[12] = BATCHY_FUNCTIONS::SERIAL_INIT; \
		BATCHYArray[13] = BATCHY_FUNCTIONS::SERIAL_GET_ALL_REG; \
		BATCHYArray[14] = BATCHY_FUNCTIONS::SERIAL_SET_ALL_REG;
#endif

//I2C
#ifdef BATCHY_DEF_I2C
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT
#endif

#define BATCHYArrayMax 15		//must be set to (maximal nr of functions + 1)
