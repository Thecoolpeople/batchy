#undef _GLIBCXX_DEBUG                // disable run-time bound checking, etc
#pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

#pragma GCC target("bmi,bmi2,lzcnt,popcnt")                      // bit manipulation
#pragma GCC target("aes,pclmul,rdrnd")                           // encryption
#pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD
//#define BATCHY_SD

#include "lib/batchy.h"

const BATCHY batchy(16);
char bc[] = {
/*   0*/  20,   0,   6,   0,   0,   0, // JUMP_NORMAL
/*   6*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/*  12*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  18*/   2,   0,   0,   0,   0,   0, // REG_SET_SINGLE
/*  24*/  16,   0,   0,   0,   0,   0, // STACK_PUSH
/*  30*/  19,   1,   1,   0,   0,   0, // STACK_GET
/*  36*/   2,   2,  16,  39,   0,   0, // REG_SET_SINGLE
/*  42*/  13,   0,   1,   2,   0,   0, // MATH_SMALLER
/*  48*/  31,   0,   0,   0,   0,   0, // CONDITION
/*  54*/  20,   0,  66,   0,   0,   0, // JUMP_NORMAL
/*  60*/  20,   0,  96,   0,   0,   0, // JUMP_NORMAL
/*  66*/  19,   2,   1,   0,   0,   0, // STACK_GET
/*  72*/   2,   3,   1,   0,   0,   0, // REG_SET_SINGLE
/*  78*/   3,   2,   2,   3,   0,   0, // MATH_ADD
/*  84*/  18,   2,   1,   0,   0,   0, // STACK_PUT
/*  90*/  20,   0,  30,   0,   0,   0, // JUMP_NORMAL
/*  96*/  17,   0,   0,   0,   0,   0, // STACK_POP
};

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  unsigned long start = micros();
  batchy.runCommandString(bc, sizeof(bc));
  unsigned long end = micros();
  
  Serial.println("end");
  Serial.println(end-start);
  
  //batchy.runCommandSD(4, "batchy.txt", 10*6);
  /*char bc[] = {
    20,   0, 120,   0,   0,   0,
  2,   0,  10,   0,   0,   0,
 16,   0,   0,   0,   0,   0,
 19,   0,   1,   0,   0,   0,
 16,   0,   0,   0,   0,   0,
 19,   1,   2,   0,   0,   0,
 19,   2,   1,   0,   0,   0,
  5,   1,   1,   2,   0,   0,
 19,   2,   1,   0,   0,   0,
  5,   1,   1,   2,   0,   0,
 19,   2,   2,   0,   0,   0,
  4,   0,   1,   2,   0,   0,
 16,   0,   0,   0,   0,   0,
  2,   1,   4,   0,   0,   0,
  2,   2,   1,   0,   0,   0,
 30,   0,   1,   0,   0,   0,
 17,   0,   0,   0,   0,   0,
 17,   0,   0,   0,   0,   0,
 17,   0,   0,   0,   0,   0,
 22,   0,   0,   0,   0,   0,
  2,   1, 128,  37,   0,   0,
 30,   0,  12,   0,   0,   0,
  2,   0,  10,   0,   0,   0,
 16,   0,   0,   0,   0,   0,
 19,   0,   1,   0,   0,   0,
 16,   0,   0,   0,   0,   0,
 30,   0,  13,   0,   0,   0,
 21,   0,   6,   0,   0,   0,
 30,   0,  13,   0,   0,   0,
  2,   1,   4,   0,   0,   0,
  2,   2,   1,   0,   0,   0,
 30,   0,   2,   0,   0,   0,
  2,   1, 232,   3,   0,   0,
 30,   0,  10,   0,   0,   0,
  2,   1,   4,   0,   0,   0,
  2,   2,   0,   0,   0,   0,
 30,   0,   2,   0,   0,   0,
  2,   1, 232,   3,   0,   0,
 30,   0,  10,   0,   0,   0,
 30,   0,  13,   0,   0,   0, 
  };

  batchy.runCommandString(bc, sizeof(bc));*/
}

void loop() {
   
}
