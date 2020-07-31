#ifndef ERROR_H
#define ERROR_H

#include "log.h"

class Error {
public:
  Error() {
    init();
  }

  void setOn(String reason, unsigned value) {
    on = true;
    sprintf(temp, reason.c_str(), value);
    setOn(temp);
  }

  void setOn(String r) {
    on = true;
    reason = r;
    while (reason.length() < 20)
      reason += ' ';
  }

  void init() {
    on = false;
    reason = "";
  }

  bool isOn() { return on; }
  bool isOff() { return !on; }

  String getReason() { return reason; }
  
private:
  bool on = false;
  String reason;
};

#endif
