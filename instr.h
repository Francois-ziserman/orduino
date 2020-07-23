#ifndef INSTR_H
#define INSTR_H

#include <avr/pgmspace.h>
#include "log.h"

#define I_NOP                   0x00
#define I_STOP                  0x01

// MOVE

#define I_MOV_A                 0x02
#define I_MOV_B                 0x03
#define I_MOV_C                 0x04
#define I_MOV_VAL_A             0x05
#define I_MOV_VAL_B             0x06
#define I_MOV_VAL_C             0x07
#define I_MOVR_A                0x08
#define I_MOVR_B                0x09
#define I_MOVR_C                0x0A
#define I_MOVRR_AB              0x0B

// OPERATION

#define I_ADD                   0x10
#define I_MINUS                 0x11
#define I_MULT                  0x12
#define I_DIV                   0x13
#define I_INC_A                 0x14
#define I_INC_B                 0x15
#define I_INC_C                 0x16
#define I_DEC_A                 0x17
#define I_DEC_B                 0x18
#define I_DEC_C                 0x19
#define I_INV_A                 0x1A
#define I_INV_B                 0x1B
#define I_INV_C                 0x1C

// LABELS & JUMP

#define I_LABEL                 0x20
#define I_JUMP                  0x21
#define I_JAZ                   0x22
#define I_JBZ                   0x23
#define I_JCZ                   0x24
#define I_JANZ                  0x25
#define I_JBNZ                  0x26
#define I_JCNZ                  0x27
#define I_JEQ_AB                0x28
#define I_JNEQ_AB               0x29
#define I_JEQ_AC                0x2A
#define I_JNEQ_AC               0x2B
#define I_JEQ_BC                0x2C
#define I_JNEQ_BC               0x2D
#define I_JUMP_RB               0x2E
#define I_JUMP_ADR_RB           0x2F
#define I_JUMP_SUB              0x30
#define I_RETURN                0x31

// SCREEN

#define I_SCREEN_ON             0x40
#define I_SCREEN_OFF            0x41
#define I_SCREEN_CLEAR          0x42
#define I_SCREEN_NL             0x43
#define I_INPUTA                0x44
#define I_INPUTB                0x45
#define I_INPUTC                0x46
#define I_INPUTM                0x47
#define I_PRINTA                0x48
#define I_PRINTB                0x49
#define I_PRINTC                0x4A
#define I_PRINTM                0x4B
#define I_PRINT_CHAR            0x4C
#define I_PRINT_CHAR_A          0x4D
#define I_PRINT_CHAR_M          0x4E

// SWITCH & OTHER OP

#define I_SWITCH_C_A            0x50
#define I_SWITCH_C_B            0x51
#define I_SWITCH_A_B            0x52

#define I_LSHIFT                0x53
#define I_RSHIFT                0x54
#define I_AND                   0x55
#define I_OR                    0x56

#define I_RANDOM                0x57
#define I_RANDOM_A              0x58

// STACK

#define I_STACK_INIT            0x60
#define I_STACK_PUSH_A          0x61
#define I_STACK_PUSH_B          0x62
#define I_STACK_PUSH_C          0x63
#define I_STACK_PUSH_M          0x64
#define I_STACK_POP_A           0x65
#define I_STACK_POP_B           0x66
#define I_STACK_POP_C           0x67
#define I_STACK_POP_M           0x68

// ARDUINO 

#define I_SLEEP                 0x70
#define I_SLEEP_A               0x71

#define I_ARD_PIN_MODE_OUTPUT   0x72 // set the pin rA on OUTPUT
#define I_ARD_PIN_MODE_INPUT    0x73 // set the pin rA on INPUT
#define I_ARD_DREAD             0x74 // read the pin rA & put the value in rC
#define I_ARD_DWRITE            0x75 // write on the pin rA with the value in rB
#define I_ARD_AREAD             0x76 // read the pin rA & put the value in rC
#define I_ARD_AWRITE            0x77 // write the pin rA with the value rB

#define I_LAST                  0x80

#define T true
#define F false

// Permet d'afficher le nom de la commande 
const char INSTR[][8] = {
/*           0          1          2          3          4          5          6         7     */  
/* 00 */  "no op  ", "STOP   ", "MOV A  ", "MOV B  ", "MOV C  ", "MOV VA ", "MOV VB ", "MOV VC ",
/* 08 */  "MOV RA ", "MOV RB ", "MOV RC ", "MOVRRAB", "--     ", "--     ", "--     ", "--     ",
 
/* 10 */  "ADD    ", "MINUS  ", "MULT   ", "DIV    ", "A++    ", "B++    ", "C++    ", "A--    ",
/* 18 */  "B--    ", "C--    ", "INV A  ", "INV B  ", "INV C  ", "--     ", "--     ", "--     ",

/* 20 */  "LABEL  ", "JUMP   ", "JMP A Z", "JMP B Z", "JMP C Z", "JMP A!Z", "JMP B!Z", "JMP C!Z", 
/* 28 */  "JMP A=B", "JMP A!B", "JMP A=C", "JMP A!C", "JMP B=C", "JMP_B!C", "JMP  RA", "JMP ADA",

/* 30 */  "JMP SUB", "RETURN ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
/* 38 */  "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",

/* 40 */  "SCR ON ", "SCR OFF", "SCR CLR", "SCR NL ", "INPUT A", "INPUT B", "INPUT C", "INPUT M",
/* 48 */  "PRT A  ", "PRT B  ", "PRT C  ", "PRT M  ", "PRTCH  ", "PRTCH A", "PRTCH M", "--     ",
  
/* 50 */  "SW C A ", "SW C B ", "SW A B ", "LSHIFT ", "RSHIFT ", "AND    ", "OR     ", "RND    ",
/* 58 */  "RND A  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",

/* 60 */  "ST INIT", "PUSH A ", "PUSH B ", "PUSH C ", "PUSH M ", "POP A  ", "POP B  ", "POP C  ", 
/* 68 */  "POP M  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",

/* 70 */  "SLEEP  ", "SLEEP A", "PINMOUT", "PMODINP", "DREAD  ", "DWRITE ", "AREAD  ", "AWRITE ",
 
};


const bool WITH_PARAM[] = {
/*        0  1  2  3  4  5  6  7  */
/* 00 */  F, F, T, T, T, T, T, T,
/* 08 */  T, T, T, F, F, F, F, F,

/* 10 */  F, F, F, F, F, F, F, F,
/* 18 */  F, F, F, F, F, F, F, F,

/* 20 */  T, T, T, T, T, T, T, T,
/* 28 */  T, T, T, T, T, T, F, F,

/* 30 */  T, F, F, F, F, F, F, T,
/* 38 */  T, F, F, F, F, F, F, T,

/* 40 */  F, F, F, F, F, F, F, T,
/* 48 */  F, F, F, T, T, F, T, F,

/* 50 */  F, F, F, T, T, F, F, T,
/* 58 */  F, F, F, F, F, F, F, F,

/* 60 */  F, F, F, F, T, F, F, F,
/* 68 */  T, F, F, F, F, F, F, F,

/* 70 */  T, T, T, T, F, F, F, F,

};

class Instr {
public:
  Instr() {
    action = 0;
    parameter = 0;
  }
  Instr(String data) {
    action = getInt(data[0]) * 16 + getInt(data[1]);
    parameter = getInt(data[2]) * 4096 + getInt(data[3]) * 256 + getInt(data[4]) * 16 + getInt(data[5]); 
  }
  Instr(byte a, unsigned short p) {
    action = a;
    parameter = p;
  }
  String getAsString() {
    char temp[8];
    sprintf(temp, "%02X%04X", action, parameter);
    return String(temp);
  }

  String getActionAsStr() {
    String saction = action < I_LAST ? INSTR[action] : "(error)";
    return saction; 
  }

  bool withParam() {
    return WITH_PARAM[action];  
  }
  
  byte action;
  unsigned parameter;
private:
  unsigned getInt(char letter) {
    if (letter >= '0' && letter <= '9')
      return letter - '0';
   if (letter >= 'a' && letter <= 'z')
     return letter - 'a' + 10;
   return letter - 'A' + 10;
  }
};

#endif
