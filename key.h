#ifndef KEY_H
#define KEY_H
//
#include "Keypad.h"

#define KEY_ROWS         8
#define KEY_COLS         4

#define KEY_SWITCH_MODE          'G'
#define KEY_RUN_STOP             'K'
#define KEY_SWITCH_EDIT_FILE     'K'
#define KEY_RESET                'S'
#define KEY_FILE_COPY            'S'
#define KEY_LINE_UP              'I'
#define KEY_LINE_DOWN            'M'
#define KEY_PAGE_UP              'H'
#define KEY_PAGE_DOWN            'J'
#define KEY_RIGHT                'N'
#define KEY_LEFT                 'L'
#define KEY_ENTER                'P'
#define KEY_DELETE_LINE          'Q'
#define KEY_INSERT_LINE          'R'
#define KEY_FILE_RENAME          'R'
#define KEY_STEP_BY_STEP         'O'
#define KEY_CLOCK_EDIT           'O'
#define KEY_DUMP_PRG_TO_SERIAL   'V'
#define KEY_ERASE_PROGRAM        'T'
#define KEY_FILE_LOAD            'T'
#define KEY_ERASE_RAM            'U'
                                 
#define KEY_HEX_0                '0'
#define KEY_HEX_1                '1'
#define KEY_HEX_2                '2'
#define KEY_HEX_3                '3'
#define KEY_HEX_4                '4'
#define KEY_HEX_5                '5'
#define KEY_HEX_6                '6'
#define KEY_HEX_7                '7'
#define KEY_HEX_8                '8'
#define KEY_HEX_9                '9'
#define KEY_HEX_A                'A'
#define KEY_HEX_B                'B'
#define KEY_HEX_C                'C'
#define KEY_HEX_D                'D'
#define KEY_HEX_E                'E'
#define KEY_HEX_F                'F'

const char _keys[KEY_ROWS][KEY_COLS] = {  
  {'G', 'H', 'I', 'J'}, 
  {'K', 'L', 'M', 'N'},
  {'O', 'P', 'Q', 'R'},
  {'S', 'T', 'U', 'V'},  

  {'0', '1', '2', '3'}, 
  {'4', '5', '6', '7'},  
  {'8', '9', 'A', 'B'}, 
  {'C', 'D', 'E', 'F'},

};

const byte _colPins[KEY_COLS] = {44, 42, 40, 38};
const byte _rowPins[KEY_ROWS] = {36, 34, 32, 30, 28, 26, 24, 22};

Keypad keypad = Keypad( makeKeymap(_keys), _rowPins, _colPins, KEY_ROWS, KEY_COLS );

unsigned getCharToInt(char letter) {
  if (letter >= '0' && letter <= '9')
    return letter - '0';
 if (letter >= 'a' && letter <= 'z')
   return letter - 'a' + 10;
 return letter - 'A' + 10;
}

#endif
