#pragma once
#include "Arduino.h"
#define WL_CONNECTED 3
#define WL_DISCONNECTED 6
#define WIFI_STA 1
#define WIFI_AP_STA 3
#define WIFI_SCAN_RUNNING (-1)
#define WIFI_SCAN_FAILED (-2)
#define WIFI_AUTH_OPEN 0
struct IPAddr { String toString() const { return String("192.168.1.50"); } };
struct WiFiM {
  int st = WL_CONNECTED;
  int status() { return st; }
  int RSSI(int = -1) { return -55; }
  void setHostname(const char *) {} void mode(int) {} void setAutoReconnect(bool) {}
  void begin(const char *, const char *) {} void disconnect(bool = false) {}
  String SSID(int = -1) { return String("NhaToi"); }
  IPAddr localIP() { return {}; } IPAddr softAPIP() { return {}; }
  int softAPgetStationNum() { return 0; }
  bool softAP(const char *, const char *) { return true; } void softAPdisconnect(bool) {}
  int scanNetworks(bool = false, bool = false) { return WIFI_SCAN_RUNNING; } int scanComplete() { return 0; } void scanDelete() {}
  int encryptionType(int) { return WIFI_AUTH_OPEN; }
};
extern WiFiM WiFi;
