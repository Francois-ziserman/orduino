#ifndef ENGINE_H
#define ENGINE_H

#include "lcd.h"
#include "error.h"
#include "program.h"
#include "editor.h"

class Engine {
public:
  Engine() {
    runMode = false;
    error = new Error();
    lcd = new Lcd();
    program = new Program(error, lcd);
    editor = new Editor(error, lcd, program);
    editor->updateDisplay();
  }

  void loop() {
    receiveFromSerial();
    char key = keypad.getKey();
    if (key == NO_KEY) {
      if (runMode) {
        program->doOneStep();
      } else {
        lcd->refreshCursor();
      }
    } else { // key != NO_KEY
      if (key == KEY_SWITCH_MODE) {
        runMode = !runMode;
        if (runMode) {
          Log.notice("SWITCH TO EXEC MODE" CR);
          program->startRunMode();
        } else {
          Log.notice("SWITCH TO EDIT MODE" CR);
          editor->updateDisplay();
        }
      } else if (runMode) {
        program->oneStep(key);
      } else {
        editor->edit(key);
      }
    }
  };

private:
  void receiveFromSerial() {
    if (Serial.available() == 0)
      return;
    Log.notice("Start receiving" CR);
    byte incoming[6] = {0, 0, 0, 0, 0, 0};
    byte col = 0;
    while (Serial.available() > 0) {
      char incomingChar = Serial.read();
      if (incomingChar == 0xA) {
        editor->setFromSerial(incoming);
          for (byte i = 0; i < 6; i++) incoming[i] = 0;
          col = 0;
      }
      if ((incomingChar >= 0x30 && incomingChar <= 0x39) || (incomingChar >= 0x41 && incomingChar <= 0x46)) {
        incoming[col] = incomingChar >= 0x30 && incomingChar <= 0x39 ? incomingChar - 0x30 : incomingChar - 0x41 + 0x10;
        Log.notice("  received %s in col %i" CR, X4(incoming[col]), col);
        col++;
        if (col == 6) {
          editor->setFromSerial(incoming);
          for (byte i = 0; i < 6; i++) incoming[i] = 0;
          col = 0;
        }
      }      
    }
  }
  bool runMode; // edit or run
  Error* error;
  Program* program;
  Editor* editor;
  Lcd* lcd;
};

#endif
