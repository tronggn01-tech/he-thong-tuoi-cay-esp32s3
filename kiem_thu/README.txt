KIỂM THỬ LOGIC CHƯƠNG TRÌNH tuoi_cay.ino TRÊN MÁY TÍNH (không cần mạch ESP32)

Nội dung
  mock/            Các tệp giả lập thay cho thư viện Arduino / ESP32 (Arduino.h, WiFi.h, WebServer.h,
                   Preferences.h, ESPmDNS.h, SPI.h, Adafruit_GFX.h, Adafruit_ST7789.h, DHT.h, esp_system.h).
  test_pump.cpp    Bảo vệ bơm: giới hạn 60 s, khóa, mở khóa (nút giữ 2 s và web), biểu ngữ TFT, JSON, HTTP 423 (nhóm A - G, 37 kiểm tra).
  test_t52.cpp     Các tình huống điều khiển bơm ở Bảng 5.2 (14 tình huống, 17 kiểm tra).
  test_json.cpp    In ra chuỗi JSON của /api/status để kiểm tra các trường.
  chay_kiem_thu.sh Biên dịch và chạy cả ba bộ.

Cách chạy (Linux / macOS / WSL, đã thử với g++ 13+)
  1. Đặt tuoi_cay.ino, web_page.h, bitmaps.h, boot_bitmaps.h vào một thư mục (ví dụ tuoi_cay/).
  2. Giải nén thư mục kiem_thu vào CHÍNH thư mục đó (hoặc thư mục bất kỳ rồi truyền đường dẫn sketch cho script).
  3. cd kiem_thu && ./chay_kiem_thu.sh ..
     Hoặc chạy tay một bộ:
       g++ -std=gnu++17 -I mock -I .. -DSKETCH='"../tuoi_cay.ino"' test_pump.cpp -o test_pump && ./test_pump
  Kết quả: mỗi kiểm tra in một dòng "ok" hoặc "FAIL"; dòng cuối cho tổng số kiểm tra và số lỗi.

Phương pháp: nguyên tệp tuoi_cay.ino được #include vào chương trình thử và biên dịch bằng g++; thời gian millis(),
ADC, chân nút bấm, Wi-Fi và yêu cầu HTTP do chương trình thử điều khiển; lệnh vẽ TFT được ghi lại để kiểm tra.
Giới hạn: chỉ kiểm thử logic; nhiễu điện, sụt nguồn, tốc độ SPI, sóng Wi-Fi, sai số ADC không được mô phỏng.
