#ifndef INSTR_H
#define INSTR_H

#include "util.h"
#include <avr/pgmspace.h>
#include "log.h"

#include "instr_define.h"
#include "instr_const.h"
#include "instr_progmem.h"

class Instr {
public:
  Instr() {
    action = 0;
    parameter = 0;
  }
  Instr(String data) {
    action = getCharToInt(data[0]) * 16 + getCharToInt(data[1]);
    parameter = getCharToInt(data[2]) * 4096 + getCharToInt(data[3]) * 256 + getCharToInt(data[4]) * 16 + getCharToInt(data[5]); 
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

  inline String getParamAsString() {
    return getAsString().substring(2, 6);
  }

  String getForFile() {
    if (withParam()) {
      String ret = getActionAsStr();
      ret += ' ';  
      ret += getParamAsString();
      return ret;
    } 
    return getActionAsStr();
  }

  inline bool withParam() {
    return pgm_read_word_near(WITH_PARAM + action);
  }
  
  byte action;
  unsigned parameter;
private:
};

byte getActionFromStr(String line) {
  line = rightTrim(line);
  if (line.length() > 7)
    line = line.substring(0, 7);
  
  for (byte i = 0; i < I_LAST; i++) {
    Instr instr(i, 0);
    if (rightTrim(instr.getActionAsStr()).equals(line)) {
      return instr.action;
    }
  }
  Log.error(F("Instruction not found : '%s' %i" CR), line.c_str(), line.length() );
  return 0;
}

String splitLeft(String value) {
  String ret = "";
  byte i = 0;
  // Go to first char
  for (; i < value.length(); i++) {
    if (value[i] != ' ')
      break;
  }
  // value[i] != ' '
  for (; i < value.length(); i++) {
    if (value[i] == ' ' && i > 4 )
      break;
    ret += value[i];
  }
  return ret;
}

String splitRight(String value) {
  value = rightTrim(value);
  for (int i = value.length() - 1; i >= 0; i--) {
    if (value.charAt(i) == ' ') {
      return value.substring(i+1);
    }
  }
  return String("0000"); // default value
}

Instr createInstrFromFile(String line) {
  String saction = splitLeft(line);
  byte action = getActionFromStr(saction);
  String sparameter = splitRight(line);
  char temp[8];
  sprintf(temp, "%02X%s", action, sparameter.c_str());
  Log.verbose(F("  -> '%s' --> %s-%s --> '%s'" CR), line.c_str(), saction.c_str(), sparameter.c_str(),String(temp).c_str());
  return Instr(String(temp));
}


#endif
