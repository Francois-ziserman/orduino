#ifndef UTIL_H
#define UTIL_H

String rightTrim(String value) {
  for (int i = value.length()-1; i >=0; i--) {
    if (value[i] != ' ')
      return value.substring(0, i+1);
  }
  return value;
}

String rightAndMiddleTrim(String value) {
  for (int i = 0; i < value.length(); i++) {
    if (value[i] == ' ')
      return value.substring(0, i);
  }
  return value;
}

#endif