#ifndef ENGINE_H
#define ENGINE_H

#include "lcd.h"
#include "error.h"
#include "clock.h"
#include "filesystem.h"
#include "program.h"
#include "editor.h"

class Engine {
public:
  Engine() {
    runMode = false;
    error = new Error();
    lcd = new Lcd();
    clock = new Clock(lcd, error);
    program = new Program(error, lcd, clock);
    filesystem = new FileSystem(lcd, clock, program);
    editor = new Editor(error, lcd, clock, filesystem, program);
    editor->updateDisplay();
  }

  void loop() {
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
          Log.notice(F("SWITCH TO EXEC MODE" CR));
          program->startRunMode();
        } else {
          Log.notice(F("SWITCH TO EDIT MODE" CR));
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
  
  bool runMode; // edit or run
  Error* error;
  Clock* clock;
  FileSystem* filesystem;
  Program* program;
  Editor* editor;
  Lcd* lcd;
};

#endif
