#include "Arduino.h"
#include "Adafruit_ST7789.h"
#include "WiFi.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "SPI.h"
unsigned long g_ms; int g_pin[64], g_out[64], g_adc[64], g_mv[64]; SerialMock Serial; SPIClass SPI; MDNSM MDNS; WiFiM WiFi;
std::vector<Rect> g_fills; std::vector<std::string> g_prints; int g_blits;
#include SKETCH
int main() {
  for (int p : {40, 41, 42}) g_pin[p] = HIGH;
  g_adc[SOIL_PIN] = 2370; g_mv[BAT_PIN] = 1900;
  setup(); webBegin();
  pumpLockout = true; temperature = -12.3f; humidity = 100; soilPct = 100; batPct = 100; chgState = 2;
  web.req(HTTP_GET, "/api/status");
  printf("JSONLEN=%zu\nJSON=%s\n", web.lastBody.size(), web.lastBody.c_str());
}
