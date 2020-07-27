#ifndef RAM_H
#define RAM_H

#include "EEPROM.h"
#include "Instructions.h"
#include "log.h"

#define RAM_SIZE          1024
#define RAM_IN_EPROM_SIZE 1024
/* 
 * 2 parts : 1k in RAM, 1k in eprom 
 * 
 * in the eprom, it will used 2k bits 
 */

#define RAM_EPROM_START   INSTR_SIZE*3

class Ram {
public:
  Ram(Error* e) {
    error = e;
  }
  
  unsigned short getAt(unsigned index) {
    if (index >= RAM_SIZE + RAM_IN_EPROM_SIZE) {
      error->setOn(("ram adr %03X inv."), index);
      Log.error(F("Ram::get - Invalid Address : %s"), X4(index));
      return 0;
    }
    if (index >= RAM_SIZE) {
      unsigned left =  EEPROM.read(index - RAM_SIZE + RAM_EPROM_START);
      unsigned right = EEPROM.read(index - RAM_SIZE + RAM_EPROM_START + RAM_IN_EPROM_SIZE);
      return left * 0x100 + right;
    }
    //Log.verbose(F("  Ram::get(%s) -> %s" CR), X4(index), X4(memory[index]));
    return memory[index];
  }

  void setAt(unsigned index, unsigned value) {
    if (index >= RAM_SIZE + RAM_IN_EPROM_SIZE) {
      error->setOn(("ram adr %03X inv."), index);
      return;
    }
    if (index >= RAM_SIZE) {
      unsigned left = value / 0x100;
      unsigned right = value - (left * 0x100);
      EEPROM.update(index - RAM_SIZE + RAM_EPROM_START, left);
      EEPROM.update(index - RAM_SIZE + RAM_EPROM_START + RAM_IN_EPROM_SIZE, right);
      return;
    }
    memory[index] = value;
    //Log.verbose(F("  Ram::set(%s, %s) -> %s" CR), X4(index), X4(value), X4(memory[index]));
  }

  void erase() {
    Log.notice(F("Erase RAM" CR));
    for(unsigned i = 0; i < RAM_SIZE; i++) {
      setAt(i, 0);
    }
  }

private:
  Error* error;
  unsigned short memory[RAM_SIZE];
};

#endif
