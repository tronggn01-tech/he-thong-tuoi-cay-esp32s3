#include "Arduino.h"
#include "Adafruit_ST7789.h"
#include "WiFi.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "SPI.h"
unsigned long g_ms; int g_pin[64], g_out[64], g_adc[64], g_mv[64]; SerialMock Serial; SPIClass SPI; MDNSM MDNS; WiFiM WiFi;
std::vector<Rect> g_fills; std::vector<std::string> g_prints; int g_blits;
#include SKETCH
static int fails = 0, n = 0;
#define CHECK(c, msg) do { n++; if (!(c)) { fails++; printf("FAIL %d: %s (t=%lus soil=%d ok=%d pump=%d auto=%d lock=%d)\n", n, msg, g_ms/1000, soilPct, soilOk, pumpOn, autoMode, pumpLockout); } else printf("ok   %d: %s\n", n, msg); } while (0)
static void run(unsigned long total, unsigned long dt = 100) { for (unsigned long t = 0; t < total; t += dt) { g_ms += dt; loop(); } }
static void soil(int pct) { g_adc[SOIL_PIN] = (int)(2370 - pct * 12.5); }
static void press(int pin, unsigned long hold = 150) { g_pin[pin] = LOW; run(hold, 50); g_pin[pin] = HIGH; run(150, 50); }
int main() {
  for (int p : {40, 41, 42}) g_pin[p] = HIGH; g_mv[BAT_PIN] = 1900;
  soil(50); setup();
  CHECK(soilPct == 50 && !pumpOn, "1. AUTO, 50 %, bom tat -> giu tat");
  run(7000); soil(34); run(1000);
  CHECK(soilPct == 34 && pumpOn && g_out[RELAY_PIN] == HIGH, "2. AUTO, xuong 34 % -> bat bom, relay dong");
  run(6000); soil(40); run(1000);
  CHECK(pumpOn, "3. bom chay, 40 % -> van chay (vung tre)");
  soil(46); run(1000);
  CHECK(!pumpOn && g_out[RELAY_PIN] == LOW, "4. bom chay, 46 % -> tat bom");
  run(6000); soil(34); run(1000);                       // bat lai
  run(6000);
  g_adc[SOIL_PIN] = 0; run(1700);
  CHECK(!soilOk && !pumpOn, "5. mat tin hieu cam bien -> tat bom, '--%' va 'Loi'");
  soil(29); run(7000);
  CHECK(soilOk && soilPct == 29 && pumpOn, "6. cam bien hoat dong lai (29 %) -> bat bom");
  press(MODE_PIN);
  CHECK(!autoMode && !pumpOn, "7. AUTO + bom chay, bam MODE -> THU CONG, bom tu tat");
  soil(80); press(PUMPBTN_PIN);
  CHECK(pumpOn, "8a. THU CONG, bam PUMP -> bom bat (do am 80 %)");
  press(PUMPBTN_PIN);
  CHECK(!pumpOn, "8b. bam PUMP lan 2 -> bom tat");
  soil(34); press(MODE_PIN); run(300);
  CHECK(autoMode && pumpOn, "9. chuyen lai AUTO khi 34 % -> bat bom ngay");
  press(PUMPBTN_PIN);
  CHECK(autoMode && pumpOn, "10. AUTO, bam PUMP -> bo qua, bom giu nguyen");
  // --- 4 tinh huong moi (khoa bom)
  soil(20); run(PUMP_MAX_MS + 3000);
  CHECK(!pumpOn && pumpLockout, "11. bom chay lien tuc 60 s, do am chua len -> tat bom + khoa");
  run(10000);
  CHECK(!pumpOn && pumpLockout, "12. dang khoa, AUTO dat van kho -> khong bat lai");
  g_pin[PUMPBTN_PIN] = LOW; run(2300, 50); g_pin[PUMPBTN_PIN] = HIGH; run(300, 50);
  CHECK(!pumpLockout, "13. giu PUMP 2 s -> mo khoa");
  run(1500);
  CHECK(pumpOn, "13b. sau mo khoa AUTO xet lai do am -> bat bom");
  press(MODE_PIN); press(PUMPBTN_PIN);
  CHECK(!autoMode && pumpOn, "14a. THU CONG, bat bom bang tay");
  run(PUMP_MAX_MS + 2000);
  CHECK(!pumpOn && pumpLockout, "14b. THU CONG chay qua 60 s -> tat bom + khoa");
  printf("%d kiem tra, %d loi\n", n, fails);
  return fails ? 1 : 0;
}
