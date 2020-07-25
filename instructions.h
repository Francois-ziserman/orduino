#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "EEPROM.h"
#include "instr.h"
#include "log.h"
#include "error.h"

#define INSTR_SIZE     1024
#define LABEL_SIZE     128
#define NO_INSTR_BREAK 10
#define NO_VALUE       0xffff
  
class Instructions {
public:
  Instructions(Error* e) {
    error = e;
  }

  void erase() {
    Log.notice(F("Erase program starting..." CR));
    unsigned short last = findLastInstrIndex();
    for (short i = 0; i < last+1; i++) {
      setAction(i, 0);
      setParameter(i, 0);
    }
    Log.notice(F("Erase program complete" CR));
  }
  
  void compile() {
    if (logClass) Log.verbose(F("Compile starting..." CR));
    for (short i = 0; i < LABEL_SIZE; i++) {
      labels[i] = NO_VALUE;
    }
    unsigned short last = findLastInstrIndex();
    for (short i = 0; i < last+1; i++) {
      if (getAction(i) == I_LABEL) {
        unsigned short id = getParameter(i);
        if (id >= LABEL_SIZE) {
          error->setOn("Label too big");
          Log.error(F("label too big line:%s Id:%s" CR), X4(i+1), X4(id));
          break;
        }
        labels[id] = i;
        if (logClass) Log.verbose(F("   Label line: %s Id:%s" CR), X4(i+1), X4(id));
      }
    }
  }
  
  unsigned short getLabelAddress(unsigned short labelIndex) {
    unsigned short address = labels[labelIndex];
    if (address == NO_VALUE) {
      error->setOn("unknown label %03X", labelIndex);
      return 0;
    }
    if (logClass) Log.verbose(F("Label(%s) -> %s" CR), X4(labelIndex), X4(address));
    return labels[labelIndex];
  }
  
  void set(Instr instr, unsigned short index) {
    if (logClass) Log.verbose(F("setInstruction - %2s %s" CR), X4(instr.action), X4(instr.parameter));
    setAction(index, instr.action);
    setParameter(index, instr.parameter);
  }

  Instr get(unsigned short index) {    
    Instr instr = Instr(getAction(index), getParameter(index));
    Log.verbose(F("Instruction.get Line:%i Action:%i Parameter:%i" CR), index, instr.action, instr.parameter);
    return instr;
  }

  void deleteInstr(unsigned short index) {
    unsigned short last = findLastInstrIndex();
    for (unsigned short i = index; i < last+1; i++) {
      setAction(i, getAction(i+1));
      setParameter(i, getParameter(i+1));
    }
    setAction(INSTR_SIZE - 1, 0);
    setParameter(INSTR_SIZE - 1, 0);
  }
  
  void insertInstr(unsigned short index) {
    unsigned short last = findLastInstrIndex();    
    for (unsigned short i = last+1; i > index; i--) {
      setAction(i, getAction(i-1));
      setParameter(i, getParameter(i-1));
    }
    setAction(index, 0);
    setParameter(index, 0);
  }

  unsigned short findLastInstrIndex() {
    unsigned noopNbr = 0;
    for (unsigned short i = 0; i < INSTR_SIZE; i++) {
      byte action = getAction(i);
      if (action != 0) {
        noopNbr = 0;
      } else { // action == 0
        noopNbr++;
        if (noopNbr >= NO_INSTR_BREAK) {
          if (logClass) Log.verbose(F("last instr : %s" CR), X4(i-NO_INSTR_BREAK+1));
          return i-NO_INSTR_BREAK; 
        }      
      }  
    }
    if (logClass) Log.verbose(F("  last instr : %s CR"), X4(INSTR_SIZE-1));
    return INSTR_SIZE-1;
  }

private:
  void setAction(unsigned short address, byte value) {
    EEPROM.update(address, value);
  }

  byte getAction(unsigned short index) {
    return EEPROM.read(index);
  }

  void setParameter(unsigned short address, unsigned value) {
    unsigned left = value / 0x100;
    unsigned right = value - (left * 0x100);
    EEPROM.update(address+INSTR_SIZE, left);
    EEPROM.update(address+INSTR_SIZE*2, right);
  }

  unsigned getParameter(unsigned short address) {
    unsigned left = EEPROM.read(address+INSTR_SIZE);
    unsigned right = EEPROM.read(address+INSTR_SIZE*2);
    return left * 0x100 + right;
  }

  Error* error;
  unsigned short labels[LABEL_SIZE];
  bool logClass = false;
};

#endif
