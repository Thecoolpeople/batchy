#include "lib/batchy.h"

BATCHY batchy(200);

void setup() {
  const char cmdstr[] = {
    2,1,0,0,0,255,    //set 1
    2,2,0,0,0,2,      //set 2
    3,0,1,2,0,0,      //add_register
    4,0,1,2,0,0,      //sub_register
    1,1,0,0,0,1,      //clear 1
    1,2,0,0,0,2,      //clear 2

    //function GPIO digital mode
    2,1,0,0,0,4,      //set reg1 = 4
    2,2,0,0,0,1,      //set reg2 = 1
    9,0,0,0,0,1       //set pin D4 to output
  };
  batchy.runCommandString(cmdstr, 9*6);
}

const char cmdstrHigh[] = {
  2,1,0,0,0,4,    //set reg1 = 4
  2,2,0,0,0,1,    //set reg2 = 1
  9,0,0,0,0,2,    //function GPIO digital set HIGH
  2,1,0,0,0,250,  //set reg1 = 250
  9,0,0,0,0,10    //delay
};
const char cmdstrLow[] = {
  2,1,0,0,0,4,    //set reg1 = 4
  2,2,0,0,0,0,    //set reg2 = 0
  9,0,0,0,0,2,    //function GPIO digital set LOW
  2,1,0,0,0,250,  //set reg1 = 250
  9,0,0,0,0,10    //delay
};

void loop() {
  batchy.runCommandString(cmdstrHigh, 5*6); //High
  batchy.runCommandString(cmdstrLow, 5*6); //Low
}
