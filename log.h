#ifndef LOG_H
#define LOG_H

#include "Arduino.h"
#include <ArduinoLog.h>

inline String x4(unsigned i) {
  char temp[5];
  sprintf(temp, "%4X", i);
  return temp;
}

#define X4(e) (x4(e).c_str())

#endif
