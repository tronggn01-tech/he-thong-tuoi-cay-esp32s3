#pragma once
#include "Arduino.h"
struct Preferences {
  bool begin(const char *, bool) { return true; } void end() {}
  bool isKey(const char *) { return false; }
  size_t getString(const char *, char *b, size_t) { b[0] = 0; return 0; }
  size_t putString(const char *, const char *) { return 0; }
};
