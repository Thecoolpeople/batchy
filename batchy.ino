#include "lib/batchy.h"

BATCHY batchy(200);

void setup() {
  const char cmdstr[] = {
    2,1,255,0,0,0,  //set 1
    2,2,2,1,0,0,  //set 2
    3,0,1,2,0,0,  //add_register
    4,0,1,2,0,0,  //sub_register
    1,1,0,0,0,0,  //clear 1
    1,2,0,0,0,0,   //clear 0

    //function GPIO digital mode
    2,1,4,0,0,0,      //set reg1 = 4
    2,2,1,0,0,0,      //set reg2 = 1
    9,0,1,0,0,0,      //set pin D4 to output

    //setup serial
    2,1,128,37,1,0,   //serial Speed 74880
    9,0,12,0,0,      //init serial
  };
  batchy.runCommandString((char*)cmdstr, sizeof(cmdstr));
}

const char cmdstrHigh[] = {
  2,1,4,0,0,0,    //set reg1 = 4
  2,2,1,0,0,0,    //set reg2 = 1
  9,0,2,0,0,0,    //function GPIO digital set HIGH
  2,1,250,0,0,0,  //set reg1 = 250
  9,0,10,0,0,0,   //delay
  9,0,13,0,0,0    //serial Write Register
};
const char cmdstrLow[] = {
  2,1,4,0,0,0,    //set reg1 = 4
  2,2,0,0,0,0,    //set reg2 = 0
  9,0,2,0,0,0,    //function GPIO digital set LOW
  2,1,250,0,0,0,  //set reg1 = 250
  9,0,10,0,0,0,   //delay
  9,0,13,0,0,0    //serial Write Register
};

void loop() {
  batchy.runCommandString((char*)cmdstrHigh,sizeof(cmdstrHigh)); //High
  batchy.runCommandString((char*)cmdstrLow, sizeof(cmdstrLow)); //Low
}
