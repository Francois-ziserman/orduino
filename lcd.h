#ifndef LCD_H
#define LCD_H

#include "log.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "key.h"

#define BLINK_INTERVAL 30000

LiquidCrystal_I2C _lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

class Lcd {
public:
  Lcd() {
     _lcd.begin(20,4);  
  }
  
  void initDisplay() {
    Log.verbose(F("Lcd.initDiaplay" CR));
    for(short i = 0; i < 4; i++) {
      lines[i] = F("                    ");
    }
    blink = 0;    
    behindCursor = lines[cursor_y][cursor_x];
  }

  void setLine(unsigned short index, String value) {
    while(value.length() < 20)
      value += ' ';
    lines[index] = value;
  }

  void initLine(unsigned short index) {
    lines[index] = F("                    ");
  }

  void setChar(unsigned short index, unsigned short position, char c) {
    lines[index][position] = c;
    behindCursor = lines[cursor_y][cursor_x];
  }

  void setCursor(byte x, byte y) {
    initCursor();
    cursor_x = x;
    cursor_y = y;
    behindCursor = lines[cursor_y][cursor_x];
  }
  
  void displaySingleMessage(String title, String message) {
    initDisplay();
    setCursor(0, 0);
    setLine(0, title);
    setLine(1, message);
    display();
  }

  void displaySingleMessageWithValidate(String title, String message) {
    initDisplay();
    setCursor(0, 0);
    setLine(0, title);
    setLine(1, message);
    setLine(3, F("Press enter"));
    display();
    while(true) 
      if (keypad.getKey() == KEY_ENTER)
        return;
  }

  void display() {
    Log.verbose(F("Lcd.diaplay" CR));
    for(short i = 0; i < 4; i++) {
      _lcd.setCursor(0,i);
      _lcd.print(lines[i]);
      Log.verbose(F("  %i: '%s'" CR), i, lines[i].c_str());
   }
   behindCursor = lines[cursor_y][cursor_x];
  }

  void refreshCursor() {
    blink++;
    if (blink == 1 || blink == BLINK_INTERVAL*2) {
      behindCursor = lines[cursor_y][cursor_x];
      lines[cursor_y].setCharAt(cursor_x, '_');
      _lcd.setCursor(0,cursor_y);
      _lcd.print(lines[cursor_y]);
      if (blink == BLINK_INTERVAL*2) {
        blink = 2;
      }
    } else if (blink == BLINK_INTERVAL) {
      lines[cursor_y].setCharAt(cursor_x, behindCursor);
      _lcd.setCursor(0,cursor_y);
      _lcd.print(lines[cursor_y]);
      behindCursor = '#';
    } 
  }
  
  inline String getLine(unsigned short index) { return lines[index]; }
  
private:
  void initCursor() {
    if (behindCursor != '#') {
      lines[cursor_y].setCharAt(cursor_x, behindCursor);
      _lcd.setCursor(0,cursor_y);
      _lcd.print(lines[cursor_y]);   
    } else { // behindCursor == '#'
      behindCursor = lines[cursor_y][cursor_x];
    }
    blink = 0; 
  }

  unsigned blink;
  String lines[4];
  byte cursor_x, cursor_y = 0;
  char behindCursor;
  bool logClass = true;
};

#endif
