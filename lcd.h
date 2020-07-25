#ifndef LCD_H
#define LCD_H

#include "log.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

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
      lines[i] = String("                    ");
    }
    blink = 0;    
    behindCursor = lines[cursor_y][cursor_x];
  }

  void setLine(unsigned short index, String value) {
    lines[index] = value;
  }

  void initLine(unsigned short index) {
    lines[index] = String("                    ");
  }

  void setLine(unsigned short index) {
    Log.verbose(F("Lcd.setLine %i '%s'" CR), index, line);
   lines[index] = String(line);
  }

  void setCursor(byte x, byte y) {
    initCursor();
    cursor_x = x;
    cursor_y = y;
    behindCursor = lines[cursor_y][cursor_x];
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
  
  char line[21];
  
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
