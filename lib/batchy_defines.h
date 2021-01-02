/*
	Here are the defines which functions should be included into BATCHY
	comment defines, which you do not need
*/

#define BATCHY_DEF_GPIO		//enable GPIO
#define BATCHY_DEF_TIMER	//enable timer, sleepms,..
#define BATCHY_DEF_SERIAL	//enable Serial: USART
#define BATCHY_DEF_I2C		//enable I2C


//internal core functions
#define BATCHY_CORE_CLEAR_REGISTER 1
#define BATCHY_CORE_SET_REGISTER   2
#define BATCHY_CORE_ADD_REGISTER   3
#define BATCHY_CORE_SUB_REGISTER   4
#define BATCHY_CORE_MUL_REGISTER   5
#define BATCHY_CORE_DIV_REGISTER   6
#define BATCHY_CORE_PUSH_STACK     7
#define BATCHY_CORE_POP_STACK      8
#define BATCHY_CORE_CALL           9

//batchy define functions
#define pushfoo _Pragma("push_macro(\"BATCHY_FUNCTIONS_INIT\")") //for convenience
#define popfoo  _Pragma("pop_macro(\"BATCHY_FUNCTIONS_INIT\")")
#define BATCHY_FUNCTIONS_INIT

//GPIO
#ifdef BATCHY_DEF_GPIO
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYMap[{0,0,0,1}] = BATCHY_FUNCTIONS::GPIO_DIGITAL_MODE; \
		BATCHYMap[{0,0,0,2}] = BATCHY_FUNCTIONS::GPIO_DIGITAL_WRITE; \
		BATCHYMap[{0,0,0,3}] = BATCHY_FUNCTIONS::GPIO_DIGITAL_READ; \
		BATCHYMap[{0,0,0,4}] = BATCHY_FUNCTIONS::GPIO_ANALOG_MODE; \
		BATCHYMap[{0,0,0,5}] = BATCHY_FUNCTIONS::GPIO_ANALOG_WRITE; \
		BATCHYMap[{0,0,0,6}] = BATCHY_FUNCTIONS::GPIO_ANALOG_READ;
#endif

//timer
#ifdef BATCHY_DEF_TIMER
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYMap[{0,0,0,10}] = BATCHY_FUNCTIONS::TIMER_SLEEP_MS; \
		BATCHYMap[{0,0,0,11}] = BATCHY_FUNCTIONS::TIMER_SLEEP_US;
#endif

//Serial
#ifdef BATCHY_DEF_SERIAL
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT \
		BATCHYMap[{0,0,0,12}] = BATCHY_FUNCTIONS::TIMER_SLEEP_US;
#endif

//I2C
#ifdef BATCHY_DEF_I2C
	pushfoo
	#undef BATCHY_FUNCTIONS_INIT
	#define BATCHY_FUNCTIONS_INIT popfoo BATCHY_FUNCTIONS_INIT
#endif
