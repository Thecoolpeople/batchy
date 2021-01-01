#include "lib/batchy.h"

BATCHY batchy(200);

void setup() {
  batchy.runCommand((cmd){2,1,0,0,0,255});   //set 1
  batchy.runCommand((cmd){2,2,0,0,0,2});     //set 2
  batchy.runCommand((cmd){3,0,1,2,0,0});     //add_register
  batchy.runCommand((cmd){4,0,1,2,0,0});     //sub_register
  batchy.runCommand((cmd){1,1,0,0,0,1});     //clear 1
  batchy.runCommand((cmd){1,2,0,0,0,2});     //clear 2

  //function GPIO digital mode
  batchy.runCommand((cmd){2,1,0,0,0,4});    //set reg1 = 4
  batchy.runCommand((cmd){2,2,0,0,0,1});     //set reg2 = 1
  batchy.runCommand((cmd){9,0,0,0,0,1});
}

void loop() {
  batchy.runCommand((cmd){2,1,0,0,0,4});    //set reg1 = 4
  batchy.runCommand((cmd){2,2,0,0,0,1});     //set reg2 = 1
  batchy.runCommand((cmd){9,0,0,0,0,2});     //function GPIO digital set HIGH

  batchy.runCommand((cmd){2,1,0,0,0,250});    //set reg1 = 250
  batchy.runCommand((cmd){9,0,0,0,0,10});   //dalay

  batchy.runCommand((cmd){2,1,0,0,0,4});    //set reg1 = 4
  batchy.runCommand((cmd){2,2,0,0,0,1});     //set reg2 = 0
  batchy.runCommand((cmd){9,0,0,0,0,2});     //function GPIO digital set LOW

  batchy.runCommand((cmd){2,1,0,0,0,250});    //set reg1 = 250
  batchy.runCommand((cmd){9,0,0,0,0,10});   //dalay
}
