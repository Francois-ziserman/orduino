#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SD.h>
#include <SPI.h>
#include "program.h"
#include "lcd.h"
#include "clock.h"

#define PIN_CS 53

#define CURRENT_PRGM F("PRGM/CURRENT.CAR")

// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
  tmElements_t now;
  if (!RTC.read(now)) {
      Log.error(F("Clock not found" CR));   
  }
  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(tmYearToCalendar(now.Year), now.Month, now.Day);
  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.Hour, now.Minute, now.Second);
}

class FileSystem {
public:
  FileSystem(Lcd* l, Clock* c, Program* p) {
    currentIndex = 0;
    lcd = l;
    clock = c;
    program = p;    

    pinMode(PIN_CS, OUTPUT);
    pinMode(10, OUTPUT);
    init();
  }

  void saveCurrent() {
    if (!isOk)
      return;
    Log.notice(F("Save current program starting..." CR));
    SD.remove(CURRENT_PRGM);

    File file = SD.open(CURRENT_PRGM, FILE_WRITE);
    unsigned last = program->lastInstrIndex()+1;
    for (unsigned index = 0; index <= last; index++) {
      Instr instr = program->getInstr(index);
      file.println(instr.getForFile());
      Log.verbose(F("  %i : %s" CR), index, instr.getForFile().c_str());
    }
    file.close();
    Log.notice(F("Save current program end : %i lines" CR), last);
  }

  void enter() {
    Log.notice(F("ENTER IN FILE MODE..." CR));
    init();
    displayFiles();
    while(true) {
      char key = keypad.getKey();
      lcd->refreshCursor();
      switch (key) {
        case KEY_LINE_UP:
          lineUp();
          break;
        case KEY_LINE_DOWN:
          lineDown();
          break;
        case KEY_DELETE_LINE:
          del();
          displayFiles();
          break;
        case KEY_FILE_COPY:
          copy();
          displayFiles();
          break;
        case KEY_FILE_RENAME:
          rename();
          displayFiles();
          break;
        case KEY_DUMP_PRG_TO_SERIAL:
          dumpToSerial();
          break;
        case KEY_FILE_LOAD:
          load();
          return;
        case KEY_SWITCH_MODE:
        case KEY_SWITCH_EDIT_FILE:
          return;
      }

    }
  }

private:
  void init() {
    if (!SD.begin(10)) {
      Log.error(F("SD card initialization failed" CR));
      isOk = false;
      return;
    }
    Log.verbose(F("SD is ready to use" CR));
    SdFile::dateTimeCallback(dateTime);

    if (!SD.exists(F("PRGM"))) {
      Log.notice(F("SD : PRGM default dir doesn't exist --> create it" CR));
      SD.mkdir(F("PRGM"));
    }    
  }

  void lineUp() {
    if (currentIndex <= 0)
      return;
    currentIndex--;
    displayFiles();    
  }

  void lineDown() { 
    if (currentIndex+1 >= getProgramNbr())
      return;
    currentIndex++;
    displayFiles();
  }

  void del() {
    File current = getProgramAt(currentIndex);
    if (String(current.name()) == String(F("CURRENT.CAR"))) {
      lcd->displaySingleMessageWithValidate(F("ERROR"), F("can't delete CURRENT"));
      current.close();
      return;
    }
    sprintf(temp, "/PRGM/%s", current.name());
    current.close();
    String name(temp);
    Log.trace(F("delete(%s)" CR), name.c_str());
    if (!SD.remove(name)) {
      lcd->displaySingleMessageWithValidate(F("ERROR"), F("can't delete file"));
    }
  }

  void copy() {
    File current = getProgramAt(currentIndex);
    String pref = current.name();
    pref = pref.substring(0, 2);
    unsigned i = 1;
    String name;
    while(true) {
      sprintf(temp, "/PRGM/%s%02i.CAR", pref.c_str(), i++);
      name = String(temp);
      if (!SD.exists(name)) {
        break;
      }
    }
    Log.trace(F("Copy '%s' -> '%s'" CR), current.name(), name.c_str());
    lcd->displaySingleMessage(F("COPYING"), F("Copy in process..."));
    File target = SD.open(name, FILE_WRITE);
    while(current.available()) {
      target.write(current.read());
    }

    target.close();
    current.close();
  }

  void rename() {
    File current = getProgramAt(currentIndex);
    if (String(current.name()) == String(F("CURRENT.CAR"))) {
      lcd->displaySingleMessageWithValidate(F("ERROR"), F("can't rename CURRENT"));
      current.close();
      return;
    }
    sprintf(temp, "RENAME:%s", current.name());
    String name = getFileName(String(temp), F("New   :"));
    if (name.length() > 0) {
      name += ".CAR";
      File f = getProgramByName(name);
      if (f) {
        lcd->displaySingleMessageWithValidate(F("ERROR"), F("rename already existss"));
        current.close();
        f.close();
        return;  
      }
      File target = SD.open("/PRGM/" + name, FILE_WRITE);
      lcd->displaySingleMessage(F("RENAMING"), F("Rename in process..."));
      while(current.available()) {
        target.write(current.read());
      }
      name = current.name();
      current.close();
      target.close();
      SD.remove("/PRGM/" + name);
    }
  }

  void load() { 
    read(currentIndex); 
  }
  
  void dumpToSerial() {
    File file = getProgramAt(currentIndex);
    unsigned index = 0;
    Log.notice(F("PROGRAM LISTING : %s" CR), file.name());
    while (file.available()) {
      String line;
      String comment; 
      readLine(file, line, comment);
      if (line.length() < 4 && comment.length() < 2)
        continue;
      Instr instr = createInstrFromFile(line);
      Log.trace(F("  %i : %s %s" CR), index, (instr.action == 0 ? "" : instr.getForFile().c_str()), comment.c_str());
      index++;
    }
    file.close();
    Log.notice(F("PROGRAM LISTING COMPLETE" CR));
  }

  void displayFiles() {
    Log.verbose(F("displayFiles. currentIndex:%i" CR), currentIndex);
    lcd->initDisplay();
    lcd->setCursor(0, 0);
    for (unsigned i = 0; i < 4; i++) {
      File current = getProgramAt(currentIndex+i);
      if (!current) {
        break;
      }
      displayOneFile(current, i);
      current.close();
    }
    lcd->display();
  }

  void displayOneFile(File file, unsigned index) {
    Log.verbose(F("displayOneFile(%s, %i)" CR), file.name(), index);
    sprintf(temp, "%s %s", (index == 0 ? "#" : " "), file.name());
    lcd->setLine(index, String(temp));
  }

  int getProgramNbr() {
    int index = 0;
    while(true) {
      File file = getProgramAt(index);
      if (!file)
        break;
      index++;
      file.close();
    }
    Log.verbose(F("getProgramNbr() -> %i" CR), index);
    return index;
  }

  File getProgramAt(unsigned index) {
    File root = SD.open(F("/PRGM"));
    if (!root) {
      Log.error(F("GetProgramAt(%i) -> error in root" CR), index);
      return root;
    }
    unsigned i = 0;
    File file;
    while(true) {
      file =  root.openNextFile();
      if (!file) {
        Log.verbose(F("GetProgramAt(%i) -> Not found" CR), index);
        root.close();
        return file;
      }
      if (isValidProgramName(file.name())) {
        if (i == index) {
          Log.verbose(F("GetProgramAt(%i) -> '%s'" CR), index, file.name());
          root.close();
          return file;
        } else {
          i++;
        }
      }
      file.close();
    }
  }

  File getProgramByName(String name) {
    File root = SD.open(F("/PRGM"));
    if (!root) {
      Log.error(F("GetProgramByName(%s) -> error in root" CR), name.c_str());
      return root;
    }
    unsigned i = 0;
    File file;
    while(true) {
      file =  root.openNextFile();
      if (!file) {
        Log.verbose(F("GetProgramByName(%s) -> Not found" CR), name.c_str());
        root.close();
        return file;
      }
      if (name.equals(file.name())) {
        Log.verbose(F("GetProgramByName(%s) -> found" CR), name.c_str());
        root.close();
        return file;
      }
      file.close();
    }
  }

  bool isValidProgramName(String name) {
    if (name.indexOf(".CAR") < 0) {
      return false;
    }
    for (int i = 0; i < name.length()-4; i++) {
      char c = name.charAt(i);
      if (c >= 'a' && c <= 'z')
        continue;
      if (c >= 'A' && c <= 'Z')
        continue;
      if (c >= '0' && c <= '9')
        continue;
      if (c == '_' || c == '-')
        continue;
      Log.verbose(F("isValidProgramName(%s) -> false" CR), name.c_str());
      return false;
    }
    Log.verbose(F("isValidProgramName(%s) -> true" CR), name.c_str());
    return true;
  }

  void read(unsigned fileIndex) {
    if (!isOk)
      return;
    File file = getProgramAt(fileIndex);
    if (!file) {
      Log.error(F("File doesn't exist : %i" CR), fileIndex);
      return;
    }
    Log.trace(F("Reading file '%s' starting..." CR), file.name());

    lcd->displaySingleMessage(F("READING"), F("Reading in process..."));

    program->eraseProgram();
    unsigned index = 0;
    String line;
    String comment;
    while (file.available()) {
      readLine(file, line, comment);
      if (line.length() < 4)
        continue;
      Instr instr = createInstrFromFile(line);
      Log.trace(F("  %i : %s %s -> %s" CR), index, line.c_str(), comment.c_str(), instr.getForFile().c_str());
      program->setInstr(instr, index);
      index++;
    }
    file.close();
    lcd->displaySingleMessageWithValidate(F("READING"), F("Reading complete"));
    Log.trace(F("Reading file '%s' End" CR), file.name());
  }

  void readLine(File file, String& line, String& comment) {
    line = "";
    comment = "";
    uint8_t i = 0;
    bool inComment = false;
    while (file.available() && i < 14) {
      char c = file.read();
      if (c == '\n')
        break;
      if (c == '#') 
          inComment = true;
      if (inComment)
        comment += c;
      else
        line += c;
    }
    Log.verbose(F("  Read line '%s'" CR), line.c_str());
  }

  String getFileName(String title, String message) {
    lcd->initDisplay();
    lcd->setCursor(0, 0);
    lcd->setLine(0, title);
    lcd->setLine(1, message);
    lcd->display();
    String ret = "";
    String sinput = "";
    unsigned pos = message.length();
    unsigned i;
    lcd->setCursor(pos, 1);
    while (true) {
      lcd->refreshCursor();
      char k = keypad.getKey();
      if (k == KEY_ENTER) {
        return ret;
      }
      if (k == KEY_SWITCH_MODE || k == KEY_RUN_STOP) {
        return "";
      }
      if (k == KEY_LEFT && ret.length() > 0) {
        ret = ret.substring(0, ret.length()-1);
        pos--;
        lcd->setLine(1, message + ret);
        lcd->display();
        lcd->setCursor(pos, 1);
      } else if (k >= KEY_HEX_0 && k <= KEY_HEX_F) {
        sinput+= k;
        Log.verbose(F("input: '%s'" CR), sinput.c_str());
        if (sinput.length() == 2) {
          char c = getCharToInt(sinput[0]) * 16 + getCharToInt(sinput[1]);
          if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c >= '0' && c <= '9' || c == '_' || c == '-') {
            ret += c;
            Log.verbose(F("add char '%c' --> '%s'" CR), c, ret.c_str());
            lcd->setLine(1, message + ret);
            lcd->display();
            pos++;
            lcd->setCursor(pos, 1);
            sinput = "";
          } else {
            sinput = "";
          }
        }
      }
    }
    return ret;
  }

  bool isOk = true;
  Lcd* lcd;
  Clock* clock;
  Program* program;
  int currentIndex;
};


#endif