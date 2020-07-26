#ifndef EDITOR_H
#define EDITOR_H

#include "error.h"
#include "clock.h"

#define LINE_SIZE 6
#define PAGE 4
const String LINE_DEFAULT_VALUE = "000000";

class Editor {
public:
  Editor(Error* e, Lcd* l, Clock* c, FileSystem* fs, Program* p) {
    error = e;
    lcd = l;
    clock = c;
    filesystem = fs;
    program = p;    
    indexEdit = 1;
    indexColEdit = 0;
    currentInstrAsStr = LINE_DEFAULT_VALUE;
    currentInstrAsStr = program->getInstr(indexEdit).getAsString();
    modifLine = false;
  }

  void edit(unsigned char key) {
    bool change = false;
    switch (key) {
    case KEY_LINE_UP:
      if (indexEdit == 0)
        return;
      indexEdit--;
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      indexColEdit = 0;
      change = true;
      break;
    case KEY_PAGE_UP:
      if (indexEdit == 0)
        return;
      indexEdit = indexEdit < PAGE ? 0 : indexEdit - PAGE; 
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      indexColEdit = 0;
      change = true;
      break;
    case KEY_LINE_DOWN:
      if (indexEdit == INSTR_SIZE-1)
        return;
      indexEdit++;
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      indexColEdit = 0;
      change = true;
      break;
    case KEY_PAGE_DOWN:
      if (indexEdit == INSTR_SIZE-1)
        return;
      indexEdit = indexEdit+PAGE > INSTR_SIZE-1 ? INSTR_SIZE - 1 - PAGE : indexEdit + PAGE; 
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      indexColEdit = 0;
      change = true;
      break;
    case KEY_RIGHT:
      if (indexColEdit < LINE_SIZE)
        indexColEdit++;
      change = true;
      break;
    case KEY_LEFT:
      if (indexColEdit > 0)
        indexColEdit--;
      change = true;
      break;
    case KEY_ENTER:
      editValidate();
      change = true;
      break;
    case KEY_DELETE_LINE:
      program->deleteInstr(indexEdit);
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      change = true;
      break;
    case KEY_INSERT_LINE:
      program->insertInstr(indexEdit);
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      change = true;
      break;
    case KEY_ERASE_PROGRAM:
      program->eraseProgram();
      indexEdit = 0;
      currentInstrAsStr = program->getInstr(indexEdit).getAsString();
      modifLine = false;
      change = true;
    case KEY_ERASE_RAM:
       program->eraseRam();
       break;
    case KEY_DUMP_PRG_TO_SERIAL:
      dumpToSerial();
      break;
    case KEY_CLOCK_EDIT:
      clock->edit();
      change = true;
      break;
    }
    if (key >= KEY_HEX_0 && key <= KEY_HEX_F) {
      editInput(key);
      change = true;
    }
    if (change) {
      updateDisplay();
    } 
  }

  void setFromSerial(byte* input) {
    Log.notice(F("  Received : %s%s %s%s%s%s" CR), X4(input[0]), X4(input[1]), X4(input[2]), X4(input[3]), X4(input[4]), X4(input[5]));
  }
  void updateDisplay() {
    Instr instr;  

    // LINE 0
    if (indexEdit == 0) {
      lcd->initLine(0);
    } else {
      updateLine(indexEdit-1, 0);
    }

    // LINE 1 : edit line
    instr = Instr(currentInstrAsStr);
    if (instr.withParam())
      sprintf(lcd->line, "%03X%s%c%c %c%c%c%c: %s", indexEdit+1, (modifLine ? "#" : " "), currentInstrAsStr[0], currentInstrAsStr[1], 
        currentInstrAsStr[2], currentInstrAsStr[3], currentInstrAsStr[4], currentInstrAsStr[5], instr.getActionAsStr().c_str());
    else 
      sprintf(lcd->line, "%03X%s%c%c ....: %s", indexEdit+1, (modifLine ? "#" : " "), currentInstrAsStr[0], currentInstrAsStr[1], instr.getActionAsStr().c_str());
    
    lcd->setLine(1);

    // LINE 2 & 3
    if (indexEdit+1 < INSTR_SIZE) 
      updateLine(indexEdit+1, 2);
    else 
      lcd->initLine(2);

    if (indexEdit+2 < INSTR_SIZE) 
      updateLine(indexEdit+2, 3);
    else 
      lcd->initLine(3);

    lcd->display();
    lcd->setCursor(getYCursor(), 1);
  }

private:
  void updateLine(unsigned index, byte lineIndex) {
      Instr instr = program->getInstr(index); 
      if (instr.withParam())
        sprintf(lcd->line, "%03X %02X %04X: %s", index+1, instr.action, instr.parameter, instr.getActionAsStr().c_str());
      else 
        sprintf(lcd->line, "%03X %02X ....: %s", index+1, instr.action, instr.getActionAsStr().c_str());
      
      lcd->setLine(lineIndex);
  }
  void dumpToSerial() {
    program->printToSerial();
  }
  
  byte getYCursor() {
    byte ret = indexColEdit + 4;
    if (indexColEdit >= 2)
      ret++;
    return ret;
  }
  
  void editValidate() {
    Instr instr = Instr(currentInstrAsStr);
    program->setInstr(instr, indexEdit);
    indexColEdit = 0;
    indexEdit++;
    instr = program->getInstr(indexEdit);
    currentInstrAsStr = instr.getAsString();
    modifLine = false;    
  }
  
  void editInput(unsigned char key) {
    if (currentInstrAsStr[indexColEdit] != key) {
      currentInstrAsStr[indexColEdit] = key;
      modifLine = true;
    }
    if (indexColEdit < LINE_SIZE-1) {
      indexColEdit++;
    }
  }

  Program* program;
  Lcd* lcd;
  FileSystem* filesystem;
  Error* error;
  Clock* clock;
  boolean modifLine;
  unsigned short indexEdit;
  byte indexColEdit;
  String currentInstrAsStr;
};

#endif
