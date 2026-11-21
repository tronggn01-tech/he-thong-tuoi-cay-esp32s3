#pragma once
#include "Arduino.h"
struct MDNSM { bool begin(const char *) { return true; } void addService(const char *, const char *, int) {} };
extern MDNSM MDNS;
