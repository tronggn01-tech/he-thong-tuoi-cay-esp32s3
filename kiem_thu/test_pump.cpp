#include "Arduino.h"
#include "Adafruit_ST7789.h"
#include "WiFi.h"
#include "WebServer.h"
#include "ESPmDNS.h"
#include "SPI.h"
unsigned long g_ms; int g_pin[64], g_out[64], g_adc[64], g_mv[64]; SerialMock Serial; SPIClass SPI; MDNSM MDNS; WiFiM WiFi;
std::vector<Rect> g_fills; std::vector<std::string> g_prints; int g_blits;
#include SKETCH

static int fails = 0, checks = 0;
#define CHECK(c, msg) do { checks++; if (!(c)) { fails++; printf("  FAIL: %s  (t=%lu s)\n", msg, g_ms / 1000); } else printf("  ok  : %s\n", msg); } while (0)
static void step(unsigned long ms) { g_ms += ms; loop(); }
static void run(unsigned long total, unsigned long dt = 100) { for (unsigned long t = 0; t < total; t += dt) step(dt); }
static int redBanners() { int n = 0; for (auto &r : g_fills) if (r.c == 0xDA89 && r.y == 0 && r.w == 320) n++; return n; }
static void soilDry() { g_adc[SOIL_PIN] = 2370; }   // 0 %
static void soilWet() { g_adc[SOIL_PIN] = 1500; }   // ~70 %
static void press(int pin, unsigned long holdMs) { g_pin[pin] = LOW; run(holdMs, 50); g_pin[pin] = HIGH; run(100, 50); }

int main() {
  for (int p : {40, 41, 42}) g_pin[p] = HIGH;
  g_pin[CHARGE_PIN] = LOW; g_mv[BAT_PIN] = 1900;   // 3.8 V sau chia áp
  soilDry();
  setup();
  printf("== A. AUTO, đất khô mãi (cạn bình): bơm phải bị cắt sau PUMP_MAX_MS rồi khóa\n");
  CHECK(pumpOn && g_out[RELAY_PIN] == HIGH, "mở máy: đất 0 % -> AUTO bật bơm");
  CHECK(!pumpLockout, "chưa khóa lúc đầu");
  run(PUMP_MAX_MS - 2000);
  CHECK(pumpOn && !pumpLockout, "còn 2 s nữa mới đủ PUMP_MAX_MS: bơm vẫn chạy");
  run(3000);
  CHECK(!pumpOn && g_out[RELAY_PIN] == LOW, "quá PUMP_MAX_MS: relay ngắt, bơm tắt");
  CHECK(pumpLockout, "pumpLockout = true");
  CHECK(redBanners() >= 1, "màn TFT: đã vẽ biểu ngữ đỏ");
  bool hasText = false; for (auto &t : g_prints) if (t.find("BINH NUOC") != std::string::npos) hasText = true;
  CHECK(hasText, "biểu ngữ có chữ KIEM TRA BINH NUOC");
  run(20000);
  CHECK(!pumpOn && pumpLockout, "đất vẫn khô 20 s nữa: AUTO KHÔNG tự bật lại khi đang khóa");
  web.req(HTTP_GET, "/api/status");
  CHECK(web.lastBody.find("\"lock\":1") != std::string::npos && web.lastBody.find("\"maxS\":60") != std::string::npos, "JSON /api/status có lock:1, maxS:60");

  printf("== B. Mở khóa bằng nút trên web\n");
  web.req(HTTP_POST, "/api/pump", {{"on", "1"}});
  CHECK(web.lastCode == 409, "web bật bơm khi đang AUTO -> 409 (như cũ)");
  web.req(HTTP_POST, "/api/pump/unlock");
  CHECK(web.lastCode == 200 && !pumpLockout, "POST /api/pump/unlock -> 200, hết khóa");
  CHECK(web.lastBody.find("\"lock\":0") != std::string::npos, "JSON báo lock:0");
  CHECK(!pumpOn, "mở khóa không tự bật bơm ngay (chờ lần đọc đất kế tiếp)");
  run(1000);
  CHECK(pumpOn, "AUTO xét lại độ ẩm (đất khô) -> bơm bật lại");

  printf("== C. Khóa lần 2, mở khóa bằng nhấn giữ PUMP\n");
  run(PUMP_MAX_MS + 1000);
  CHECK(pumpLockout && !pumpOn, "lại bị khóa sau PUMP_MAX_MS");
  press(PUMPBTN_PIN, 300);
  CHECK(pumpLockout, "nhấn ngắn PUMP: vẫn khóa");
  g_pin[PUMPBTN_PIN] = LOW; run(1000, 50);
  CHECK(pumpLockout, "giữ PUMP 1 s: chưa mở khóa");
  run(1200, 50);
  CHECK(!pumpLockout, "giữ PUMP > 2 s: mở khóa");
  g_pin[PUMPBTN_PIN] = HIGH; run(300, 50);
  CHECK(!pumpLockout, "nhả nút không khóa lại");

  printf("== D. THỦ CÔNG: giới hạn thời gian áp dụng cả khi bấm tay\n");
  run(1500);                                // để AUTO bật bơm xong (không quan trọng)
  press(MODE_PIN, 100);
  CHECK(!autoMode && !pumpOn, "MODE -> THỦ CÔNG, bơm tự tắt");
  press(PUMPBTN_PIN, 100);
  CHECK(pumpOn, "PUMP -> bơm bật (không khóa)");
  run(PUMP_MAX_MS - 2000);
  CHECK(pumpOn, "chạy tay chưa đủ giới hạn: còn bơm");
  run(3000);
  CHECK(!pumpOn && pumpLockout, "chạy tay quá PUMP_MAX_MS: tắt + khóa");
  press(PUMPBTN_PIN, 100);
  CHECK(!pumpOn, "PUMP ngắn khi đang khóa: không bật được");
  web.req(HTTP_POST, "/api/pump", {{"on", "1"}});
  CHECK(web.lastCode == 423 && !pumpOn, "web bật bơm khi đang khóa -> 423");
  int b0 = redBanners();
  press(PAGEBTN_PIN, 100);
  CHECK(redBanners() > b0, "đổi trang khi đang khóa: biểu ngữ vẫn được vẽ lại");
  g_pin[PUMPBTN_PIN] = LOW; run(2300, 50); g_pin[PUMPBTN_PIN] = HIGH; run(200, 50);
  CHECK(!pumpLockout && !pumpOn, "giữ PUMP 2 s: mở khóa, bơm vẫn tắt (THỦ CÔNG)");
  press(PUMPBTN_PIN, 100);
  CHECK(pumpOn, "sau mở khóa: PUMP bật bơm bình thường");
  press(PUMPBTN_PIN, 100);
  CHECK(!pumpOn, "PUMP lần nữa: tắt (bình thường)");

  printf("== E. Bơm bình thường (đất ẩm lên) không bị khóa\n");
  press(MODE_PIN, 100);                       // về AUTO
  soilDry(); run(2000);
  CHECK(autoMode && pumpOn, "AUTO + đất khô: bơm bật");
  run(20000);
  soilWet(); run(8000);
  CHECK(!pumpOn && !pumpLockout, "đất lên >= 45 % sau 20 s: bơm tắt, không khóa");
  run(PUMP_MAX_MS * 2);
  CHECK(!pumpOn && !pumpLockout, "để đất ẩm lâu: không bật, không khóa");

  printf("== F. Giữ trạng thái >= AUTO_MIN_HOLD_MS vẫn hoạt động (nhiễu thoáng qua)\n");
  soilDry(); run(700);
  CHECK(pumpOn, "đất khô: bơm bật");
  unsigned long tOn = relayChangedAt;
  soilWet(); run(2000);
  CHECK(pumpOn, "2 s sau khi bật, đất báo ẩm (nhiễu): chưa tắt vì phải giữ >= 5 s");
  run(4000);
  CHECK(!pumpOn && relayChangedAt - tOn >= AUTO_MIN_HOLD_MS, "đủ 5 s mới tắt");

  printf("== G. Mất cảm biến: tắt NGAY, kể cả đang giữ trạng thái\n");
  soilDry(); run(6000);        // đã qua 5 s giữ trạng thái sau lần tắt trước
  CHECK(pumpOn, "bơm bật lại");
  g_adc[SOIL_PIN] = 0; run(1700);
  CHECK(!soilOk && !pumpOn, "mất cảm biến: bơm tắt ngay, không chờ 5 s");

  printf("\n%d kiểm tra, %d lỗi\n", checks, fails);
  return fails ? 1 : 0;
}
