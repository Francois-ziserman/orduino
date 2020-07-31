#ifndef PROGRAM_H
#define PROGRAM_H

#include "key.h"
#include "instructions.h"
#include "screen.h"
#include "input.h"
#include "stack.h"
#include "error.h"
#include "clock.h"

//#include "MemoryFree.h"

#define LABELS_SIZE   64
#define PAGE 3
#define FILE_NBR 0xF

#define GETRAMX(index) x4(index < RAM_SIZE ? ram->getAt(index) : 0).c_str()

class Program {
public:
  Program(Error* e, Lcd* l, Clock* c) {
    error = e;
    lcd = l;
    clock = c;
    ram = new Ram(error);
    screen = new Screen(ram, lcd);
    stack = new Stack(ram, screen->getFirstAddress()-2, error);
    instructions = new Instructions(error);
    isRunning = false;
    stepByStep = false;
    indexExec = 0;
    keyPressed = 0;
    setScreenMode(false);
    initRegisters(); 
    eraseRam();
  };

  void startRunMode() {
    instructions->compile();
    if (screenIsOn) {
      displayScreen();
    } else {
      updateDisplay();    
    }
  }

  void oneStep(unsigned char key) {
    ram->setAt(KEY_ADDRESS, key);
    switch (key) {
    case KEY_STEP_BY_STEP:
      stepByStep = !stepByStep;
      Log.notice(stepByStep ? F("Switch to Step by Step mode" CR) : F("Switch to Normal mode" CR));
      updateDisplay();
      break;
    case KEY_DUMP_PRG_TO_SERIAL:
      printToSerial();
      break;
    case KEY_RUN_STOP:
      if (error->isOn()) {
        break;
      }
      if (stepByStep) {
        isRunning = true;
        doOneStep();
        if (screenIsOn) {
          displayScreen();
        } 
        isRunning = false;
      } else { // KEY_RUN_STOP && not stepByStep
        isRunning = !isRunning;
        Log.notice(isRunning ? F("RUN" CR) : F("STOP" CR));
        if (isRunning) {
          if (screenIsOn) {
            displayScreen();
          } else {
            updateDisplay();            
          }
        }
      }
      break;
    case KEY_RESET:
      reset();
      updateDisplay(); 
      break;
    case KEY_LINE_UP:
      if (!isRunning && indexExec > 0) {
        indexExec--;
        updateDisplay(); 
      }
      break;
    case KEY_LINE_DOWN:
      if (!isRunning && indexExec < INSTR_SIZE-1) {
        indexExec++;
        updateDisplay(); 
      }
      break;
    case KEY_PAGE_UP:
      if (indexExec == 0)
        break;
      indexExec = indexExec < PAGE ? 0 : indexExec - PAGE; 
      updateDisplay(); 
      break;
    case KEY_PAGE_DOWN:
      if (indexExec == INSTR_SIZE-1)
        break;
      indexExec = indexExec+PAGE > INSTR_SIZE-1 ? INSTR_SIZE - 1 - PAGE : indexExec + PAGE; 
      updateDisplay(); 
      break;
    case KEY_ERASE_RAM:
      if (!isRunning) {
        eraseRam();
        updateDisplay(); 
      }
      break;
    }
  };
  
  void doOneStep() {
    if (!isRunning)
      return;
    Instr instr = instructions->get(indexExec);
    if (instr.withParam()) {
      Log.trace(F("execute line %s : %s --> %s(%s)" CR), X4(indexExec+1), X4(instr.action), instr.getActionAsStr().c_str(), X4(instr.parameter)); 
    } else {
     Log.trace(F("execute line %s : %s --> %s" CR), X4(indexExec+1), X4(instr.action), instr.getActionAsStr().c_str());       
    }
    
    bool update = false;
    switch (instr.action) {
      case I_MOV_A:
        ra = ram->getAt(instr.parameter);
        update = true;
        break;
      case I_MOV_B:
        rb = ram->getAt(instr.parameter);
        update = true;
        break;
      case I_MOV_C:
        rc = ram->getAt(instr.parameter);
        update = true;
        break;
      case I_MOV_VAL_A:
        ra = (unsigned short)instr.parameter;
        update = true;
        break;
      case I_MOV_VAL_B:
        rb = (unsigned short)instr.parameter;
        update = true;
        break;
      case I_MOV_VAL_C:
        rc = (unsigned short)instr.parameter;
        update = true;
        break;
      case I_MOVR_A:
        ram->setAt(instr.parameter, ra);
        update = true;
        break;
      case I_MOVR_B:
        ram->setAt(instr.parameter, rb);
        update = true;
        break;
      case I_MOVR_C:
        ram->setAt(instr.parameter, rc);
        update = true;
        break;
      case I_MOV_TIME: 
        setRamWithTime(instr.parameter);
        update = true;
        break;
      case I_MOV_DATE: 
        setRamWithDate(instr.parameter);
        update = true;
        break;
      case I_ADD:
        rc = ra + rb;
        update = true;
        break;
      case I_MINUS:
        rc = ra - rb;
        update = true;
        break;
      case I_MULT:
        rc = ra * rb;
        update = true;
        break;
      case I_DIV:
        if (rb == 0) {
          error->setOn(F("DIV BY 0"));
        } else {
          rc = ra / rb;
        }
        update = true;
        break;
      case I_INC_A:
        ra++;
        update = true;
        break;
      case I_INC_B:
        rb++;
        update = true;
        break;
      case I_INC_C:
        rc++;
        update = true;
        break;
      case I_DEC_A:
        ra--;
        update = true;
        break;
      case I_DEC_B:
        rb--;
        update = true;
        break;
      case I_DEC_C:
        rc--;
        update = true;
        break;
      case I_INV_A:
        ra = -ra;
        update = true;
        break;
      case I_INV_B:
        rb = -rb;
        update = true;
        break;
      case I_INV_C:
        rc = -rc;
        update = true;
        break;
      case I_SWITCH_C_A:
        switchValues( &rc, &ra);
        update = true;
        break;
      case I_SWITCH_C_B:
        switchValues( &rc, &rb);
        update = true;
        break;
      case I_SWITCH_A_B:
        switchValues( &ra, &rb);
        update = true;
        break;
      case I_JEQ_AB:
        if (ra == rb) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JEQ_AB %s %s" CR), (ra == rb ? "jmp" : "nop"), X4(indexExec+1)); 
        update = true;
        break;
      case I_JNEQ_AB:
        if (ra != rb) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JNEQ_AB %s %s" CR), (ra != rb ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JEQ_AC:
        if (ra == rc) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JEQ_AC %s %s" CR), (ra == rc ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JNEQ_AC:
        if (ra != rc) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JNEQ_AC %s %s" CR), (ra != rc ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JEQ_BC:
        if (rb == rc) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JEQ_BC %s %s" CR), (rb == rc ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JNEQ_BC:
        if (rb != rc) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JNEQ_BC %s %s" CR), (rb != rc ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JUMP_RB:
        indexExec = instructions->getLabelAddress(rb);
        Log.trace(F("  --> JUMP_RB %s" CR), X4(indexExec+1));
        update = true;
        break;
      case I_JUMP_ADR_RB:
        indexExec = rb;
        Log.trace(F("  --> JUMP_ADR_RB %s" CR), X4(indexExec+1));
        update = true;
        break;
      case I_JUMP_SUB:
        stack->subPush(indexExec);
        indexExec = instructions->getLabelAddress(instr.parameter);
        update = true;
        break;
      case I_JUMP_SUB_AZ:
        if (ra <= 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_JUMP_SUB_BZ:
        if (rb <= 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_JUMP_SUB_CZ:
        if (rc <= 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_JUMP_SUB_ANZ:
        if (ra != 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_JUMP_SUB_BNZ:
        if (rb != 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_JUMP_SUB_CNZ:
        if (rc != 0) {
          stack->subPush(indexExec);
          indexExec = instructions->getLabelAddress(instr.parameter);
          update = true;
        }
        break;
      case I_RETURN:
        indexExec = stack->subPop();
        update = true;
        break;
      case I_LSHIFT:
        rc = ra << rb;
        update = true;
        break;
      case I_RSHIFT:
        rc = ra >> rb;
        update = true;
        break;
      case I_AND:
        rc = ra & rb;
        update = true;
        break;
      case I_OR:
        rc = ra | rb;
        update = true;
        break;
      case I_JUMP:
        indexExec = instructions->getLabelAddress(instr.parameter);
        Log.trace(F("  --> JMP %s" CR), X4(indexExec+1));
        update = true;
        break;
      case I_JAZ:
        if (ra <= 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JAZ %s %s" CR), (ra <= 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JBZ:
        if (rb <= 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JBZ %s %s" CR), (rb <= 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JCZ:
        if (rc <= 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JCZ %s %s" CR), (rc <= 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JANZ:
        if (ra != 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JANZ %s %s" CR), (ra != 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JBNZ:
        if (rb != 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JBNZ %s %s" CR), (rb != 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_JCNZ:
        if (rc != 0) {
          indexExec = instructions->getLabelAddress(instr.parameter);
        }
        Log.trace(F("  --> JCZ %s %s" CR), (rc != 0 ? "jmp" : "nop"), X4(indexExec+1));
        update = true;
        break;
      case I_LABEL:
        update = true;
        break;
      case I_STOP:
        Log.trace(F("  --> STOP" CR));
        isRunning = false;
        update = true;
        break;
      case I_INPUTA:
        if (screenIsOn) {
          ra = getInput("A");
        } 
        break;
      case I_INPUTB:
        if (screenIsOn) {
          rb = getInput("B");
        }
        break;
      case I_INPUTC:
        if (screenIsOn) {
          rc = getInput("C");
        }
        break;
      case I_INPUTM:
        if (screenIsOn) {
          sprintf(temp, "MEM(%x)", instr.parameter);
          ram->setAt(instr.parameter, getInput(temp));
        }
        break;
      case I_PRINTA:
        if (screenIsOn) {
          screen->printValue("A", ra);
        }
        break;
      case I_PRINTB:
        if (screenIsOn) {
          screen->printValue("B", rb);
        }
        break;
      case I_PRINTC:
        if (screenIsOn) {
          screen->printValue("C", rc);
        }
        break;
      case I_PRINTM:
        if (screenIsOn) {
          sprintf(temp, "MEM(%x)", ram->getAt(instr.parameter));
          screen->printValue(temp, rc);
        }
        break;
      case I_PRINT_CHAR:
        if (screenIsOn) {
          screen->printC(instr.parameter, true);
        }
        break; 
      case I_PRINT_CHAR_A:
        if (screenIsOn) {
          screen->printC((char)ra, true);
        }
        break; 
      case I_PRINT_CHAR_M:
        if (screenIsOn) {
          screen->printC((char)(ram->getAt(instr.parameter)), true);
        }
        break; 
      case I_SCREEN_ON:
        setScreenMode(true);
        break;
      case I_SCREEN_OFF:
        if (screenIsOn) {
          setScreenMode(false);
          update = true;
        }
        break;
      case I_SCREEN_NL:
        if (screenIsOn) {
          screen->setNewLine();
        }
        break;
      case I_SCREEN_CLEAR:
        if (screenIsOn) {
          screen->clear();
        }
        break;
      case I_DT_PRINT_DATE:
        if (screenIsOn) {
          tmElements_t tm = clock->get();
          if (!error->isOn()) {
            sprintf(temp, "%02d/%02d/%04d", tm.Day, tm.Month, tmYearToCalendar(tm.Year));
            screen->print(temp);
          }
        }
        break;
      case I_DT_PRINT_TIME:
        if (screenIsOn) {
          tmElements_t tm = clock->get();
          if (!error->isOn()) {
            sprintf(temp, "%02d:%02d:%02d", tm.Hour, tm.Minute, tm.Second);
            screen->print(temp);
          }
        }
        break;
      case I_STACK_PUSH_A:
        stack->push(ra);
        update = true;
        break;
      case I_STACK_PUSH_B:
        stack->push(rb);
        update = true;
        break;
      case I_STACK_PUSH_C:
        stack->push(rc);
        update = true;
        break;
      case I_STACK_PUSH_M:
        stack->push(ram->getAt(instr.parameter));
        update = true;
        break;
      case I_STACK_POP_A:
        ra = stack->pop();
        update = true;
        break;
      case I_STACK_POP_B:
        rb = stack->pop();
        update = true;
        break;
      case I_STACK_POP_C:
        rc = stack->pop();
        update = true;
        break;
      case I_STACK_POP_M:
        ram->setAt(instr.parameter, stack->pop());
        update = true;
        break;
      case I_STACK_INIT:
        stack->init();
        update = true;
        break;
      case I_MOVRR_AB:
        ram->setAt(ra, rb);
        Log.trace(F("  --> MOVRR_AB (%s) = %s" CR), X4(ra), X4(rb));
        update = true;
        break;
      case I_RANDOM:
        rc = random(0, instr.parameter);
        update = true;
        break;
      case I_RANDOM_A:
        rc = random(0, ra);
        update = true;
        break;
      case I_SLEEP:
        delay(instr.parameter);
        update = true;
        break;
      case I_SLEEP_A:
        delay(ra);
        update = true;
        break;
      case I_ARD_PIN_MODE_OUTPUT:
        pinMode(ra, OUTPUT);
        update = true;
        break;
      case I_ARD_PIN_MODE_INPUT:
        pinMode(ra, INPUT);
        update = true;
        break;
      case I_ARD_DREAD:
        rc = digitalRead(ra);
        update = true;
        break;
      case I_ARD_DWRITE:
        digitalWrite(ra, rb);
        update = true;
        break;
      case I_ARD_AREAD:
        rc = analogRead(ra);
        update = true;
        break;
      case I_ARD_AWRITE:
        analogWrite(ra, rb);
        update = true;
        break;
      case I_F_OPEN_R:
        if (rb >= FILE_NBR) { 
          error->setOn(F("File idx too big"));
        } else {
          sprintf(temp, "/DATA/%04X.dat", ra);
          files[rb] = SD.open(temp, FILE_READ);
          update = true;
        }
        break;
      case I_F_OPEN_W:
        if (rb >= FILE_NBR) { 
          error->setOn(F("File idx too big"));
        } else {
          sprintf(temp, "/DATA/%04X.dat", ra);
          files[rb] = SD.open(temp, FILE_WRITE);
          update = true;
        }
        break;
      case I_F_REMOVE:
        sprintf(temp, "/DATA/%04X", ra);
        SD.remove(temp);
        update = true;
        break;
      case I_F_CLOSE:
        if (rb >= FILE_NBR) { 
          error->setOn(F("File idx too big"));
        } else {
          files[rb].close();
          update = true;
        }
        break;
      case I_F_READ:
        ra = files[rb].read();
        update = true;
        break;
      case I_F_WRITE:
        files[rb].write(ra);
        update = true;
        break;
      case I_F_AVAILABLE:
        rc = files[rb].available();
        update = true;
        break;
    }
    if (error->isOn()) 
      isRunning = false;         
    else {
      indexExec++;
    }
    updateDisplay();
    
    if (indexExec > instructions->findLastInstrIndex()) {
      Log.notice(F("STOP" CR));
      isRunning = false;
    }
  }

  void reset() {
    Log.notice(F("RESET PROGRAM" CR));
    setScreenMode(false);
    indexExec = 0;
    initRegisters();
    error->init();
    instructions->compile();
    eraseRam();
  }

  void eraseProgram() { instructions->erase(); }
  void eraseRam() { ram->erase(); }
  inline void setInstr(Instr instr, unsigned short index) { instructions->set(instr, index); }
  inline Instr getInstr(unsigned short index) { return instructions->get(index); }
  inline void deleteInstr(unsigned short index) { instructions->deleteInstr(index); }
  inline void insertInstr(unsigned short index) { instructions->insertInstr(index); }
  inline unsigned lastInstrIndex() { return instructions->findLastInstrIndex(); }
  
  void printToSerial() {
    Log.notice(F("ADDRESS" CR));
    Log.notice(F("  ram free addresses               : %s - %s" CR), X4(0), X4(stack->getBottomAddress()-1));
    Log.notice(F("  stack addresses                  : %s - %s" CR), X4(stack->getBottomAddress()), X4(stack->getTopAddress()));  
    Log.notice(F("  stack index (size)               : %s (%s)" CR), X4(stack->getIndexAddress()), X4(stack->getIndex()));
    Log.notice(F("  sub stack index (value)          : %s (%s)" CR), X4(stack->subGetIndexAddress()), X4(stack->subGetIndex()));
    Log.notice(F("  keypressed address (value)       : %s (%s)" CR), X4(KEY_ADDRESS), X4(ram->getAt(KEY_ADDRESS)));
    Log.notice(F("  screen addresses                 : %s - %s" CR), X4(screen->getFirstAddress()), X4(screen->getLastAddress()));
    Log.notice(F("  screen cursor addresses Line Col : %s , %s" CR), X4(SCREEN_RAM_Y), X4(SCREEN_RAM_X));
    Log.notice(F("  last ram address                 : %s" CR), X4(RAM_SIZE-1));    
    Log.notice(F("  ram in eeprom addresses          : %s - %s" CR), X4(RAM_SIZE), X4(RAM_SIZE + RAM_IN_EPROM_SIZE - 1));    
    Log.notice(F("RAM" CR));
    for (unsigned i = 0; i < RAM_SIZE; i += 16) {
      Log.notice(F("  %s:%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s" CR),
                             X4(i), GETRAMX(i), GETRAMX(i+1), GETRAMX(i+2), GETRAMX(i+3), GETRAMX(i+4), GETRAMX(i+5), 
                             GETRAMX(i+6), GETRAMX(i+7), GETRAMX(i+8), GETRAMX(i+9), GETRAMX(i+10), GETRAMX(i+11), 
                             GETRAMX(i+12), GETRAMX(i+13), GETRAMX(i+14), GETRAMX(i+15));  
    }
    Log.notice(F("PROGRAM LISTING STARTING" CR));
    Log.notice(F("  LINE INST PARA : INSTR NAME" CR));
    unsigned nopNbr = 0; // Stop the listing after 2 NOP
    for (unsigned i = 0; i < INSTR_SIZE && nopNbr < 2; i++) {
      Instr instr = getInstr(i); 
      if (instr.action == I_NOP)
        nopNbr++;
      else 
        nopNbr = 0;
      if (nopNbr == 2)
        break;
      if(instr.withParam())
        Log.notice(F("  %s %s %s : %s" CR), X4(i+1), X4(instr.action), X4(instr.parameter), instr.getActionAsStr().c_str());
      else
        Log.notice(F("  %s %s .... : %s" CR), X4(i+1), X4(instr.action), instr.getActionAsStr().c_str());
    }
    Log.notice(F("PROGRAM LISTING END" CR));
    #ifdef MEMORY_FREE_H
      Log.notice(F("Free memory in Arduino : %u" CR), freeMemory());
    #endif
  }

private:
  void setRamWithDate(unsigned address) {
    tmElements_t tm = clock->get();
    if (error->isOn())
      return;
    ram->setAt(address, tmYearToCalendar(tm.Year));
    ram->setAt(address+1, tm.Month);
    ram->setAt(address+2, tm.Day);
  }

  void setRamWithTime(unsigned address) {
    tmElements_t tm = clock->get();
    if (error->isOn())
      return;
    ram->setAt(address, tm.Hour);
    ram->setAt(address+1, tm.Minute);
    ram->setAt(address+2, tm.Second);
  }

  void displayScreen() {
    screen->switchOn();
    screen->updateScreen();
    if (stepByStep) {
      char k = NO_KEY;
      while(k == NO_KEY)
        k = keypad.getKey();
      
      screen->switchOff();
      updateDisplay(); 
    }   
  }
  
  void updateDisplay() {
    if (screen->isOn() && error->isOff()  && !stepByStep)
      return; 
    
    lcd->initDisplay();

    // LINE 0
    sprintf(temp, "%s A:%03X B:%03X C:%03X ", getRunState().c_str(), ra, rb, rc);
    lcd->setLine(0, String(temp));

    if (error->isOn()) {
      lcd->setLine(1, error->getReason());
      Log.error(F("%s" CR), error->getReason().c_str());
   } else if (indexExec > 0) {
      displayLine(indexExec-1, 1, false);
    }
    for (unsigned short i = 0; i < 2 && i+indexExec < INSTR_SIZE; i++) {
      displayLine(indexExec+i, i+2, (i == 0));
    }
    lcd->display();
  }
  
  unsigned getInput(String name) {
    Input input(screen, name);
    return input.get();
  }
     
  String getRunState() {
    if (error->isOn()) 
      return "E";
    if (stepByStep) 
      return "S";
    if (isRunning) 
      return "R";
    return "O";
  }

  void setScreenMode(bool mode) {
    screenIsOn = mode;
    if (screenIsOn) {
      screen->switchOn();
    } else {
      screen->switchOff();
    }
  }

  void switchValues(unsigned* a, unsigned* b) {
    unsigned temp = *a;
    *a = *b;
    *b = temp;  
  }
  
  void displayLine(unsigned short index, unsigned short displayLine, bool current) {
    Instr instr = getInstr(index);
    if (instr.withParam())
      sprintf(temp, "%s%03X %02X %04X:%s", (current ? ">" : " "),
        index+1, instr.action, instr.parameter, instr.getActionAsStr().c_str());
    else
      sprintf(temp, "%s%03X %02X ....:%s", (current ? ">" : " "),
        index+1, instr.action, instr.getActionAsStr().c_str());
    lcd->setLine(displayLine, String(temp));
  }

  void initRegisters() {
    ra = 0;
    rb = 0;
    rc = 0; 
  }

  bool isRunning;
  bool stepByStep;
  byte keyPressed;
  bool screenIsOn; // Save the screen state, for switching edit
  unsigned short ra, rb, rc; // Registers  
  unsigned short indexExec;
  Ram* ram;
  Screen* screen;
  Stack* stack;
  Lcd* lcd;
  Error* error;
  Clock* clock;
  Instructions* instructions;
  File files[FILE_NBR];
};

#endif
