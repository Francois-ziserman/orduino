#ifndef SCREEN_H
#define SCREEN_H

#include "ram.h"
#include "lcd.h"

#define SCREEN_NB_LINES    4
#define SCREEN_NB_COLS     20
#define SCREEN_NB_CAR      (SCREEN_NB_LINES * SCREEN_NB_COLS)
#define SCREEN_RAM_START   (RAM_SIZE - (SCREEN_NB_CAR/2 + 2))
#define SCREEN_RAM_X       (RAM_SIZE - 1)
#define SCREEN_RAM_Y       (RAM_SIZE - 2)

#define KEY_ADDRESS        (SCREEN_RAM_START-1)  


class Screen {
public:
  Screen(Ram* r, Lcd* l) {
    ram = r;
    lcd = l;
    screenOn = false;
  };

  void init() {
    initContent();
  }

  bool isOn() { return screenOn; }
  
  void switchOn() {
     screenOn = true;
    updateScreen();  
  }

  void switchOff() {
    screenOn = false;
  }

  void clear() {
    initContent();
    updateScreen();    
    setCursor(0, 0);
  }

  void initContent() {
    for (byte i = 0; i < SCREEN_NB_LINES; i++) {
      clearLine(i);
    }
    setCursor(0, 0);
  }

  void printValue(String name, unsigned value) {
    sprintf(temp, "%s : %u", name.c_str(), value);
    print(temp);
  }

  void print(String message) {
    for(byte i = 0; i < message.length(); i++) {
      printC(message[i], false);
    }
    updateScreen();
  }
  
  void printC(char car, bool update) {
    setAt(getY(), getX(), car);
    incrX();
    if (update)
      updateScreen();
  }
  
  void setNewLine() {
    byte currentLineIndex = getY();
    if (currentLineIndex == SCREEN_NB_LINES - 1) {
      scroll();
      setCursor(0, currentLineIndex);
    } else {
      clearLine(currentLineIndex+1);
      setCursor(0, currentLineIndex+1);
    }
  }

  void scroll() {
     for (byte i = 1; i < SCREEN_NB_LINES; i++) {
      for (byte j = 0; j < SCREEN_NB_COLS; j++) {
        setAt(i-1, j, getAt(i, j));
      }
    }
    clearLine(SCREEN_NB_LINES-1);
  }

  void clearLine(byte i) {
    for (byte j = 0; j < SCREEN_NB_COLS; j++) {
      setAt(i, j, ' ');
    }
  }
  
  String getLine(byte line) {
    String ret = "";
    for (byte i = 0; i < SCREEN_NB_COLS; i++) {
      ret += String(getAt(line, i));
    };
    return ret;
  }

  void backX() {
    decrX();
    printC(' ', true);
    decrX();
  }
  
  void setAt(byte line, byte col, char car) {
    unsigned v = ram->getAt(getRamIndex(line,col));
    char left = v / 256;
    char right = v - (left * 256);
    if (col & 1) { // pair
      left = car; 
    } else {
      right = car;
    }
    v = left * 256 + right;
    ram->setAt(getRamIndex(line,col), v); 
  }

  char getAt(byte line, byte col) {
    unsigned v = ram->getAt(getRamIndex(line,col));
    char left = v / 256;
    char right = v - (left * 256);
    char ret = col & 1 ? left : right;
    return ret == 0 ? ' ' : ret;
  }

  unsigned getFirstAddress() {
    return getRamIndex(0, 0);
  }

  unsigned getLastAddress() {
    return getRamIndex(SCREEN_NB_LINES-1, SCREEN_NB_COLS-1);
  }
  
  byte getX() { return ram->getAt(SCREEN_RAM_X); }

  byte getY() { return ram->getAt(SCREEN_RAM_Y); }

  void setX(byte x) { ram->setAt(SCREEN_RAM_X, x); }

  void setY(byte y) { ram->setAt(SCREEN_RAM_Y, y); }
  
  void setCursor(byte x, byte y) { setX(x); setY(y);  lcd->setCursor(x, y); }

  void setCursor() { lcd->setCursor(getX(), getY()); }

  void decrX() {
    byte x = getX();
    if (x == 0) {
      if (decrY()) {
        setX(SCREEN_NB_COLS -1);
      }   
    } else {
      setX(x-1);
    }
  }

  bool decrY() {
    byte y = getY();
    if (y == 0) {
      return false;
    }
    setY(y-1);
  }
  
  void incrX() {
    byte x = getX();
    if (x == SCREEN_NB_COLS -1) {
      setX(0);
      incrY();
    } else {
      setX(x+1);
    }
  }

  void incrY() {
    byte y = getY();
    if (y == SCREEN_NB_LINES - 1) {
      scroll();  
    } else {
      setY(y+1);
    }
  }

  unsigned getRamIndex(byte line, byte col) {
    return SCREEN_RAM_START + ((line * SCREEN_NB_COLS) + col)/2; 
  }

  void updateScreen() {
    for (byte i = 0; i < SCREEN_NB_LINES; i++) {
      String line = getLine(i);
      lcd->setLine(i, line);
    }
    lcd->display();
  }

  void refreshCursor() {
    lcd->refreshCursor();
  }
  
private:
  Ram* ram;
  Lcd* lcd;
  bool screenOn;
};

#endif
