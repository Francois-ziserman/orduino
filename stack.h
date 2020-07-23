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
    int index = incrIndex();
    if (error->isOn()) {
      return;
    }
    ram->setAt(address-1 - index, value);    
    Log.trace("  Stack::push at the address %s : %s" CR, X4(address-1 - index), X4(value));
  }
  
  unsigned pop() {
    int index = getIndex();
    if (index <= 0) {
      error->setOn("Stack empty");
      return 0;
    }
    unsigned ret = ram->getAt(address-1 - index);
    Log.trace("  Stack::get at the address %s : %s" CR, X4(address-1 - index), X4(ret));
    decrIndex();
    return ret;
  }
  
  void init() {
    initIndex();
  }

  unsigned getIndexAddress() { return address; }
  unsigned getFirstAddress() { return address-1; }
  unsigned getLastAddress() { return address-1-STACK_SIZE; }

  unsigned getIndex() {
    Log.verbose("  Stack::index %s = %s" CR, X4(address), X4(ram->getAt(address))); 
    return ram->getAt(address);
  }

private:
  void initIndex() {
    ram->setAt(address, 0);
    Log.verbose("  Stack::init index %s = %s", X4(address), X4(getIndex())); 
  }
  

  int incrIndex() {
    int index = getIndex();
    if (index >= STACK_SIZE) {
      error->setOn("Stack overflow:%03x", index);
      return 0;
    }
    ram->setAt(address, ++index);
    return index;
  }

  int decrIndex() {
    int index = getIndex();
    ram->setAt(address, --index);
    return index;
  }
  
  Ram*ram;
  Error* error;
  unsigned address;
};

#endif
