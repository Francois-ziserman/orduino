#ifndef CLOCK_H
#define CLOCK_H

#include <DS1307RTC.h>
#include "lcd.h"
#include "key.h"
#include "log.h"
#include "error.h"

#define TIME_TO_STR "%02d/%02d/%04d  %02d:%02d:%02d"

class Clock {
public:
  Clock(Lcd* l, Error* e) {
  	lcd = l;
  	error = e;
  }

  void edit() {
    tmElements_t tm;
    if (!RTC.read(tm)) {
      error->setOn(F("Clock Not found"));
      Log.error(F("Clock not found" CR));
      return;
    }

  	lcd->initDisplay();
  	lcd->setLine(0, F("   CLOCK SETTINGS   "));
    sprintf(lcd->line, TIME_TO_STR, 
      tm.Day, tm.Month, tmYearToCalendar(tm.Year), tm.Hour, tm.Minute, tm.Second );
    timeAsStr = lcd->line;
    lcd->setLine(2, timeAsStr);
    lcd->setCursor(0, 2);
    position = 0;
    lcd->display();
  	while(true) {
      char key = keypad.getKey();
      switch (key) {
      case NO_KEY:
        lcd->refreshCursor();
        break;
      case KEY_HEX_0:
      case KEY_HEX_1:
      case KEY_HEX_2:
      case KEY_HEX_3:
      case KEY_HEX_4:
      case KEY_HEX_5:
      case KEY_HEX_6:
      case KEY_HEX_7:
      case KEY_HEX_8:
      case KEY_HEX_9:
        setChar(key);
        right();
        break;
      case KEY_RIGHT:
        right();
        break;
      case KEY_LEFT:
        left();
        break;
      case KEY_ENTER:
        save();
        return;
      case KEY_CLOCK_EDIT:
      case KEY_SWITCH_MODE:
        // Cancel, Return to edit mode
        return;
      } 
  	}
  }

private:
  inline void setCurtor() { lcd->setCursor(position, 2); }
  
  void save() {
    Log.trace(F("Clock saving..." CR));
    tmElements_t tm;

    tm.Day = timeAsStr.substring(0, 2).toInt();
    tm.Month = timeAsStr.substring(3, 5).toInt();
    tm.Year = CalendarYrToTm(timeAsStr.substring(6, 10).toInt());

    tm.Hour = timeAsStr.substring(12, 14).toInt();
    tm.Minute = timeAsStr.substring(15, 17).toInt();
    tm.Second = timeAsStr.substring(18, 20).toInt();

    RTC.write(tm);
  }

  void setChar(char k) {
    lcd->setChar(2, position, k);
    timeAsStr[position] = k;
  }

  void right() {
    if (position == 19)
      return;
    if (position == 1 || position == 4 || position == 13 || position == 16)
      position += 2;
    else if (position == 9)
      position = 12;
    else
      position++;
    setCurtor();
  }

  void left() {
    if (position == 0)
      return;
    if (position == 3 || position == 6 || position == 15 || position == 18)
      position -= 2;
    else if (position == 12)
      position = 9;
    else
      position--;
    setCurtor();
  }

  int position = 0;

	Lcd* lcd;
	Error* error;
  String timeAsStr;
};

#endif