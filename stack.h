#ifndef STACK_H
#define STACK_H

#include "ram.h"
#include "log.h"

#define STACK_SIZE 256

class Stack {
public:
  Stack(Ram* r, unsigned start, Error* e) {
    ram = r;
    address = start;
    error = e;
    init();
  }
  
  void push(unsigned value) {
    incrIndex();
    if (error->isOn()) {
      return;
    }
    ram->setAt(getAddress(), value);    
    Log.trace(F("  Stack::push at the address %s : %s" CR), X4(getAddress()), X4(value));
  }
  
  unsigned pop() {
    if (getIndex() <= 0) {
      error->setOn(F("Stack empty"));
      return 0;
    }
    unsigned ret = ram->getAt(getAddress());
    Log.trace(F("  Stack::get at the address %s : %s" CR), X4(getAddress()), X4(ret));
    decrIndex();
    return ret;
  }
  
  void subPush(unsigned value) {
    subIncrIndex();
    if (error->isOn()) {
      return;
    }
    ram->setAt(subGetAddress(), value);    
    Log.trace(F("  Stack::push at the address %s : %s" CR), X4(subGetAddress()), X4(value));
  }
  
  unsigned subPop() {
    if (subGetIndex() <= 0) {
      error->setOn(F("Stack empty"));
      return 0;
    }
    unsigned ret = ram->getAt(subGetAddress());
    Log.trace(F("  Stack::get at the address %s : %s" CR), X4(subGetAddress()), X4(ret));
    subDecrIndex();
    return ret;
  }
  
  void init() {
    initIndex();
  }

  inline unsigned getIndexAddress()    { return address; }
  inline unsigned subGetIndexAddress() { return address-1; }
  inline unsigned getTopAddress()      { return address-2; }
  inline unsigned getBottomAddress()   { return address-2-STACK_SIZE; }
  inline unsigned getIndex()           { return ram->getAt(getIndexAddress()); }
  inline unsigned subGetIndex()        { return ram->getAt(subGetIndexAddress()); }
  inline unsigned getAddress()         { return getTopAddress() - (getIndex() - 1); }
  inline unsigned subGetAddress()      { return getBottomAddress() + subGetIndex() - 1; };
  
private:
  void initIndex() {
    ram->setAt(getIndexAddress(), 0);
    ram->setAt(subGetIndexAddress(), 0);
  } 

  int incrIndex() {
    int index = getIndex();
    int other = subGetIndex();
    if (index + other > STACK_SIZE) {
      error->setOn(F("Stack overflow:%03x"), index);
      return 0;
    }
    index++;
    ram->setAt(address, index);
    return index;
  }

  int decrIndex() {
    int index = getIndex();
    if (index == 0) {
      error->setOn(F("Stack empty"));
      return 0;
    }
    index--;
    ram->setAt(address, index);
    return index;
  }
  
  int subIncrIndex() {
    int index = subGetIndex();
    int other = getIndex();
    if (index + other > STACK_SIZE) {
      error->setOn(F("Stack overflow:%03x"), index);
      return 0;
    }
    index++;
    ram->setAt(address-1, index);
    return index;
  }

  int subDecrIndex() {
    int index = subGetIndex();
    if (index == 0) {
      error->setOn(F("Stack empty"));
      return 0;
    }
    index--;
    ram->setAt(address-1, index);
    return index;
  }
  
  Ram*ram;
  Error* error;
  unsigned address;
};

#endif
