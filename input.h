#ifndef INPUT_H
#define INPUT_H

#include "key.h"
#include "log.h"
#include "screen.h"

class Input {
public:
  Input(Screen* s, String n) {
    screen = s;
    name = n;
  }
  
  int get() {
    char temp[21];
    sprintf(temp, "%s : ", name.c_str());
    screen->print(temp);
    String sinput = "";
    int index = 0;
    bool inputMode = true;
    screen->setCursor();
    while (inputMode) {
      char key = keypad.getKey();
      if (key == NO_KEY) {
        screen->refreshCursor();
        continue;
      }
      if (key == KEY_ENTER) {
        inputMode = false;
        break;
      } 
      if (key >= KEY_HEX_0 && key <= KEY_HEX_9) {
        sinput+= key;
        screen->printC(key, true);
        if (index < 4) {
          index++;
        }
      } 
      if (key == KEY_LEFT && index > 0) {
        index--;
        sinput = sinput.substring(0, sinput.length()-1);
        screen->backX();
      }
      screen->setCursor();
    }
    unsigned ret = sinput.toInt();
    return ret;
  }
private:
  Screen* screen;
  String name;
};

#endif