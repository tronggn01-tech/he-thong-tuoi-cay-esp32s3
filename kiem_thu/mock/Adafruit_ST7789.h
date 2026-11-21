#pragma once
#include "Arduino.h"
#include "SPI.h"
struct Rect { int x, y, w, h; unsigned c; };
extern std::vector<Rect> g_fills; extern std::vector<std::string> g_prints; extern int g_blits;
class Adafruit_ST7789 {
 public:
  Adafruit_ST7789(SPIClass *, int, int, int) {}
  void init(int, int) {}
  void setSPISpeed(unsigned) {}
  void invertDisplay(bool) {}
  void setRotation(int) {}
  void sendCommand(uint8_t, const uint8_t *, int) {}
  void startWrite() {}
  void endWrite() {}
  void setAddrWindow(int, int, int, int) {}
  void writePixels(uint16_t *, uint32_t) { g_blits++; }
  void fillRect(int x, int y, int w, int h, unsigned c) { g_fills.push_back({x, y, w, h, c}); }
  void setTextWrap(bool) {}
  void setTextSize(int) {}
  void setTextColor(unsigned) {}
  void setCursor(int, int) {}
  void print(const char *t) { g_prints.push_back(t); }
};
