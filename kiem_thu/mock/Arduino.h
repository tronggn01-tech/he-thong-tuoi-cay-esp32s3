#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdarg>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <ctime>
#include <algorithm>
using std::isnan;
#define PROGMEM
#define pgm_read_byte(p) (*(const uint8_t *)(p))
#define pgm_read_word(p) (*(const uint16_t *)(p))
#define memcpy_P memcpy
#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define ARDUINO_ARCH_ESP32 1
#define constrain(a, lo, hi) ((a) < (lo) ? (lo) : ((a) > (hi) ? (hi) : (a)))
extern unsigned long g_ms;
extern int g_pin[64], g_out[64], g_adc[64], g_mv[64];
inline unsigned long millis() { return g_ms; }
inline void delay(unsigned long ms) { g_ms += ms; }
inline void delayMicroseconds(unsigned) {}
inline void pinMode(int, int) {}
inline int digitalRead(int p) { return g_pin[p]; }
inline void digitalWrite(int p, int v) { g_out[p] = v; }
inline int analogRead(int p) { return g_adc[p]; }
inline int analogReadMilliVolts(int p) { return g_mv[p]; }
inline void analogReadResolution(int) {}
inline long map(long x, long a, long b, long c, long d) { return (x - a) * (d - c) / (b - a) + c; }
#ifndef __GLIBC_USE_LIB_EXT2
#endif
#if !defined(__GLIBC__) || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 38)
inline size_t strlcpy(char *d, const char *s, size_t n) { size_t l = strlen(s); if (n) { size_t c = l >= n ? n - 1 : l; memcpy(d, s, c); d[c] = 0; } return l; }
#endif
class String {
 public:
  std::string s;
  String() {}
  String(const char *c) : s(c ? c : "") {}
  String(const std::string &c) : s(c) {}
  bool operator==(const char *c) const { return s == c; }
  bool operator!=(const char *c) const { return s != c; }
  const char *c_str() const { return s.c_str(); }
  size_t length() const { return s.size(); }
  operator const char *() const { return s.c_str(); }
};
struct SerialMock {
  void begin(long) {}
  void println(const char *t) { printf("[SER] %s\n", t); }
  void printf(const char *f, ...) __attribute__((format(printf, 2, 3))) {
    char b[512]; va_list a; va_start(a, f); vsnprintf(b, sizeof b, f, a); va_end(a);
    ::printf("[SER] %s", b);
  }
};
extern SerialMock Serial;
inline bool getLocalTime(struct tm *, int = 0) { return false; }
inline void configTime(long, int, const char *, const char *) {}
