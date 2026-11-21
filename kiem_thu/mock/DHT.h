#pragma once
#include "Arduino.h"
#define DHT11 11
struct DHT { DHT(int, int) {} void begin() {} float readTemperature() { return 27.5f; } float readHumidity() { return 66.0f; } };
