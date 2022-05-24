# batchy

BATCHY is a very simple bytecode interpreter. This simple and small code can be used for Arduino!

## installation
just download the repository and open the `batchy.ino` file.


## documentation
### structure of a command
A single Command is build with:
 - 1 Byte CommandID
 - 1 Byte RegisterID -> This will result in a maximum of 256 Registers, which can be used
 - 4 Byte of Data


All available commands:

	| CommandID | RegisterID | 4 Byte of Data |   GROUP  | Description |
	|-----------|------------|----------------|----------|-------------|
	| 0         | 0          | 0,0,0,0        |Register  | clear all Registers|
	| 1         | N          | 0,0,0,0        |Register  | clear the Register with ID `N`|
	| 2         | N          | 4Byte-DATA     |Register  | set the Register with ID `N` to value DATA|
	| 3         | N          | A,B,0,0        |Math      | (N = A + B) Add Register A to Register B and save it to Register N|
	| 4         | N          | A,B,0,0        |Math      | (N = A - B) Sub Register A to Register B and save it to Register N|
	| 5         | N          | A,B,0,0        |Math      | (N = A * B) Mult Register A to Register B and save it to Register N|
	| 6         | N          | A,B,0,0        |Math      | (N = A / B) Div Register A from Register B and save it to Register N|
	| 7         | N          | A,B,0,0        |Math      | (N = A % B) Modulo Register A from Register B and save it to Register N|
	| 10        | N          | 0,0,0,0        |Stack     | Push Register N to stack|
	| 11        | N          | 0,0,0,0        |Stack     | Pop last Stack to Register N|
	| 12        | N          | 4-Byte:A       |Stack     | Put Register N to the -A last Stacksize element|
	| 13        | N          | 4-Byte:A       |Stack     | Get the -A last Stacksize element to Register N|
	| 20        | 0          | 4-Byte:A       |Jump      | Jump to position N in the BYTECODE|
	| 21        | 0          | 4-Byte:A       |Jump      | JAL: Jump and remember the current position in the BYTECODE (Put that position onto the stack)|
	| 22        | 0          | 0,0,0,0        |Jump      | JAL return Jump back in the BYTECODE (Must be used after JAL, otherwise stack will explode)|
	| 30        | N          | 4-Byte:A       |BATCHY    | Will execute the A's number of BATCHY Function and return the value (if there is one) to Register N|
	| 31        | TODO       | TODO           |Condition | TODO|
	

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

## compiler
The compiler works quite simple. just open the index.html with your favourite browser.
- put you code in the code tap
- click on C ast->generate
- click on Bytecode->generate
then you will get the Bytecode for direct use in batchy

## license
MIT License

## contribution
just fork the repository and create a pull request or create an issue and put your idea in there
