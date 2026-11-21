# Hệ thống tưới cây tự động sử dụng công nghệ IoT (ESP32-S3)

Mã nguồn của tiểu luận học phần Lập trình nhúng (INT4421): thiết bị tưới cây tự động dùng ESP32-S3, đo độ ẩm đất, điều khiển máy bơm qua relay, hiển thị trên màn hình TFT và điều khiển qua trang web (Wi-Fi).

## Chức năng chính

- Đọc cảm biến độ ẩm đất điện dung và cảm biến DHT11 (nhiệt độ, độ ẩm không khí).
- Chế độ Tự động: bật bơm khi độ ẩm đất dưới 35 %, tắt khi đạt 45 % (vùng trễ); chế độ Thủ công bằng nút bấm hoặc trang web.
- Bảo vệ bơm: tắt và khóa bơm nếu chạy liên tục quá 60 giây (PUMP_MAX_MS); mở khóa bằng giữ nút PUMP 2 giây hoặc nút trên web.
- Tắt bơm an toàn khi cảm biến đất bị mất (đứt dây, chập).
- Màn hình TFT 2,4 inch ST7789 hai trang; hiển thị giờ (NTP), Wi-Fi, phần trăm pin và trạng thái sạc.
- Trang web điều khiển có mật khẩu (HTTP Basic) và đổi Wi-Fi ngay trên web (chế độ AP khi mất mạng).

## Cấu trúc thư mục

| Tệp / thư mục | Nội dung |
|---|---|
| `tuoi_cay.ino` | Chương trình chính |
| `web_page.h` | Trang web điều khiển (HTML, CSS, JavaScript) |
| `bitmaps.h`, `boot_bitmaps.h` | Ảnh nền màn hình TFT và màn hình khởi động (mảng RGB565 do công cụ tạo tự động) |
| `kiem_thu/` | Bộ kiểm thử mô phỏng chạy trên máy tính (xem `kiem_thu/README.txt`) |

## Bảng chân ESP32-S3

| Chức năng | GPIO |
|---|---|
| TFT SCK / MOSI / CS / DC / RST | 12 / 11 / 10 / 9 / 13 |
| DHT11 (DATA) | 5 |
| Cảm biến độ ẩm đất (AOUT, ADC1) | 6 |
| Relay (IN) | 38 |
| Nút PAGE / MODE / PUMP | 40 / 41 / 42 |
| Đo pin (cầu phân áp 100 kΩ / 100 kΩ, tụ 100 nF xuống GND) | 1 |
| Báo sạc (cầu phân áp 22 kΩ / 33 kΩ) | 2 |

## Biên dịch và nạp

1. Cài Arduino IDE 2.x và core **esp32 by Espressif Systems**.
2. Cài thư viện bằng Library Manager: Adafruit GFX Library, Adafruit BusIO, Adafruit ST7735 and ST7789 Library, DHT sensor library, Adafruit Unified Sensor.
3. Đặt các tệp trong thư mục có tên `tuoi_cay` (trùng tên với `tuoi_cay.ino`), mở `tuoi_cay.ino`.
4. Menu Tools: Board **ESP32S3 Dev Module**, Flash Size **16MB (128Mb)**, Partition Scheme **16M Flash (3MB APP/9.9MB FATFS)**.
5. Sửa `WIFI_SSID`, `WIFI_PASS`, `WEB_USER`, `WEB_PASS`, `AP_PASS` trong `tuoi_cay.ino` (đổi mật khẩu mặc định), chọn đúng cổng COM rồi Upload. Serial Monitor 115200 baud.

Lưu ý: tháo nguồn 5 V bên ngoài trước khi cắm cáp USB để hai nguồn không đấu vào nhau.

## Kiểm thử mô phỏng

Chạy trong thư mục `kiem_thu` (cần g++ hỗ trợ C++17): `./chay_kiem_thu.sh ..`. Bộ kiểm thử chạy nguyên `tuoi_cay.ino` với các lớp giả lập Arduino/ESP32 và chỉ kiểm tra logic; nhiễu điện, sụt nguồn, tốc độ SPI, sóng Wi-Fi và sai số ADC không được mô phỏng.

## Giới hạn

Giá trị giới hạn bơm 60 giây là giá trị đặt thử, cần chỉnh theo lưu lượng bơm và thể tích chậu rồi kiểm chứng trên mạch thật. Hệ số đo pin `BAT_DIVIDER = 2,0` là giá trị danh định, chưa hiệu chuẩn bằng đồng hồ vạn năng.
