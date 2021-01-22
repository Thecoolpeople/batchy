# batchy

BATCHY is a very simple bytecode interpreter. This simple and small code can be used for Arduino!

## installation
just download the repository and open the `batchy.ino` file.

## code implemention

include the main batchy header
```
#include "lib/batchy.h"
```
create an instance of batchy with an internal eeprom of 200 bytes.
```
BATCHY batchy(200);
```

To run a command in batchy, you can use `batchy.runCommand((cmd){0,0,0,0,0,0})` for interpreting one command. You have to replace the `0` with the value you want.

If you want to run multiple commands, you should prefer of using:
```
const char cmdstr[] = {
	2,1,255,0,0,0,  	//set 1
	2,2,2,1,0,0,  		//set 2
	3,0,1,2,0,0,  		//add_register
	4,0,1,2,0,0,  		//sub_register
	1,1,0,0,0,0,  		//clear 1
	1,2,0,0,0,0,   		//clear 0

	//function GPIO digital mode
	2,1,4,0,0,0,      	//set reg1 = 4
	2,2,1,0,0,0,      	//set reg2 = 1
	9,0,1,0,0,0,      	//set pin D4 to output

	//setup serial
	2,1,128,37,1,0,   	//serial Speed 74880
	9,0,12,0,0,      	//init serial
};
batchy.runCommandString((char*)cmdstr, sizeof(cmdstr));
```
