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

// 00  "no op  ", "STOP   ", "MOV A  ", "MOV B  ", "MOV C  ", "MOV VA ", "MOV VB ", "MOV VC ",
// 08  "MOV RA ", "MOV RB ", "MOV RC ", "MOVRRAB", "--     ", "--     ", "--     ", "--     ",
const char _instr_00[] PROGMEM = "no op  ";
const char _instr_01[] PROGMEM = "STOP   ";
const char _instr_02[] PROGMEM = "MOV A  ";
const char _instr_03[] PROGMEM = "MOV B  ";
const char _instr_04[] PROGMEM = "MOV C  ";
const char _instr_05[] PROGMEM = "MOV VA ";
const char _instr_06[] PROGMEM = "MOV VB ";
const char _instr_07[] PROGMEM = "MOV VC ";
const char _instr_08[] PROGMEM = "MOV RA ";
const char _instr_09[] PROGMEM = "MOV RB ";
const char _instr_0A[] PROGMEM = "MOV RC ";
const char _instr_0B[] PROGMEM = "MOVRRAB";
const char _instr_0C[] PROGMEM = "--     ";
const char _instr_0D[] PROGMEM = "--     ";
const char _instr_0E[] PROGMEM = "--     ";
const char _instr_0F[] PROGMEM = "--     ";

// 10   "ADD    ", "MINUS  ", "MULT   ", "DIV    ", "A++    ", "B++    ", "C++    ", "A--    ",
// 18   "B--    ", "C--    ", "INV A  ", "INV B  ", "INV C  ", "--     ", "--     ", "--     ",
const char _instr_10[] PROGMEM = "ADD    ";
const char _instr_11[] PROGMEM = "MINUS  ";
const char _instr_12[] PROGMEM = "MULT   ";
const char _instr_13[] PROGMEM = "DIV    ";
const char _instr_14[] PROGMEM = "A++    ";
const char _instr_15[] PROGMEM = "B++    ";
const char _instr_16[] PROGMEM = "C++    ";
const char _instr_17[] PROGMEM = "A--    ";
const char _instr_18[] PROGMEM = "B--    ";
const char _instr_19[] PROGMEM = "C--    ";
const char _instr_1A[] PROGMEM = "INV A  ";
const char _instr_1B[] PROGMEM = "INV B  ";
const char _instr_1C[] PROGMEM = "INV C  ";
const char _instr_1D[] PROGMEM = "--     ";
const char _instr_1E[] PROGMEM = "--     ";
const char _instr_1F[] PROGMEM = "--     ";

// 20   "LABEL  ", "JUMP   ", "JMP A Z", "JMP B Z", "JMP C Z", "JMP A!Z", "JMP B!Z", "JMP C!Z",
// 28   "JMP A=B", "JMP A!B", "JMP A=C", "JMP A!C", "JMP B=C", "JMP_B!C", "JMP  RA", "JMP ADA",
const char _instr_20[] PROGMEM = "LABEL  ";
const char _instr_21[] PROGMEM = "JUMP   ";
const char _instr_22[] PROGMEM = "JMP A Z";
const char _instr_23[] PROGMEM = "JMP B Z";
const char _instr_24[] PROGMEM = "JMP C Z";
const char _instr_25[] PROGMEM = "JMP A!Z";
const char _instr_26[] PROGMEM = "JMP B!Z";
const char _instr_27[] PROGMEM = "JMP C!Z";
const char _instr_28[] PROGMEM = "JMP A=B";
const char _instr_29[] PROGMEM = "JMP A!B";
const char _instr_2A[] PROGMEM = "JMP A=C";
const char _instr_2B[] PROGMEM = "JMP A!C";
const char _instr_2C[] PROGMEM = "JMP B=C";
const char _instr_2D[] PROGMEM = "JMP_B!C";
const char _instr_2E[] PROGMEM = "JMP  RA";
const char _instr_2F[] PROGMEM = "JMP ADA";

// 30   "JMP SUB", "RETURN ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
// 38   "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_30[] PROGMEM = "JMP SUB";
const char _instr_31[] PROGMEM = "RETURN ";
const char _instr_32[] PROGMEM = "--     ";
const char _instr_33[] PROGMEM = "--     ";
const char _instr_34[] PROGMEM = "--     ";
const char _instr_35[] PROGMEM = "--     ";
const char _instr_36[] PROGMEM = "--     ";
const char _instr_37[] PROGMEM = "--     ";
const char _instr_38[] PROGMEM = "--     ";
const char _instr_39[] PROGMEM = "--     ";
const char _instr_3A[] PROGMEM = "--     ";
const char _instr_3B[] PROGMEM = "--     ";
const char _instr_3C[] PROGMEM = "--     ";
const char _instr_3D[] PROGMEM = "--     ";
const char _instr_3E[] PROGMEM = "--     ";
const char _instr_3F[] PROGMEM = "--     ";

// 40   "SCR ON ", "SCR OFF", "SCR CLR", "SCR NL ", "INPUT A", "INPUT B", "INPUT C", "INPUT M",
// 48   "PRT A  ", "PRT B  ", "PRT C  ", "PRT M  ", "PRTCH  ", "PRTCH A", "PRTCH M", "--     ",
const char _instr_40[] PROGMEM = "SCR ON ";
const char _instr_41[] PROGMEM = "SCR OFF";
const char _instr_42[] PROGMEM = "SCR CLR";
const char _instr_43[] PROGMEM = "SCR NL ";
const char _instr_44[] PROGMEM = "INPUT A";
const char _instr_45[] PROGMEM = "INPUT B";
const char _instr_46[] PROGMEM = "INPUT C";
const char _instr_47[] PROGMEM = "INPUT M";
const char _instr_48[] PROGMEM = "PRT A  ";
const char _instr_49[] PROGMEM = "PRT B  ";
const char _instr_4A[] PROGMEM = "PRT C  ";
const char _instr_4B[] PROGMEM = "PRT M  ";
const char _instr_4C[] PROGMEM = "PRTCH  ";
const char _instr_4D[] PROGMEM = "PRTCH A";
const char _instr_4E[] PROGMEM = "PRTCH M";
const char _instr_4F[] PROGMEM = "--     ";

// 50   "SW C A ", "SW C B ", "SW A B ", "LSHIFT ", "RSHIFT ", "AND    ", "OR     ", "RND    ",
// 58   "RND A  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_50[] PROGMEM = "SW C A ";
const char _instr_51[] PROGMEM = "SW C B ";
const char _instr_52[] PROGMEM = "SW A B ";
const char _instr_53[] PROGMEM = "LSHIFT ";
const char _instr_54[] PROGMEM = "RSHIFT ";
const char _instr_55[] PROGMEM = "AND    ";
const char _instr_56[] PROGMEM = "OR     ";
const char _instr_57[] PROGMEM = "RND    ";
const char _instr_58[] PROGMEM = "RND A  ";
const char _instr_59[] PROGMEM = "--     ";
const char _instr_5A[] PROGMEM = "--     ";
const char _instr_5B[] PROGMEM = "--     ";
const char _instr_5C[] PROGMEM = "--     ";
const char _instr_5D[] PROGMEM = "--     ";
const char _instr_5E[] PROGMEM = "--     ";
const char _instr_5F[] PROGMEM = "--     ";

// 60   "ST INIT", "PUSH A ", "PUSH B ", "PUSH C ", "PUSH M ", "POP A  ", "POP B  ", "POP C  ",
// 68   "POP M  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_60[] PROGMEM = "ST INIT";
const char _instr_61[] PROGMEM = "PUSH A ";
const char _instr_62[] PROGMEM = "PUSH B ";
const char _instr_63[] PROGMEM = "PUSH C ";
const char _instr_64[] PROGMEM = "PUSH M ";
const char _instr_65[] PROGMEM = "POP A  ";
const char _instr_66[] PROGMEM = "POP B  ";
const char _instr_67[] PROGMEM = "POP C  ";
const char _instr_68[] PROGMEM = "POP M  ";
const char _instr_69[] PROGMEM = "--     ";
const char _instr_6A[] PROGMEM = "--     ";
const char _instr_6B[] PROGMEM = "--     ";
const char _instr_6C[] PROGMEM = "--     ";
const char _instr_6D[] PROGMEM = "--     ";
const char _instr_6E[] PROGMEM = "--     ";
const char _instr_6F[] PROGMEM = "--     ";

// 70   "SLEEP  ", "SLEEP A", "PINMOUT", "PMODINP", "DREAD  ", "DWRITE ", "AREAD  ", "AWRITE ",
const char _instr_70[] PROGMEM = "SLEEP  ";
const char _instr_71[] PROGMEM = "SLEEP A";
const char _instr_72[] PROGMEM = "PINMOUT";
const char _instr_73[] PROGMEM = "PMODINP";
const char _instr_74[] PROGMEM = "DREAD  ";
const char _instr_75[] PROGMEM = "DWRITE ";
const char _instr_76[] PROGMEM = "AREAD  ";
const char _instr_77[] PROGMEM = "AWRITE ";
const char _instr_78[] PROGMEM = "--     ";
const char _instr_79[] PROGMEM = "--     ";
const char _instr_7A[] PROGMEM = "--     ";
const char _instr_7B[] PROGMEM = "--     ";
const char _instr_7C[] PROGMEM = "--     ";
const char _instr_7D[] PROGMEM = "--     ";
const char _instr_7E[] PROGMEM = "--     ";
const char _instr_7F[] PROGMEM = "--     ";



const char* const INSTR[] PROGMEM = {
  _instr_00, _instr_01, _instr_02, _instr_03, _instr_04, _instr_05, _instr_06, _instr_07,
  _instr_08, _instr_09, _instr_0A, _instr_0B, _instr_0C, _instr_0D, _instr_0E, _instr_0F,

  _instr_10, _instr_11, _instr_12, _instr_13, _instr_14, _instr_15, _instr_16, _instr_17,
  _instr_18, _instr_19, _instr_1A, _instr_1B, _instr_1C, _instr_1D, _instr_1E, _instr_1F,

  _instr_20, _instr_21, _instr_22, _instr_23, _instr_24, _instr_25, _instr_26, _instr_27,
  _instr_28, _instr_29, _instr_2A, _instr_2B, _instr_2C, _instr_2D, _instr_2E, _instr_2F,

  _instr_30, _instr_31, _instr_32, _instr_33, _instr_34, _instr_35, _instr_36, _instr_37,
  _instr_38, _instr_39, _instr_3A, _instr_3B, _instr_3C, _instr_3D, _instr_3E, _instr_3F,

  _instr_40, _instr_41, _instr_42, _instr_43, _instr_44, _instr_45, _instr_46, _instr_47,
  _instr_48, _instr_49, _instr_4A, _instr_4B, _instr_4C, _instr_4D, _instr_4E, _instr_4F,

  _instr_50, _instr_51, _instr_52, _instr_53, _instr_54, _instr_55, _instr_56, _instr_57,
  _instr_58, _instr_59, _instr_5A, _instr_5B, _instr_5C, _instr_5D, _instr_5E, _instr_5F,

  _instr_60, _instr_61, _instr_62, _instr_63, _instr_64, _instr_65, _instr_66, _instr_67,
  _instr_68, _instr_69, _instr_6A, _instr_6B, _instr_6C, _instr_6D, _instr_6E, _instr_6F,

  _instr_70, _instr_71, _instr_72, _instr_73, _instr_74, _instr_75, _instr_76, _instr_77,
  _instr_78, _instr_79, _instr_7A, _instr_7B, _instr_7C, _instr_7D, _instr_7E, _instr_7F,

};

const static bool WITH_PARAM[] PROGMEM = {
/*        0  1  2  3  4  5  6  7  */

// 00  "no op  ", "STOP   ", "MOV A  ", "MOV B  ", "MOV C  ", "MOV VA ", "MOV VB ", "MOV VC ",
// 08  "MOV RA ", "MOV RB ", "MOV RC ", "MOVRRAB", "--     ", "--     ", "--     ", "--     ",
/* 00 */  0, 0, 1, 1, 1, 1, 1, 1,
/* 08 */  1, 1, 1, 0, 0, 0, 0, 0,

// 10   "ADD    ", "MINUS  ", "MULT   ", "DIV    ", "A++    ", "B++    ", "C++    ", "A--    ",
// 18   "B--    ", "C--    ", "INV A  ", "INV B  ", "INV C  ", "--     ", "--     ", "--     ",
/* 10 */  0, 0, 0, 0, 0, 0, 0, 0,
/* 18 */  0, 0, 0, 0, 0, 0, 0, 0,

// 20   "LABEL  ", "JUMP   ", "JMP A Z", "JMP B Z", "JMP C Z", "JMP A!Z", "JMP B!Z", "JMP C!Z",
// 28   "JMP A=B", "JMP A!B", "JMP A=C", "JMP A!C", "JMP B=C", "JMP_B!C", "JMP  RA", "JMP ADA",
/* 20 */  1, 1, 1, 1, 1, 1, 1, 1,
/* 28 */  1, 1, 1, 1, 1, 1, 0, 0,

// 30   "JMP SUB", "RETURN ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
// 38   "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
/* 30 */  1, 0, 0, 0, 0, 0, 0, 1,
/* 38 */  1, 0, 0, 0, 0, 0, 0, 1,

// 40   "SCR ON ", "SCR OFF", "SCR CLR", "SCR NL ", "INPUT A", "INPUT B", "INPUT C", "INPUT M",
// 48   "PRT A  ", "PRT B  ", "PRT C  ", "PRT M  ", "PRTCH  ", "PRTCH A", "PRTCH M", "--     ",
/* 40 */  0, 0, 0, 0, 0, 0, 0, 1,
/* 48 */  0, 0, 0, 1, 1, 0, 1, 0,

// 50   "SW C A ", "SW C B ", "SW A B ", "LSHIFT ", "RSHIFT ", "AND    ", "OR     ", "RND    ",
// 58   "RND A  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
/* 50 */  0, 0, 0, 1, 1, 0, 0, 1,
/* 58 */  0, 0, 0, 0, 0, 0, 0, 0,

// 60   "ST INIT", "PUSH A ", "PUSH B ", "PUSH C ", "PUSH M ", "POP A  ", "POP B  ", "POP C  ",
// 68   "POP M  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
/* 60 */  0, 0, 0, 0, 1, 0, 0, 0,
/* 68 */  1, 0, 0, 0, 0, 0, 0, 0,

// 70   "SLEEP  ", "SLEEP A", "PINMOUT", "PMODINP", "DREAD  ", "DWRITE ", "AREAD  ", "AWRITE ",
/* 70 */  1, 1, 1, 1, 0, 0, 0, 0,

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
    if (action >= I_LAST)
      return "(error)";
      
    char temp[8];
    strcpy_P(temp, (char*)pgm_read_word(&(INSTR[action])));
    return temp; 
  }

  bool withParam() {
    bool ret = pgm_read_word_near(WITH_PARAM + action);
    return ret; //WITH_PARAM[action];  
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
