/*
 * HỆ THỐNG TƯỚI CÂY THÔNG MINH  -  ESP32-S3 + TFT ST7789 2.4" (240x320, xoay NGANG -> 320x240)
 *
 *  HAI TRANG giao diện (cùng phong cách "Tưới cây thông minh"), đổi trang bằng nút PAGE:
 *
 *   TRANG 0 - TRANG CHỦ (mở máy là vào trang này):
 *     - Tên hệ thống "Tưới cây thông minh"
 *     - Giờ thật (giờ:phút lớn + giây nhỏ) và thứ / ngày / tháng / năm, lấy từ Internet (Wi-Fi + NTP)
 *     - Kết nối Wi-Fi: vạch sóng theo cường độ, "Đã kết nối" / "Đang kết nối" / "Mất kết nối" + tên mạng
 *     - Dung lượng pin (%) có biểu tượng pin đổi màu xanh / cam / đỏ
 *       + HIỆU ỨNG KHI ĐANG SẠC: viền thẻ Pin sáng xanh nhấp nháy, nhãn "Đang sạc" + tia sét, ruột pin chạy vệt sáng,
 *         số % đổi màu bạc hà. Pin đầy (từ 98%) mà còn cắm sạc: nhãn "Đã đầy". Cách nhận biết sạc: xem mục "SẠC PIN" bên dưới.
 *     - Chế độ đang chạy: TỰ ĐỘNG (xanh lá) hoặc THỦ CÔNG (cam); có giọt nước xanh khi máy bơm đang chạy
 *
 *   TRANG 1 - THÔNG SỐ:
 *     - Nhiệt độ (DHT11) và độ ẩm không khí (DHT11) ở góc trên bên phải
 *     - Độ ẩm đất (số lớn + nhãn vùng + thanh 4 vùng có chấm báo vị trí)
 *     - Chế độ vận hành: công tắc trượt  TỰ ĐỘNG <-> THỦ CÔNG
 *     - Máy bơm tưới: hình máy bơm (có nước nhỏ giọt khi bơm chạy) + trạng thái "Đang bơm"/"Đã tắt"
 *
 *  Bơm tưới, đọc cảm biến, nút MODE / PUMP luôn chạy bình thường ở CẢ HAI trang.
 *
 *  MÀN HÌNH KHỞI ĐỘNG: bật máy là hiện logo chiếc lá, tên "Tưới cây thông minh" và thanh tiến trình (0 -> 100%) đi qua 4 bước thật:
 *    khởi động màn hình -> cảm biến (DHT11, độ ẩm đất) -> Wi-Fi -> đọc pin -> "Sẵn sàng!" rồi vào TRANG CHỦ (khoảng 1,5 giây).
 *    Hình và chữ lấy từ file boot_bitmaps.h (để cùng thư mục với tuoi_cay.ino và bitmaps.h). Muốn bỏ: đặt BOOT_SCREEN = 0 ở mục MÀN HÌNH KHỞI ĐỘNG.
 *
 *  CHẾ ĐỘ BƠM (giữ nguyên như bản cũ)
 *    AUTO   : độ ẩm đất < 35%  -> TỰ BẬT bơm.
 *             Bơm tắt khi độ ẩm đất >= 45% (chống bật/tắt liên tục, đổi ở SOIL_OFF_AT).
 *    MANUAL : bơm chỉ bật/tắt bằng nút PUMP. Vào MANUAL bơm tự tắt.
 *
 *  KHÔNG NHẬN ĐƯỢC CẢM BIẾN ĐẤT (đứt dây / chưa cắm / chân bỏ hở / chập):
 *    - Số đất hiện "--%" màu xám, nhãn "Lỗi", thanh vùng không có chấm, dòng phụ "Chưa nhận cảm biến đất".
 *    - AUTO  : bơm được TẮT (không tưới "mù" khi không có số liệu).
 *    - MANUAL: vẫn bật/tắt bơm bằng nút PUMP như bình thường.
 *    - Cắm lại cảm biến thì tự chạy lại bình thường. Ngưỡng phát hiện chỉnh ở SOIL_RAW_MIN / SOIL_RAW_MAX / SOIL_NOISE_MAX.
 *
 *  CHỐNG NHIỄU KHI BƠM CHẠY (motor bơm gây xung nhiễu và làm sụt nguồn mỗi lần khởi động / dừng):
 *    - Độ ẩm đất: đọc 16 mẫu, bỏ 4 mẫu thấp nhất và 4 mẫu cao nhất rồi lấy trung bình 8 mẫu giữa -> vài xung nhiễu không làm lệch số.
 *    - Bỏ qua các lần đọc đất trong SOIL_BLANK_MS ms ngay sau khi đóng / ngắt rơ-le (lúc đó nhiễu và sụt nguồn mạnh nhất).
 *    - AUTO: sau mỗi lần đổi trạng thái bơm, giữ nguyên ít nhất AUTO_MIN_HOLD_MS ms rồi mới cho độ ẩm đất đổi tiếp, để nhiễu không làm bơm
 *      bật - tắt dồn dập (mỗi lần bơm khởi động là một lần sụt nguồn). Mất cảm biến vẫn tắt bơm NGAY; nút bấm / web / đổi chế độ không bị giữ.
 *    - Phần mềm KHÔNG thay được việc xử lý nguồn: nếu máy vẫn tự khởi động lại khi bơm chạy, xem dòng "KHOI DONG LAI" trên Serial Monitor
 *      (SUT NGUON = brownout) và làm theo hướng dẫn phần cứng: tụ 100nF ở hai cực bơm, tụ 470..1000uF cạnh relay + bơm, nguồn riêng cho bơm.
 *
 *  BẢO VỆ BƠM (cạn bình nước, bơm chạy khô, cảm biến đất kẹt không báo "đủ ẩm"):
 *    - Bơm chạy LIÊN TỤC quá PUMP_MAX_MS (mặc định 60 giây; áp dụng cho cả TỰ ĐỘNG và THỦ CÔNG) thì TẮT bơm và KHÓA bơm (pumpLockout).
 *    - Đang khóa: màn TFT hiện biểu ngữ đỏ "BOM BI KHOA: KIEM TRA BINH NUOC" ở mép trên (cả hai trang), trang web hiện "Bị khóa" + nút
 *      "Mở khóa bơm", Serial Monitor in cảnh báo. AUTO không bật bơm, nút PUMP và web cũng không bật được bơm.
 *    - Mở khóa: NHẤN GIỮ nút PUMP 2 giây (PUMP_UNLOCK_HOLD_MS) hoặc bấm "Mở khóa bơm" trên web, sau khi đã kiểm tra bình nước.
 *      Khóa chỉ nằm trong RAM: tắt máy / khởi động lại là hết khóa.
 *    - PUMP_MAX_MS phải chọn theo lưu lượng bơm và thể tích chậu (đủ để độ ẩm đất đi từ 35% lên 45% khi bình còn nước), rồi kiểm chứng bằng
 *      thực nghiệm; đặt 0 để bỏ giới hạn.
 *
 *  NÚT BẤM (mỗi nút: một chân nối GPIO, chân kia nối GND, dùng pull-up nội):
 *    GPIO40 = PAGE : đổi trang  TRANG CHỦ <-> THÔNG SỐ
 *    GPIO41 = MODE : đổi AUTO <-> MANUAL
 *    GPIO42 = PUMP : bật/tắt bơm (chỉ có tác dụng ở MANUAL); NHẤN GIỮ 2 giây khi bơm đang bị khóa = mở khóa bơm
 *
 *  ĐO PIN: GPIO1 qua cầu phân áp 2 điện trở 100k + tụ lọc 100nF GPIO1-GND (Vpin = mV đọc được x BAT_DIVIDER). Không nối pin -> hiện "--%".
 *
 *  SẠC PIN - nhận biết "đang sạc" bằng chân báo có nguồn sạc (đặt ở mục CHÂN KẾT NỐI bên dưới)
 *    Lấy 5V của cổng sạc, chia xuống ~3V bằng cầu phân áp 22k / 33k rồi vào GPIO2 (CHARGE_PIN)
 *
 *          5V cổng sạc ---[ 22k ]---+--- GPIO2 (CHARGE_PIN)
 *                                   |
 *                                [ 33k ]
 *                                   |
 *                                  GND   (nối chung GND với ESP32 và mạch sạc)
 *
 *        R trên = 22k: một đầu vào 5V cổng sạc, đầu kia vào GPIO2.   R dưới = 33k: một đầu vào GPIO2, đầu kia vào GND.
 *        Cắm sạc: 5V x 33 / (22 + 33) = 3,0V (cổng USB 4,75 .. 5,25V -> 2,85 .. 3,15V), trên ngưỡng mức CAO của ESP32-S3 (2,475V). Rút sạc: 0V.
 *        Trở sai số 5% vẫn đạt: xấu nhất ~2,7V khi cổng chỉ có 4,75V, cao nhất ~3,28V khi 5,25V (không vượt 3,3V).
 *        Nếu sau này đổi điện trở: giữ R dưới = 1,2 .. 2 lần R trên; đừng chia đôi 1:1 (chỉ ~2,5V, sát ngưỡng 2,475V nên dễ không nhận).
 *        Code để chân ở chế độ INPUT (không bật điện trở kéo nội ~45k vì nó song song với trở 33k làm điện áp tụt thấp).
 *        Chưa đấu cầu phân áp thì chân để hở và có thể báo sạc nhầm. Nếu chân nhảy lung tung (gần relay / bơm): thêm tụ 100nF GPIO2 - GND.
 *        Cắm sạc -> chân GPIO2 lên mức CAO, rút sạc -> mức THẤP.
 *        Pin chưa đầy: "Đang sạc"; pin từ 98% trở lên: "Đã đầy" (vẫn cắm sạc). Rút sạc: về hiển thị bình thường.
 *    Khi đang sạc, điện áp đo được trên pin CAO HƠN điện áp thật (dòng sạc chạy qua nội trở pin, dây, mạch bảo vệ) nên không đổi thẳng ra %:
 *      - vừa cắm sạc: giữ nguyên % cũ khoảng 30 giây (CHARGE_SETTLE_MS) cho dòng sạc ổn định, rồi đo mức điện áp bị đội lên so với
 *        điện áp pin ngay trước lúc cắm và trừ mức đó đi (% không nhảy vọt khi cắm sạc, chỉ tăng dần, không bao giờ tụt khi đang sạc);
 *      - gần cuối (điện áp đo chạm ~4,2 V = mạch sạc chuyển sang giai đoạn áp không đổi): dòng giảm dần nên % được ước tính theo thời gian,
 *        tiến dần tới 100% (CHARGE_CV_TAU_MS). Mạch sạc không báo "đã đầy" nên đây chỉ là ước tính; muốn chính xác hãy nối thêm chân STDBY của mạch sạc;
 *      - rút sạc: % không vượt quá % trước lúc rút trong 2 phút (CHARGE_CEIL_MS) vì điện áp pin còn "dư" một lúc sau khi sạc.
 *
 *  VÙNG ĐỘ ẨM ĐẤT (chỉ số tương đối, để gọi tên / tô màu; KHÔNG dùng để điều khiển bơm):
 *      x < 20 Rất khô | 20 <= x < 30 Khô | 30 <= x <= 50 Lý tưởng | x > 50 Ướt   (x là số nguyên 0..100)
 *
 *  ĐIỀU KHIỂN QUA WEB (điện thoại / máy tính dùng CÙNG mạng Wi-Fi với ESP32):
 *    - Mở trình duyệt vào  http://tuoicay.local  hoặc  http://<IP của ESP32>  (IP in trên Serial Monitor 115200, nhắc lại mỗi 60 giây).
 *    - Hỏi tên + mật khẩu: WEB_USER / WEB_PASS (đổi ở mục WI-FI bên dưới). Trang hiện nhiệt độ, độ ẩm không khí, độ ẩm đất (4 vùng),
 *      pin / sạc, chế độ TỰ ĐỘNG <-> THỦ CÔNG và nút BẬT / TẮT BƠM; tự cập nhật mỗi giây.
 *    - Quy tắc y hệt nút bấm trên máy: đổi chế độ bằng web = bấm MODE; bật/tắt bơm bằng web = bấm PUMP (chỉ khi THỦ CÔNG; đang TỰ ĐỘNG
 *      thì web báo "chuyển sang Thủ công trước"). Nút trên máy và web dùng song song, màn TFT cập nhật theo.
 *    - Chỉ dùng trong mạng nhà. Điều khiển từ xa qua Internet cần thêm VPN / chuyển tiếp cổng / dịch vụ đám mây (chưa có trong code này).
 *
 *  ĐỔI WI-FI KHÔNG CẦN MỞ CODE / NẠP LẠI (mang máy sang nơi khác, đổi router, đổi mật khẩu Wi-Fi):
 *    - Mở máy ở nơi không có Wi-Fi cũ (hoặc mất mạng) quá AP_AFTER_MS (60 giây): thiết bị tự phát Wi-Fi riêng tên "TuoiCay-Setup"
 *      (mật khẩu AP_PASS, mặc định 12345678; tên mạng này cũng hiện ở thẻ Wi-Fi trên màn hình). Trên điện thoại: nối vào Wi-Fi đó,
 *      mở http://192.168.4.1, nhập WEB_USER / WEB_PASS, bấm nút "Wi-Fi" ở đầu trang, chọn mạng trong danh sách (hoặc gõ tên mạng),
 *      nhập mật khẩu, bấm "Lưu và kết nối". Nối được thì tên + mật khẩu được LƯU vào bộ nhớ flash (tắt máy vẫn còn);
 *      nối không được (sai mật khẩu...) thì KHÔNG lưu và giữ lại mạng cũ.
 *    - Đang nối Wi-Fi bình thường vẫn đổi được: mở trang web, bấm "Wi-Fi", làm như trên.
 *    - WIFI_SSID / WIFI_PASS ở mục WI-FI chỉ là mạng MẶC ĐỊNH, dùng khi chưa lưu mạng nào.
 *
 *  Gồm 3 file cùng thư mục sketch: tuoi_cay.ino (logic) + bitmaps.h (hình màn TFT, tạo tự động từ ảnh thiết kế) + web_page.h (trang web).
 *  Thư viện: Adafruit ST7789 (gói Adafruit ST7735 and ST7789 Library), Adafruit GFX, Adafruit BusIO,
 *            DHT sensor library, Adafruit Unified Sensor   (WiFi, WebServer, ESPmDNS và time có sẵn trong core ESP32)
 *
 *  LƯU Ý NẠP CODE: hình ảnh (~560 KB) + Wi-Fi + web làm chương trình dài ~1,4 MB (core ESP32 2.0.x) đến ~1,7 MB (core 3.x),
 *     quá phân vùng mặc định (1,25 MB) nên PHẢI đổi phân vùng. Module ESP32-S3-N16R8 có flash 16 MB nên chọn (vẫn có OTA vì có 2 phân vùng ứng dụng):
 *       Arduino IDE -> Tools -> Flash Size -> "16MB (128Mb)"  và  Tools -> Partition Scheme -> "16M Flash (3MB APP/9.9MB FATFS)"
 *     Board chỉ có flash 4 MB thì dùng "Huge APP (3MB No OTA/1MB SPIFFS)" (không có OTA).
 *     Nếu quên sẽ báo "Sketch too big" / "text section exceeds available space in board".
 */

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <time.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <esp_system.h>          // esp_reset_reason(): biết vì sao máy vừa khởi động lại (sụt nguồn, watchdog...)
#endif

#include "bitmaps.h"   // nền 320x240 (2 trang) + sprite + bảng chữ số (tạo tự động)
#include "web_page.h"  // trang web điều khiển (HTML trong flash)

// ===================== MÀN HÌNH KHỞI ĐỘNG =====================
// Cần thêm file boot_bitmaps.h (cùng thư mục với file này). Đặt BOOT_SCREEN = 0 để bỏ màn hình khởi động (vào thẳng trang chủ).
#define BOOT_SCREEN    1      // 1 = hiện logo + thanh tiến trình lúc bật máy
#define BOOT_STEP_MS   6      // ms nghỉ giữa hai khung hình khi thanh chạy (nhỏ = chạy nhanh)
#define BOOT_HOLD_MS   450    // ms giữ "Sẵn sàng! 100%" trước khi vào trang chủ
#if BOOT_SCREEN
#include "boot_bitmaps.h"     // logo, tên hệ thống, chữ số, nhãn các bước (mặt nạ độ đậm, màu tô lúc chạy)
#endif

// ===================== WI-FI / GIỜ THẬT =====================
const char *WIFI_SSID = "TEN_WIFI";          // <-- mạng Wi-Fi MẶC ĐỊNH (dùng khi chưa lưu mạng nào); sau này đổi bằng trang web, khỏi sửa ở đây
const char *WIFI_PASS = "MAT_KHAU_WIFI";     // <-- mật khẩu của mạng mặc định
const char *NTP_SERVER1 = "pool.ntp.org";
const char *NTP_SERVER2 = "time.google.com";
const long  GMT_OFFSET_SEC = 7 * 3600;       // Việt Nam = UTC+7

// Đổi Wi-Fi bằng trang web (xem mục "ĐỔI WI-FI KHÔNG CẦN MỞ CODE" ở đầu file)
const char *AP_SSID = "TuoiCay-Setup";           // tên Wi-Fi riêng của thiết bị khi cần cài đặt
const char  AP_PASS[] = "12345678";              // mật khẩu Wi-Fi riêng (8..63 ký tự) - nên đổi
static_assert(sizeof(AP_PASS) >= 9 && sizeof(AP_PASS) <= 64, "AP_PASS phai dai tu 8 den 63 ky tu");
const unsigned long AP_AFTER_MS   = 60000;       // ms: không nối được Wi-Fi quá chừng này thì mở Wi-Fi riêng
const unsigned long AP_KEEP_MS    = 120000;      // ms: nối lại được Wi-Fi rồi, đợi chừng này (và không còn ai nối vào) mới tắt Wi-Fi riêng
const unsigned long WIFI_TRIAL_MS = 25000;       // ms: thử nối mạng mới; quá chừng này chưa được thì bỏ, quay lại mạng cũ

// ===================== WEB =====================
const char *WEB_USER = "admin";              // tên đăng nhập trang web
const char *WEB_PASS = "MAT_KHAU_WEB";       // <-- ĐỔI mật khẩu trang web (ai cùng Wi-Fi cũng phải nhập đúng mới xem / điều khiển được)
const char *WEB_HOST = "tuoicay";            // tên thiết bị: http://tuoicay.local (cũng là tên trong danh sách thiết bị của router)
const unsigned long WEB_MSG_EVERY = 60000;   // ms: nhắc lại địa chỉ trang web trên Serial Monitor

// ===================== CHÂN KẾT NỐI =====================
#define TFT_SCK    12
#define TFT_MOSI   11
#define TFT_CS     10
#define TFT_DC      9
#define TFT_RST    13

#define DHT_PIN     5
#define SOIL_PIN    6      // cảm biến đất (analog, ADC1)
#define RELAY_PIN  38
#define PAGEBTN_PIN 40     // nút PAGE : đổi trang
#define MODE_PIN   41      // nút MODE : AUTO <-> MANUAL
#define PUMPBTN_PIN 42     // nút PUMP : bật/tắt bơm (chế độ MANUAL)
#define BAT_PIN     1      // đo pin qua cầu phân áp 2 điện trở 100k + tụ 100nF GPIO1-GND (GPIO1 = ADC1)

// Nhận biết đang sạc pin (xem mục "SẠC PIN" ở đầu file): 5V cổng sạc qua cầu phân áp 22k (trên) / 33k (dưới) vào CHARGE_PIN, cắm sạc = mức CAO
#define CHARGE_PIN  2      // GPIO2

// Đổi chân làm mạch in: Wi-Fi chiếm ADC2 (GPIO11..20) nên chân ĐỌC ANALOG phải là ADC1 = GPIO1..10, nếu không analogRead() sai khi Wi-Fi chạy.
// Không dùng GPIO0, 3, 45, 46 (chân khởi động), 19, 20 (USB), 26..37 (flash / PSRAM), 43, 44 (Serial).
static_assert(SOIL_PIN >= 1 && SOIL_PIN <= 10 && BAT_PIN >= 1 && BAT_PIN <= 10, "SOIL_PIN va BAT_PIN phai la GPIO1..GPIO10 (ADC1)");

// Hướng màn hình ngang: 1 hoặc 3 (xoay 180°). Nếu hình bị lộn ngược thì đổi 1 <-> 3.
#define TFT_ROTATION 3

// ----- NẾU MÀU TRÊN MÀN HÌNH BỊ SAI (màu nền, màu chữ...) -----
//  Bước 1: đặt TFT_COLOR_TEST = 1 rồi nạp lại: màn hiện 5 ô ĐỎ / XANH LÁ / XANH DƯƠNG / TRẮNG / ĐEN trong 6 giây.
//  Bước 2: chỉnh theo những gì nhìn thấy:
//    - ô TRẮNG hiện đen và ô ĐEN hiện trắng (hình như "âm bản")  -> đổi TFT_INVERT (1 <-> 0)
//    - ô ĐỎ hiện xanh dương và ô XANH DƯƠNG hiện đỏ               -> đổi TFT_BGR từ 0 thành 1
//  Bước 3: đúng màu rồi thì đặt lại TFT_COLOR_TEST = 0.
#define TFT_INVERT      1   // 1 = bật đảo màu (mặc định của thư viện Adafruit_ST7789, hợp màn IPS) ; 0 = tắt
#define TFT_BGR         0   // 1 = hoán đổi Đỏ <-> Xanh dương
#define TFT_COLOR_TEST  0   // 1 = hiện 5 ô thử màu lúc khởi động

// ===================== CẤU HÌNH =====================
const bool RELAY_ACTIVE_LOW = false;   // rơ-le kích mức CAO (module của bạn). Đổi thành true nếu rơ-le kích mức THẤP

const int SOIL_DRY = 2370;      // raw khi để ngoài không khí (khô)
const int SOIL_WET = 1120;      // raw khi nhúng nước (ướt)

const int SOIL_ON_BELOW = 35;   // AUTO: dưới mức này thì BẬT bơm
const int SOIL_OFF_AT   = 45;   // AUTO: từ mức này trở lên thì TẮT bơm (đặt = 35 nếu muốn tắt ngay)

// Phát hiện "không nhận cảm biến đất" (giá trị raw 12 bit của ADC)
const int SOIL_RAW_MIN   = 150;    // raw dưới mức này (gần 0 V: đứt dây, chưa cắm, nối GND)  -> không có cảm biến
const int SOIL_RAW_MAX   = 3950;   // raw trên mức này (gần 3.3 V: chập, nối vào 3V3)           -> không có cảm biến
const int SOIL_NOISE_MAX = 450;    // độ rộng dao động của 12 mẫu giữa (bỏ 2 mẫu thấp nhất và 2 cao nhất trong 16): chân bỏ hở bắt nhiễu nên dao động rất mạnh; vài xung nhiễu riêng lẻ thì bị bỏ qua
const uint8_t SOIL_LOST_COUNT = 3; // số lần đọc xấu LIÊN TIẾP (mỗi lần SOIL_INTERVAL) để báo MẤT cảm biến
const uint8_t SOIL_BACK_COUNT = 2; // số lần đọc tốt LIÊN TIẾP để báo CÓ LẠI cảm biến
const unsigned long SOIL_BLANK_MS    = 600;    // ms: ngay sau khi đóng / ngắt rơ-le (bơm khởi động / dừng gây xung nhiễu + sụt nguồn) bỏ qua các lần đọc đất
const unsigned long AUTO_MIN_HOLD_MS = 5000;   // ms: AUTO giữ nguyên trạng thái bơm ít nhất chừng này sau mỗi lần đổi (chống bật/tắt dồn dập do nhiễu). 0 = tắt
const unsigned long PUMP_MAX_MS      = 60000;  // ms: bơm chạy liên tục quá chừng này thì TẮT và KHÓA bơm (chống chạy khô / cạn bình). Chỉnh theo bơm + chậu thật. 0 = không giới hạn
const unsigned long PUMP_UNLOCK_HOLD_MS = 2000; // ms: nhấn giữ nút PUMP chừng này để MỞ KHÓA bơm

// Pin: Vpin = mV_đọc_được * BAT_DIVIDER. Phân áp 100k/100k -> 2.0 (danh định; hiệu chuẩn: BAT_DIVIDER = Vpin đo bằng đồng hồ / mV đọc được ở GPIO1)
const float BAT_DIVIDER = 2.0f;

// Sạc pin
const float CHARGE_V_OFFSET_MV = 180;   // khi sạc, điện áp đo cao hơn pin thật (dòng sạc x nội trở). Bình thường máy tự đo mức này lúc cắm sạc;
                                        // con số này chỉ dùng khi không đo được (vd: bật máy khi đã cắm sạc sẵn)
const unsigned long CHARGE_SETTLE_MS = 30000;      // vừa cắm sạc: giữ nguyên % chừng này (ms) cho dòng sạc ổn định rồi mới đo mức đội điện áp
const float CHARGE_CV_MV = 4160;                   // điện áp đo (mV) từ mức này = mạch sạc đã sang giai đoạn áp không đổi (~4.2 V)
const unsigned long CHARGE_CV_TAU_MS = 2700000UL;  // ước tính thời gian (ms) giai đoạn áp không đổi: 45 phút ứng với mức đội 190 mV. Pin to hơn -> tăng
const unsigned long CHARGE_CEIL_MS = 120000;       // vừa rút sạc: trong chừng này (ms), % không vượt quá % lúc đang sạc
#define CHG_FRAMES 16                   // số khung của một vòng hiệu ứng sạc
const unsigned long CHARGE_ANIM = 100;  // ms giữa 2 khung hiệu ứng sạc (16 khung = 1,6 giây / vòng)

const unsigned long DHT_INTERVAL  = 2000;   // ms
const unsigned long SOIL_INTERVAL = 500;    // ms
const unsigned long ANIM_INTERVAL = 130;    // ms giữa 2 khung nước nhỏ giọt
const unsigned long BTN_DEBOUNCE  = 50;     // ms
const unsigned long TOGGLE_STEP   = 12;     // ms giữa 2 khung khi công tắc trượt
const unsigned long BAT_INTERVAL  = 5000;   // ms giữa 2 lần đo pin
const unsigned long CLOCK_POLL    = 200;    // ms: kiểm tra giờ 5 lần/giây, chỉ vẽ lại khi giây đổi
const unsigned long WIFI_POLL     = 500;    // ms giữa 2 lần xem trạng thái Wi-Fi
const unsigned long WIFI_ANIM     = 350;    // ms giữa 2 khung vạch sóng khi "Đang kết nối"
const unsigned long WIFI_GIVEUP   = 20000;  // ms: quá thời gian này chưa nối được thì hiện "Mất kết nối"
const unsigned long WIFI_RETRY    = 30000;  // ms: tự thử nối lại Wi-Fi sau mỗi chừng này khi đang mất kết nối

// ===================== ĐỐI TƯỢNG =====================
Adafruit_ST7789 tft(&SPI, TFT_CS, TFT_DC, TFT_RST);
DHT dht(DHT_PIN, DHT11);

// Bộ đệm ghép chữ số lên nền: lớn nhất là ô đồng hồ (HCLK_W x HCLK_H) ; số đất 148x55 và thẻ Pin 98x56 nhỏ hơn
#define WIN_PX (HCLK_W * HCLK_H)
static_assert(WIN_PX >= SOIL_WIN_W * SOIL_WIN_H, "winBuf nho hon o so dat");
static_assert(WIN_PX >= HCHG_W * HCHG_H, "winBuf nho hon the Pin");
static uint16_t winBuf[WIN_PX] __attribute__((aligned(4)));
static uint16_t barBuf[BAR_W * BAR_H] __attribute__((aligned(4)));

// ===================== TRẠNG THÁI =====================
enum { PAGE_HOME = 0, PAGE_DATA = 1 };
enum { WU_OFF = 0, WU_ING = 1, WU_ON = 2 };   // trạng thái Wi-Fi hiển thị: mất / đang nối / đã nối

uint8_t page = PAGE_HOME;        // trang đang hiển thị (mở máy vào trang chủ)
static const uint16_t *curBg = BG_FULL;       // nền của trang đang hiển thị (dùng khi ghép chữ)

float   temperature = NAN;
float   humidity    = NAN;
int     soilPct     = 0;
bool    autoMode    = true;      // true = AUTO, false = MANUAL
bool    pumpOn      = false;
bool    relayChanged = false;    // rơ-le đã đổi trạng thái ít nhất một lần từ lúc bật máy
unsigned long relayChangedAt = 0; // thời điểm đổi gần nhất (ms)
bool    pumpLockout = false;     // true = bơm bị KHÓA vì chạy quá PUMP_MAX_MS: không bật được cho tới khi người dùng mở khóa
unsigned long pumpOnSince = 0;   // thời điểm bơm vừa bật (ms) - để tính thời gian chạy liên tục
uint8_t animFrame   = 0;         // khung nước nhỏ giọt của bơm
uint8_t tgFrame     = 0;         // khung công tắc: 0 = AUTO ... TG_FRAMES-1 = MANUAL
bool    soilOk      = true;      // false = KHÔNG nhận được cảm biến đất
uint8_t soilBad     = 0;         // đếm số lần đọc xấu liên tiếp
uint8_t soilGood    = 0;         // đếm số lần đọc tốt liên tiếp (khi đang mất cảm biến)

uint8_t wifiUi      = WU_ING;    // trạng thái Wi-Fi đang hiển thị
uint8_t wifiBars    = 1;         // số vạch sóng (1..3) khi đã nối
uint8_t wifiFrame   = 0;         // khung hoạt ảnh "đang kết nối" (0..3)
bool    wifiWasOn   = false;
bool    ntpStarted  = false;
bool    webStarted  = false;     // đã mở máy chủ web (làm ngay khi Wi-Fi nối được lần đầu)
bool    timeSynced  = false;     // đã có giờ thật từ Internet chưa
int     shownSec    = -1;        // giây đang hiển thị (-2: đang hiện "--:--")
int     shownDay    = -1;        // ngày đang hiển thị
int     batPct      = -1;        // % pin, -1: không có pin
float   batMv       = 0;         // điện áp pin (mV) đã lọc nhiễu
int     shownBat    = -2;        // % pin đang hiển thị
// Đo pin khi sạc (xem readBattery)
float   batRest1 = 0, batRest2 = 0;   // 2 điện áp pin (đã lọc) gần nhất lúc KHÔNG sạc: để biết điện áp nghỉ ngay trước lúc cắm sạc
float   chgRestMv = 0;                // điện áp nghỉ trước khi cắm sạc (0: không biết)
unsigned long chgPlugAt = 0;          // lúc cắm sạc (ms)
bool    chgOffReady = false;          // đã đo xong mức đội điện áp do dòng sạc chưa
float   chgOffMv = 0;                 // mức đội điện áp (mV) đang trừ đi
bool    chgCv = false;                // mạch sạc đã sang giai đoạn áp không đổi
unsigned long chgCvAt = 0;            // lúc sang giai đoạn áp không đổi (ms)
float   chgEma = 0, chgRefMv = 0;     // điện áp đo (trung bình trượt) và mốc 10 phút trước: để thấy điện áp đã ngừng tăng
unsigned long chgRefAt = 0;
int     batCeil = -1;                 // vừa rút sạc: % không vượt quá số này (-1: không giới hạn)
unsigned long batCeilAt = 0;

enum { CHGS_NONE = 0, CHGS_RUN = 1, CHGS_FULL = 2 };   // sạc: không sạc / đang sạc / đã đầy (vẫn cắm sạc)
uint8_t chgState    = CHGS_NONE; // trạng thái sạc đang hiển thị
bool    chgPlugged  = false;     // đang có sạc (chân CHARGE_PIN mức CAO); "đầy hay chưa" xét thêm theo % pin
uint8_t chgFrame    = 0;         // khung hoạt ảnh sạc 0..CHG_FRAMES-1
uint8_t chgUp       = 0;     // số lần đọc chân báo LIÊN TIẾP khác trạng thái hiện tại (lọc nhiễu)

unsigned long lastDht = 0, lastSoil = 0, lastAnim = 0, lastBat = 0, lastClk = 0, lastChg = 0, lastChgAnim = 0;
unsigned long lastWifi = 0, lastWifiAnim = 0, wifiTryStart = 0, wifiRetry = 0, lastWebMsg = 0;

// Wi-Fi cài đặt qua web
char    wifiSsid[33], wifiPass[65];   // mạng đang dùng (đọc từ bộ nhớ flash; chưa lưu thì lấy WIFI_SSID / WIFI_PASS)
char    pendSsid[33], pendPass[65];   // mạng mới đang thử
uint8_t wifiTrial   = 0;              // 0 chưa thử | 1 đang thử mạng mới | 2 nối được (đã lưu) | 3 KHÔNG nối được (không lưu, giữ mạng cũ)
bool    trialBegun  = false;          // đã gọi WiFi.begin() cho mạng mới
bool    apOn        = false;          // đang phát Wi-Fi riêng để cài đặt
bool    mdnsOk      = false;
bool    wifiUpSeen  = false;          // đang nối Wi-Fi (để tính thời gian đã nối)
unsigned long trialStart = 0, wifiUpSince = 0, lastMdns = 0;

// Cửa sổ ghép chữ: bộ đệm RGB565 phủ hình chữ nhật (x,y,w,h) của màn hình
struct Win { int x, y, w, h; uint16_t *buf; };

struct Btn { uint8_t pin; bool last; unsigned long t; };
Btn btnPage = { PAGEBTN_PIN, HIGH, 0 };
Btn btnMode = { MODE_PIN, HIGH, 0 };
Btn btnPump = { PUMPBTN_PIN, HIGH, 0 };

// ===================== HÀM VẼ CƠ BẢN =====================
// Đẩy một khối RGB565 (w*h) lên màn hình tại (x,y)
static void blit(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *bmp) {
  tft.startWrite();
  tft.setAddrWindow(x, y, w, h);
  tft.writePixels((uint16_t *)bmp, (uint32_t)w * h);
  tft.endWrite();
}

// Chép w*h điểm nền của trang đang hiển thị (BG_FULL hoặc BG_HOME, trong flash) vào bộ đệm
static void loadBg(uint16_t *buf, int x, int y, int w, int h) {
  for (int r = 0; r < h; r++)
    memcpy_P(buf + r * w, &curBg[(y + r) * UI_W + x], w * sizeof(uint16_t));
}

// Gói 8 bit/kênh -> RGB565 kèm nhiễu (dither) theo toạ độ màn hình (x,y): tránh vằn màu ở vùng chuyển sắc
static inline uint16_t pack565(int r, int g, int b, int x, int y) {
  int T = pgm_read_byte(&DITHER16[((y & 15) << 4) | (x & 15)]);   // 0..255
  int r5 = (r * 31 + T) / 255; if (r5 > 31) r5 = 31;
  int g6 = (g * 63 + T) / 255; if (g6 > 63) g6 = 63;
  int b5 = (b * 31 + T) / 255; if (b5 > 31) b5 = 31;
  return (uint16_t)((r5 << 11) | (g6 << 5) | b5);
}

// Trộn một điểm RGB565 (tại toạ độ màn hình x,y) với màu (r,g,b) theo độ trong suốt a (0..255)
static inline uint16_t mix565(uint16_t c, int r, int g, int b, int a, int x, int y) {
  int cr = (c >> 11) & 31; cr = (cr << 3) | (cr >> 2);
  int cg = (c >> 5) & 63;  cg = (cg << 2) | (cg >> 4);
  int cb = c & 31;         cb = (cb << 3) | (cb >> 2);
  cr += (r - cr) * a / 255;
  cg += (g - cg) * a / 255;
  cb += (b - cb) * a / 255;
  return pack565(cr, cg, cb, x, y);
}

static void winLoad(Win &W) { loadBg(W.buf, W.x, W.y, W.w, W.h); }
static void winPush(Win &W) { blit(W.x, W.y, W.w, W.h, W.buf); }

// Ghép bảng alpha (w x h) lên cửa sổ tại (x0,y0) (toạ độ trong cửa sổ).
//  rowRGB != NULL : màu từng hàng (3 byte/hàng, hàng tính theo cửa sổ); ngược lại dùng màu cố định rgb24.
//  k = 0..255 : nhân thêm vào độ đậm (255 = giữ nguyên) - dùng cho hiệu ứng "thở" của viền sáng.
static void putAlphaK(Win &W, int x0, int y0, const uint8_t *alpha, int w, int h,
                      const uint8_t *rowRGB, uint32_t rgb24, int k) {
  for (int r = 0; r < h; r++) {
    int y = y0 + r;
    if (y < 0 || y >= W.h) continue;
    int cr, cg, cb;
    if (rowRGB) {
      cr = pgm_read_byte(&rowRGB[y * 3]);
      cg = pgm_read_byte(&rowRGB[y * 3 + 1]);
      cb = pgm_read_byte(&rowRGB[y * 3 + 2]);
    } else {
      cr = (rgb24 >> 16) & 255; cg = (rgb24 >> 8) & 255; cb = rgb24 & 255;
    }
    for (int c = 0; c < w; c++) {
      int x = x0 + c;
      if (x < 0 || x >= W.w) continue;
      int a = pgm_read_byte(&alpha[r * w + c]);
      if (!a) continue;
      if (k != 255) {
        a = a * k / 255;
        if (!a) continue;
      }
      uint16_t *p = &W.buf[y * W.w + x];
      *p = mix565(*p, cr, cg, cb, a, W.x + x, W.y + y);
    }
  }
}

static void putAlpha(Win &W, int x0, int y0, const uint8_t *alpha, int w, int h,
                     const uint8_t *rowRGB, uint32_t rgb24) {
  putAlphaK(W, x0, y0, alpha, w, h, rowRGB, rgb24, 255);
}

// ===================== MÀN HÌNH KHỞI ĐỘNG =====================
// Lúc bật máy: logo (chiếc lá), tên hệ thống, thanh tiến trình + số % và dòng chữ cho từng bước khởi động.
// Hình / chữ nằm trong boot_bitmaps.h (chỉ là mặt nạ độ đậm), màu tô lúc chạy; nền là dải màu đất tối vẽ bằng code (không tốn bộ nhớ ảnh).
#if BOOT_SCREEN
#define BOOT_CX        160     // tâm logo
#define BOOT_CY         62
#define BOOT_R          34     // bán kính nền tròn của logo
#define BOOT_GLOW       56     // bán kính quầng sáng xanh quanh logo
#define BOOT_TITLE_Y   122     // tên hệ thống
#define BOOT_PCT_Y     160     // hàng số %
#define BOOT_BAR_X      40     // thanh tiến trình
#define BOOT_BAR_W     240
#define BOOT_BAR_Y     192
#define BOOT_BAR_H      10
#define BOOT_LBL_Y     212     // dòng chữ của bước đang chạy
#define BOOT_COL_TITLE 0xFAEBD2
#define BOOT_COL_PCT   0xFFF1DC
#define BOOT_COL_SIGN  0xDEBE9C
#define BOOT_COL_LABEL 0xCEB094
#define BOOT_COL_READY 0x7EE49C   // "Sẵn sàng!"
#define BOOT_COL_LEAF  0xFFFCEC

static int bootPct = 0;           // số % đang hiển thị

// Nền: xanh rêu đậm ở trên -> nâu đất đen ở dưới (cùng tông với trang chủ)
static inline void bootBg(int y, int &r, int &g, int &b) {
  r = 34 + (20 - 34) * y / (UI_H - 1);
  g = 48 + (13 - 48) * y / (UI_H - 1);
  b = 30 + (9 - 30) * y / (UI_H - 1);
}

static void bootFill(Win &W) {
  for (int r = 0; r < W.h; r++) {
    int R, G, B;
    bootBg(W.y + r, R, G, B);
    uint16_t *p = &W.buf[r * W.w];
    for (int c = 0; c < W.w; c++) p[c] = pack565(R, G, B, W.x + c, W.y + r);
  }
}

// Độ phủ 0..1 (khử răng cưa) của hình chữ nhật bo góc [x0,x1] x [y0,y1], bán kính rr, tại điểm (px,py)
static float bootRRect(float px, float py, float x0, float y0, float x1, float y1, float rr) {
  const float hx = (x1 - x0) * 0.5f, hy = (y1 - y0) * 0.5f;
  const float qx = fabsf(px - (x0 + hx)) - (hx - rr);
  const float qy = fabsf(py - (y0 + hy)) - (hy - rr);
  const float ox = qx > 0 ? qx : 0, oy = qy > 0 ? qy : 0;
  const float sd = sqrtf(ox * ox + oy * oy) + fminf(fmaxf(qx, qy), 0.0f) - rr;
  const float cov = 0.5f - sd;
  return cov < 0 ? 0 : (cov > 1 ? 1 : cov);
}

// Quầng sáng + nền tròn xanh của logo, ghép lên một dải của màn hình
static void bootLogo(Win &W) {
  for (int r = 0; r < W.h; r++) {
    const int y = W.y + r, dy = y - BOOT_CY;
    if (dy < -BOOT_GLOW || dy > BOOT_GLOW) continue;
    for (int c = 0; c < W.w; c++) {
      const int x = W.x + c, dx = x - BOOT_CX;
      const int d2 = dx * dx + dy * dy;
      if (d2 > BOOT_GLOW * BOOT_GLOW) continue;
      const float d = sqrtf((float)d2);
      uint16_t *p = &W.buf[r * W.w + c];
      if (d > BOOT_R - 1) {
        const float k = (BOOT_GLOW - d) / (BOOT_GLOW - BOOT_R + 1);          // 1 sát mép tròn -> 0 ở rìa quầng sáng
        *p = mix565(*p, 70, 205, 120, (int)(72.0f * k * k), x, y);
      }
      float cov = BOOT_R + 0.5f - d;
      if (cov > 0) {
        if (cov > 1) cov = 1;
        int t = (dy + BOOT_R) * 255 / (2 * BOOT_R);                          // 0 (trên) .. 255 (dưới)
        if (t < 0) t = 0; else if (t > 255) t = 255;
        *p = mix565(*p, 92 + (42 - 92) * t / 255, 218 + (150 - 218) * t / 255, 140 + (84 - 140) * t / 255, (int)(cov * 255), x, y);
      }
    }
  }
}

// Phần đứng yên: nền + logo + chiếc lá + tên hệ thống, vẽ từng dải (winBuf nhỏ hơn cả màn hình)
static void bootDrawStatic() {
  const int rows = WIN_PX / UI_W;
  for (int y0 = 0; y0 < UI_H; y0 += rows) {
    Win W = { 0, y0, UI_W, (UI_H - y0 < rows) ? (UI_H - y0) : rows, winBuf };
    bootFill(W);
    bootLogo(W);
    putAlpha(W, BOOT_CX - BT_LEAF_W / 2, BOOT_CY - BT_LEAF_H / 2 - y0, BT_LEAF, BT_LEAF_W, BT_LEAF_H, NULL, BOOT_COL_LEAF);
    putAlpha(W, (UI_W - BT_TITLE_W) / 2, BOOT_TITLE_Y - y0, BT_TITLE, BT_TITLE_W, BT_TITLE_H, NULL, BOOT_COL_TITLE);
    winPush(W);
  }
}

// Thanh tiến trình: rãnh nâu + phần đã chạy chuyển từ xanh lá sang xanh chanh, đầu bo tròn
static void bootDrawBar(int pct) {
  const int wx = BOOT_BAR_X - 4, wy = BOOT_BAR_Y - 4, ww = BOOT_BAR_W + 8, wh = BOOT_BAR_H + 8;
  Win W = { wx, wy, ww, wh, winBuf };
  bootFill(W);
  const float x0 = BOOT_BAR_X, x1 = BOOT_BAR_X + BOOT_BAR_W, y0 = BOOT_BAR_Y, y1 = BOOT_BAR_Y + BOOT_BAR_H;
  const float rr = BOOT_BAR_H * 0.5f, fw = BOOT_BAR_W * pct / 100.0f;
  for (int r = 0; r < wh; r++) {
    for (int c = 0; c < ww; c++) {
      const int x = wx + c, y = wy + r;
      const float px = x + 0.5f, py = y + 0.5f;
      const float ct = bootRRect(px, py, x0, y0, x1, y1, rr);
      if (ct <= 0) continue;
      uint16_t *p = &W.buf[r * ww + c];
      *p = mix565(*p, 66, 48, 37, (int)(ct * 255), x, y);                    // rãnh
      if (fw < 1.0f) continue;
      const float cf = bootRRect(px, py, x0, y0, x0 + fw, y1, fminf(rr, fw * 0.5f));
      if (cf <= 0) continue;
      const int t = (int)((px - x0) * 255 / BOOT_BAR_W);
      int R = 61 + (182 - 61) * t / 255, G = 220 + (240 - 220) * t / 255, B = 132 + (106 - 132) * t / 255;
      if (py < y0 + 4) { R += 34; G += 15; B += 22; if (R > 255) R = 255; if (G > 255) G = 255; if (B > 255) B = 255; }   // vệt sáng phía trên
      *p = mix565(*p, R, G, B, (int)(cf * 255), x, y);
    }
  }
  winPush(W);
}

// Số % (chữ số cách đều nên "100%" rộng nhất = 3 chữ số + dấu %)
static void bootDrawPct(int pct) {
  const int ww = 120, wh = 24, wx = UI_W / 2 - ww / 2, wy = BOOT_PCT_Y - 4;
  Win W = { wx, wy, ww, wh, winBuf };
  bootFill(W);
  char s[8];
  snprintf(s, sizeof(s), "%d", pct);
  const int n = (int)strlen(s);
  int x = ww / 2 - (n * BT_DIG_W + BT_PCT_W) / 2;
  for (int i = 0; i < n; i++, x += BT_DIG_W)
    putAlpha(W, x, 4, BT_DIG[s[i] - '0'], BT_DIG_W, BT_NUM_H, NULL, BOOT_COL_PCT);
  putAlpha(W, x, 4, BT_PCT_A, BT_PCT_W, BT_NUM_H, NULL, BOOT_COL_SIGN);
  winPush(W);
}

// Dòng chữ của bước đang chạy: 0 màn hình | 1 cảm biến | 2 Wi-Fi | 3 pin | 4 "Sẵn sàng!"
static void bootLabel(int i) {
  const int ww = 280, wh = 20, wx = UI_W / 2 - ww / 2, wy = BOOT_LBL_Y - 3;
  Win W = { wx, wy, ww, wh, winBuf };
  bootFill(W);
  putAlpha(W, UI_W / 2 - BT_LBL_W[i] / 2 - wx, 3, BT_LBL[i], BT_LBL_W[i], BT_LBL_H, NULL, i == 4 ? BOOT_COL_READY : BOOT_COL_LABEL);
  winPush(W);
}

static void bootBegin() {
  bootPct = 0;
  bootDrawStatic();
  bootDrawBar(0);
  bootDrawPct(0);
  bootLabel(0);
}

// Cho thanh chạy đều tới target (%); chỉ chạy tới, không lùi
static void bootTo(int target) {
  while (bootPct < target) {
    bootPct += 2;
    if (bootPct > target) bootPct = target;
    bootDrawBar(bootPct);
    bootDrawPct(bootPct);
    delay(BOOT_STEP_MS);
  }
}

static void bootEnd() {
  bootTo(100);
  delay(BOOT_HOLD_MS);            // giữ "Sẵn sàng! 100%" một lúc rồi mới vào trang chủ
}
#else
#define bootBegin()     ((void)0)
#define bootLabel(i)    ((void)0)
#define bootTo(p)       ((void)0)
#define bootEnd()       ((void)0)
#endif

// ===================== VÙNG ĐỘ ẨM ĐẤT =====================
// 0 Rất khô (x<20) | 1 Khô (20<=x<30) | 2 Lý tưởng (30<=x<=50) | 3 Ướt (x>50)
const uint8_t ZONE_NONE = 4;      // chỉ số hình "không nhận cảm biến" trong BADGE_SPR / BAR_SPR / SOIL_COL
static uint8_t zoneOf(int p) {
  if (p < 20) return 0;
  if (p < 30) return 1;
  if (p <= 50) return 2;
  return 3;
}

// ===================== TRANG 1 : VẼ CÁC Ô SỐ LIỆU =====================
// Số nhỏ (nhiệt độ / độ ẩm không khí): chữ số + ký hiệu đơn vị, ghép lên nền rồi đẩy một lần (không nháy)
static void drawSmall(int x0, int winW, const char *s, uint32_t rgb24,
                      const uint8_t *dig, const uint8_t *dot,
                      const uint8_t *unit, int uw, int uh, int udx, int udy) {
  Win W = { x0, SM_Y, winW, SM_CELL_H, winBuf };
  winLoad(W);
  int x = 0;
  for (; *s; s++) {
    if (*s >= '0' && *s <= '9') {
      putAlpha(W, x, 0, dig + (*s - '0') * (SM_CELL_W * SM_CELL_H), SM_CELL_W, SM_CELL_H, NULL, rgb24);
      x += SM_CELL_W;
    } else if (*s == '.' && dot) {
      putAlpha(W, x, 0, dot, SM_DOT_W, SM_CELL_H, NULL, rgb24);
      x += SM_DOT_W;
    } else if (*s == '-') {
      putAlpha(W, x, 0, dig + 10 * (SM_CELL_W * SM_CELL_H), SM_CELL_W, SM_CELL_H, NULL, rgb24);
      x += SM_CELL_W;
    }
  }
  putAlpha(W, x + udx, udy, unit, uw, uh, NULL, rgb24);
  winPush(W);
}

// Chuỗi hiển thị của nhiệt độ (1 số lẻ) / độ ẩm không khí (số nguyên); NaN -> "--"
static void fmtTemp(char *buf, size_t n, float v) {
  if (isnan(v)) snprintf(buf, n, "--");
  else          snprintf(buf, n, "%.1f", (double)v);
}
static void fmtHum(char *buf, size_t n, float v) {
  if (isnan(v)) snprintf(buf, n, "--");
  else          snprintf(buf, n, "%.0f", (double)v);
}

void drawTemp() {
  char buf[12];
  fmtTemp(buf, sizeof(buf), temperature);
  drawSmall(TEMP_X, TEMP_WIN_W, buf, TEMP_RGB, &TEMP_DIG[0][0], TEMP_DOT, DEGC_A, DEGC_W, DEGC_H, DEGC_DX, DEGC_DY);
}

void drawHum() {
  char buf[12];
  fmtHum(buf, sizeof(buf), humidity);
  drawSmall(HUM_X, HUM_WIN_W, buf, HUM_RGB, &HUM_DIG[0][0], NULL, HPCT_A, HPCT_W, HPCT_H, HPCT_DX, HPCT_DY);
}

// Số đất lớn (màu đổi theo vùng; không nhận cảm biến -> "--%" màu xám)
void drawSoilValue() {
  const uint8_t *col = SOIL_COL[soilOk ? zoneOf(soilPct) : ZONE_NONE];
  Win W = { SOIL_X, SOIL_Y, SOIL_WIN_W, SOIL_WIN_H, winBuf };
  winLoad(W);
  if (!soilOk) {
    // "--%": hai gạch ngang (ô chữ số số 10) đặt sát nhau, rồi tới dấu %
    putAlpha(W, -7,  0, SOIL_DIG[10], SOIL_CELL_W, SOIL_WIN_H, col, 0);
    putAlpha(W, 25,  0, SOIL_DIG[10], SOIL_CELL_W, SOIL_WIN_H, col, 0);
    putAlpha(W, 64 + SOIL_PCT_DX, SOIL_PCT_DY, SOIL_PCT_A, SOIL_PCT_W, SOIL_PCT_H, col, 0);
  } else if (soilPct >= 100) {
    putAlpha(W, SOIL100_DX, SOIL100_DY, SOIL100_A, SOIL100_W, SOIL100_H, col, 0);
  } else {
    char buf[6];
    int n = snprintf(buf, sizeof(buf), "%d", soilPct);
    int x = 0;
    for (int i = 0; i < n; i++) {
      putAlpha(W, x, 0, SOIL_DIG[buf[i] - '0'], SOIL_CELL_W, SOIL_WIN_H, col, 0);
      x += SOIL_CELL_W;
    }
    putAlpha(W, x + SOIL_PCT_DX, SOIL_PCT_DY, SOIL_PCT_A, SOIL_PCT_W, SOIL_PCT_H, col, 0);
  }
  winPush(W);
}

// Nhãn vùng cạnh chữ "Độ ẩm đất" ("Lỗi" khi không nhận cảm biến)
void drawBadge() {
  blit(BADGE_X, BADGE_Y, BADGE_W, BADGE_H, BADGE_SPR[soilOk ? zoneOf(soilPct) : ZONE_NONE]);
}

// Thanh 4 vùng + chấm báo vị trí độ ẩm đất (chấm chạy trong vùng hiện tại)
void drawBar() {
  if (!soilOk) {                                    // không nhận cảm biến: không vùng nào sáng, không có chấm
    blit(BAR_X, BAR_Y, BAR_W, BAR_H, BAR_SPR[ZONE_NONE]);
    return;
  }
  const uint8_t z = zoneOf(soilPct);
  memcpy_P(barBuf, BAR_SPR[z], sizeof(barBuf));

  static const int V0[4] = { 0, 20, 30, 50 };
  static const int V1[4] = { 20, 30, 50, 100 };
  float f = (float)(soilPct - V0[z]) / (float)(V1[z] - V0[z]);
  if (f < 0) f = 0;
  if (f > 1) f = 1;
  float cx = SEG_X0[z] + f * (SEG_X1[z] - SEG_X0[z]);
  int ix = (int)cx;
  int ph = (int)((cx - ix) * 4.0f);                 // 4 pha lẻ điểm ảnh
  if (ph > 3) ph = 3;
  int mx = ix - MARK_SZ / 2 - BAR_X;                // góc trái trên của chấm, tính trong bộ đệm thanh
  int my = MARK_CY - MARK_SZ / 2 - BAR_Y;
  const uint8_t  *ma = MARK_A[ph];
  const uint16_t *mc = MARK_RGB[ph * 4 + z];
  for (int r = 0; r < MARK_SZ; r++) {
    int y = my + r;
    if (y < 0 || y >= BAR_H) continue;
    for (int c = 0; c < MARK_SZ; c++) {
      int x = mx + c;
      if (x < 0 || x >= BAR_W) continue;
      int a = pgm_read_byte(&ma[r * MARK_SZ + c]);
      if (!a) continue;
      uint16_t m = pgm_read_word(&mc[r * MARK_SZ + c]);
      int mr = (m >> 11) & 31; mr = (mr << 3) | (mr >> 2);
      int mg = (m >> 5) & 63;  mg = (mg << 2) | (mg >> 4);
      int mb = m & 31;         mb = (mb << 3) | (mb >> 2);
      uint16_t *p = &barBuf[y * BAR_W + x];
      *p = mix565(*p, mr, mg, mb, a, BAR_X + x, BAR_Y + y);
    }
  }
  blit(BAR_X, BAR_Y, BAR_W, BAR_H, barBuf);
}

// ===================== TRANG 1 : CÔNG TẮC AUTO / MANUAL =====================
void drawToggle() {
  blit(TG_X, TG_Y, TG_W, TG_H, TG_SPR[tgFrame]);
}

// Đổi chế độ: núm trượt mượt sang phía kia (khung 0 = AUTO ... khung cuối = MANUAL)
void animateToggle(bool wantAuto) {
  const int target = wantAuto ? 0 : TG_FRAMES - 1;
  while (tgFrame != target) {
    tgFrame += (tgFrame < target) ? 1 : -1;
    drawToggle();
    delay(TOGGLE_STEP);
  }
}

// ===================== TRANG 1 : MÁY BƠM =====================
// Tắt = ảnh đứng yên; bật = nước nhỏ giọt (4 khung chạy vòng)
void drawPump() {
  if (pumpOn) blit(PUMP_X, PUMP_Y, PUMP_W, PUMP_H, PUMP_ON[animFrame]);
  else        blit(PUMP_X, PUMP_Y, PUMP_W, PUMP_H, PUMP_OFF);
}

void drawPill() {
  blit(PILL_X, PILL_Y, PILL_W, PILL_H, PILL_SPR[pumpOn ? 1 : 0]);
}

// Dòng phụ dưới chữ "Bơm tưới": "Điều khiển thủ công" / "Tự động theo độ ẩm đất" / "Chưa nhận cảm biến đất"
void drawSubtitle() {
  uint8_t s = !autoMode ? 0 : (soilOk ? 1 : 2);
  blit(SUBT_X, SUBT_Y, SUBT_W, SUBT_H, SUBT_SPR[s]);
}

// Biểu ngữ đỏ "bơm bị khóa" ở mép trên màn hình (vẽ đè lên cả hai trang). Dùng phông có sẵn của thư viện GFX nên chữ không dấu
// (hình có dấu trong bitmaps.h là ảnh dựng sẵn); phần trên cùng của hai trang không có số liệu nào cập nhật nên không bị vẽ đè.
void drawLockBanner() {
  static const char *L1 = "BOM BI KHOA: KIEM TRA BINH NUOC";
  static const char *L2 = "Giu nut PUMP 2 giay de mo khoa";
  tft.fillRect(0, 0, UI_W, 24, 0xDA89);             // đỏ D9534F (RGB565)
  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setTextColor(0xFFFF);
  tft.setCursor((UI_W - 6 * (int)strlen(L1)) / 2, 4);
  tft.print(L1);
  tft.setCursor((UI_W - 6 * (int)strlen(L2)) / 2, 14);
  tft.print(L2);
}

// Vẽ toàn bộ TRANG 1 (thông số)
void renderAll() {
  curBg = BG_FULL;
  blit(0, 0, UI_W, UI_H, BG_FULL);
  drawTemp();
  drawHum();
  drawSoilValue();
  drawBadge();
  drawBar();
  tgFrame = autoMode ? 0 : TG_FRAMES - 1;
  drawToggle();
  drawPump();
  drawPill();
  drawSubtitle();
  if (pumpLockout) drawLockBanner();
}

// ===================== TRANG 0 : TRANG CHỦ =====================
// Màu chữ (RGB 24 bit)
#define COL_SEC       0xDEBE9C   // giây (nhỏ) cạnh đồng hồ
#define COL_SEC_DIM   0x967A66   // "--" khi chưa có giờ
#define COL_WEEKDAY   0xF4E0C4   // "Thứ hai,"
#define COL_DATE      0xCEB094   // "19/10/2026"
#define COL_NOTIME    0xB0927C   // dòng "Đang lấy giờ..."
#define COL_SSID_ON   0xCEB094   // tên Wi-Fi khi đã nối
#define COL_SSID_OFF  0x967E6C   // tên Wi-Fi khi chưa nối
#define COL_BAT_OK    0xFFF1DC   // % pin > 30
#define COL_BAT_LOW   0xFFC45C   // % pin 16..30 (cam)
#define COL_BAT_CRIT  0xFF7A62   // % pin <= 15 (đỏ)
#define COL_BAT_NONE  0xB09682   // không có pin
#define COL_BAT_CHG   0xD8FFE4   // số % khi đang sạc (bạc hà)
#define COL_CHG_RING  0x7EE49C   // viền sáng của thẻ Pin khi sạc
#define COL_CHG_TEXT  0x96F0B2   // nhãn "Đang sạc" / "Đã đầy"
#define COL_SHINE     0xECFFF2   // vệt sáng chạy trên ruột pin
#define COL_BOLT_OUT  0x0E3E24   // viền tối của tia sét
#define COL_BOLT_FILL 0xFFFCEC   // ruột sáng của tia sét

// Đồng hồ: hh < 0 -> chưa có giờ, hiện "--:--" mờ
static void drawClock(int hh, int mm, int ss) {
  Win W = { HCLK_X, HCLK_Y, HCLK_W, HCLK_H, winBuf };
  winLoad(W);
  const bool ok = (hh >= 0);
  const uint8_t *rows = HCLK_COL[ok ? 0 : 1];
  int d[4] = { 10, 10, 10, 10 };                    // 10 = dấu "-"
  int s[2] = { 10, 10 };
  if (ok) {
    d[0] = hh / 10; d[1] = hh % 10; d[2] = mm / 10; d[3] = mm % 10;
    s[0] = ss / 10; s[1] = ss % 10;
  }
  int x = HCLK_GX;
  for (int i = 0; i < 4; i++) {
    if (i == 2) {                                   // dấu ":" giữa giờ và phút
      putAlpha(W, x, 0, HCLK_COLON, HCLK_COLON_W, HCLK_H, rows, 0);
      x += HCLK_COLON_W;
    }
    putAlpha(W, x, 0, HCLK_DIG[d[i]], HCLK_P, HCLK_H, rows, 0);
    x += HCLK_P;
  }
  x += HCLK_SEC_GAP;
  for (int i = 0; i < 2; i++) {                     // giây: số nhỏ, cùng đường chân chữ với giờ
    putAlpha(W, x, HCLK_SEC_Y, TEMP_DIG[s[i]], SM_CELL_W, SM_CELL_H, NULL, ok ? COL_SEC : COL_SEC_DIM);
    x += SM_CELL_W;
  }
  winPush(W);
}

// Dòng ngày: "Thứ hai, 19/10/2026" (t == NULL -> chưa có giờ: hiện lý do)
static void drawDate(const struct tm *t) {
  Win W = { HDATE_X, HDATE_Y, HDATE_W, HDATE_H, winBuf };
  winLoad(W);
  if (t) {
    int wd = t->tm_wday;
    if (wd < 0 || wd > 6) wd = 0;
    int w1 = pgm_read_byte(&HWDAY_W[wd]);
    int total = w1 + HDATE_GAP + 8 * HDATE_DIG_W + 2 * HDATE_SLASH_W;
    int x = (HDATE_W - total) / 2;
    putAlpha(W, x, 0, HWDAY_A[wd], HWDAY_MAXW, HDATE_H, NULL, COL_WEEKDAY);
    x += w1 + HDATE_GAP;
    char s[24];
    snprintf(s, sizeof(s), "%02d/%02d/%04d", t->tm_mday % 100, (t->tm_mon + 1) % 100, (t->tm_year + 1900) % 10000);
    for (const char *p = s; *p; p++) {
      if (*p == '/') {
        putAlpha(W, x, 0, HDATE_SLASH, HDATE_SLASH_W, HDATE_H, NULL, COL_DATE);
        x += HDATE_SLASH_W;
      } else if (*p >= '0' && *p <= '9') {
        putAlpha(W, x, 0, HDATE_DIG[*p - '0'], HDATE_DIG_W, HDATE_H, NULL, COL_DATE);
        x += HDATE_DIG_W;
      }
    }
  } else {
    const int m = (wifiUi == WU_OFF) ? 1 : 0;       // 0: "Đang lấy giờ từ Internet..." | 1: "Chưa có giờ - kiểm tra Wi-Fi"
    int w1 = pgm_read_byte(&HNOTIME_W[m]);
    putAlpha(W, (HDATE_W - w1) / 2, 0, HNOTIME_A[m], HNOTIME_MAXW, HDATE_H, NULL, COL_NOTIME);
  }
  winPush(W);
}

// Đọc giờ hệ thống; vẽ lại đồng hồ khi giây đổi, vẽ lại dòng ngày khi sang ngày mới. force = vẽ lại tất cả.
void updateClock(bool force) {
  struct tm t;
  if (getLocalTime(&t, 0) && t.tm_year >= 120) {    // đã có giờ thật (NTP)
    if (!timeSynced) {
      timeSynced = true;
      force = true;
      Serial.println("GIO: da dong bo tu Internet");
    }
    if (force || t.tm_sec != shownSec) {
      shownSec = t.tm_sec;
      drawClock(t.tm_hour, t.tm_min, t.tm_sec);
    }
    int key = t.tm_year * 366 + t.tm_yday;
    if (force || key != shownDay) {
      shownDay = key;
      drawDate(&t);
    }
  } else if (!timeSynced) {                         // chưa từng có giờ: "--:--"
    if (force || shownSec != -2) {
      shownSec = -2;
      drawClock(-1, 0, 0);
    }
    if (force) drawDate(NULL);
  }
  // (đã có giờ rồi mà một lần đọc hụt -> bỏ qua, giữ nguyên hình đang hiển thị)
}

// ---- font nhỏ (tên Wi-Fi): chữ ASCII 32..126, bề rộng thay đổi theo chữ
static int f9Adv(char c) {
  int g = (unsigned char)c - HSSIDF_FIRST;
  return (g < 0 || g >= HSSIDF_COUNT) ? 0 : pgm_read_byte(&HSSIDF_ADV[g]);
}
static int f9Width(const char *s, int n) {
  int w = 0;
  for (int i = 0; i < n && s[i]; i++) w += f9Adv(s[i]);
  return w;
}
// Vẽ chuỗi vào cửa sổ tại (x,y); dài quá maxW thì cắt bớt và thêm ".."
static void drawF9(Win &W, int x, int y, const char *s, int maxW, uint32_t rgb) {
  int n = strlen(s);
  bool dots = false;
  if (f9Width(s, n) > maxW) {
    dots = true;
    int dw = 2 * f9Adv('.');
    while (n > 0 && f9Width(s, n) + dw > maxW) n--;
  }
  for (int i = 0; i < n + (dots ? 2 : 0); i++) {
    char c = (i < n) ? s[i] : '.';
    int g = (unsigned char)c - HSSIDF_FIRST;
    if (g < 0 || g >= HSSIDF_COUNT) continue;           // ký tự ngoài ASCII (chữ có dấu...) -> bỏ qua
    putAlpha(W, x, y, &HSSIDF_A[pgm_read_word(&HSSIDF_OFF[g])], pgm_read_byte(&HSSIDF_W[g]), HSSIDF_H, NULL, rgb);
    x += pgm_read_byte(&HSSIDF_ADV[g]);
  }
}

// ---- thẻ Wi-Fi: biểu tượng vạch sóng + trạng thái + tên mạng
void drawWifiIcon() {
  uint8_t i;
  if (wifiUi == WU_ON)       i = wifiBars - 1;      // 0..2 : đã nối, 1..3 vạch
  else if (wifiUi == WU_ING) i = 3 + wifiFrame;     // 3..6 : đang nối (hoạt ảnh)
  else                       i = 7;                 // 7    : mất kết nối (gạch chéo đỏ)
  blit(HWIFI_ICON_X, HWIFI_ICON_Y, HWIFI_ICON_W, HWIFI_ICON_H, HWIFI_ICON_SPR[i]);
}

void drawWifi() {
  drawWifiIcon();
  blit(HWIFI_TXT_X, HWIFI_TXT_Y, HWIFI_TXT_W, HWIFI_TXT_H, HWIFI_TXT_SPR[wifiUi == WU_ON ? 0 : (wifiUi == WU_ING ? 1 : 2)]);
  Win W = { HSSID_X, HSSID_Y, HSSID_W, HSSID_H, winBuf };
  winLoad(W);
  drawF9(W, 0, 0, (apOn && wifiUi != WU_ON) ? AP_SSID : wifiSsid, HSSID_W, wifiUi == WU_ON ? COL_SSID_ON : COL_SSID_OFF);   // đang phát Wi-Fi riêng mà chưa nối mạng: hiện tên Wi-Fi riêng để biết mà nối
  winPush(W);
}

// ---- thẻ Pin: vẽ lại CẢ thẻ (cửa sổ HCHG_*) từ nền BG_HOME rồi ghép lên:
//      [đang sạc] viền sáng + nhãn "Đang sạc" | ruột pin (đổi màu theo mức) [+ vệt sáng + sóng sạc] | [tia sét] | số %
// Nhịp "thở" của viền sáng (0..255) theo khung hoạt ảnh: 0.5 - 0.5*cos(2*pi*khung/16)
static const uint8_t CHG_BREATH[CHG_FRAMES] = { 0, 10, 37, 79, 128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10 };

void drawBattery() {
  shownBat = batPct;
  Win W = { HCHG_X, HCHG_Y, HCHG_W, HCHG_H, winBuf };
  winLoad(W);
  const bool chg = (chgState != CHGS_NONE);
  if (chg) {
    // viền thẻ sáng xanh: đang sạc thì "thở" theo khung hoạt ảnh, đầy thì sáng đều
    const int k = (chgState == CHGS_RUN) ? 77 + ((179 * CHG_BREATH[chgFrame]) >> 8) : 140;
    putAlphaK(W, 0, 0, HCHG_RING, HCHG_W, HCHG_H, NULL, COL_CHG_RING, k);
    putAlpha(W, HCHG_CHIP_X - W.x, HCHG_CHIP_Y - W.y, HCHG_CHIP[chgState == CHGS_FULL ? 1 : 0],
             HCHG_CHIP_W, HCHG_CHIP_H, NULL, COL_CHG_TEXT);
  }
  uint32_t rgb = COL_BAT_NONE;
  char s[12] = "--";
  if (batPct >= 0) {
    const uint8_t lvl = chg ? 0 : (batPct > 30 ? 0 : (batPct > 15 ? 1 : 2));   // xanh lá / cam / đỏ ; đang sạc: luôn xanh lá
    rgb = chg ? COL_BAT_CHG : ((lvl == 0) ? COL_BAT_OK : (lvl == 1 ? COL_BAT_LOW : COL_BAT_CRIT));
    snprintf(s, sizeof(s), "%d", batPct);
    // phần pin đầy = mặt nạ ruột pin x độ phủ từng cột (cột biên phủ một phần -> mép mịn)
    uint8_t fa[HBAT_IN_W * HBAT_IN_H];
    const int ix = HBAT_IN_X - W.x, iy = HBAT_IN_Y - W.y;
    const int FW = HBAT_IN_W * 256;                                   // bề rộng ruột pin, đơn vị 1/256 điểm ảnh
    const int fw = FW * batPct / 100;                                 // bề rộng phần đầy
    int cov[HBAT_IN_W];
    for (int c = 0; c < HBAT_IN_W; c++) {
      int v = fw - c * 256;
      cov[c] = (v < 0) ? 0 : (v > 256 ? 256 : v);
    }
    for (int c = 0; c < HBAT_IN_W; c++)
      for (int r = 0; r < HBAT_IN_H; r++)
        fa[r * HBAT_IN_W + c] = (uint8_t)((pgm_read_byte(&HBAT_MASK[r * HBAT_IN_W + c]) * cov[c]) >> 8);
    putAlpha(W, ix, iy, fa, HBAT_IN_W, HBAT_IN_H, HBAT_COL[lvl], 0);

    if (chgState == CHGS_RUN) {
      const int t = chgFrame * 256 / CHG_FRAMES;                      // 0..255: vị trí trong vòng hoạt ảnh
      const int B = 4 * 256;                                          // nửa bề rộng vệt sáng
      const int sc = -B + (FW + 2 * B) * t / 256;                     // tâm vệt sáng: chạy từ trái sang phải
      const int front = fw + (FW - fw) * t / 256;                     // đầu "sóng sạc": lan từ mép phần đầy tới cuối pin
      const int fade = 255 - ((t * t) >> 8);                          // sóng mờ dần trong vòng
      int ha[HBAT_IN_W], wa[HBAT_IN_W];
      for (int c = 0; c < HBAT_IN_W; c++) {
        const int xc = c * 256 + 128;                                 // tâm cột
        int d = xc - sc;
        if (d < 0) d = -d;
        const int hh = (d >= B) ? 0 : 255 - d * 255 / B;
        const int sm = (hh * hh / 255) * (765 - 2 * hh) / 255;        // smoothstep: vệt sáng mềm hai bên
        ha[c] = ((sm * 153) >> 8) * cov[c] >> 8;                      // vệt sáng chỉ nằm trên phần pin đầy
        int w = 0;
        if (batPct < 100 && xc > fw) {                                // sóng sạc nằm trên phần pin còn trống
          int ins = xc - fw + 128; if (ins > 256) ins = 256;
          int soft = (front - xc) * 256 / 384; if (soft < 0) soft = 0; if (soft > 256) soft = 256;
          w = 107 * fade / 255 * soft / 256 * ins / 256 * (256 - cov[c]) / 256;
        }
        wa[c] = w;
      }
      for (int c = 0; c < HBAT_IN_W; c++)
        for (int r = 0; r < HBAT_IN_H; r++)
          fa[r * HBAT_IN_W + c] = (uint8_t)((pgm_read_byte(&HBAT_MASK[r * HBAT_IN_W + c]) * ha[c]) >> 8);
      putAlpha(W, ix, iy, fa, HBAT_IN_W, HBAT_IN_H, NULL, COL_SHINE);
      for (int c = 0; c < HBAT_IN_W; c++)
        for (int r = 0; r < HBAT_IN_H; r++)
          fa[r * HBAT_IN_W + c] = (uint8_t)((pgm_read_byte(&HBAT_MASK[r * HBAT_IN_W + c]) * wa[c]) >> 8);
      putAlpha(W, ix, iy, fa, HBAT_IN_W, HBAT_IN_H, HBAT_COL[lvl], 0);
    }
    if (chg) {                                                        // tia sét trên thân pin (viền tối + ruột sáng)
      putAlphaK(W, HCHG_BOLT_X - W.x, HCHG_BOLT_Y - W.y, HCHG_BOLT_O, HCHG_BOLT_W, HCHG_BOLT_H, NULL, COL_BOLT_OUT, 217);
      putAlpha(W, HCHG_BOLT_X - W.x, HCHG_BOLT_Y - W.y, HCHG_BOLT_F, HCHG_BOLT_W, HCHG_BOLT_H, NULL, COL_BOLT_FILL);
    }
  }
  int x = HBAT_TXT_X - W.x;
  const int y = HBAT_TXT_Y - W.y;
  for (const char *p = s; *p; p++) {
    int d = (*p == '-') ? 10 : (*p - '0');
    putAlpha(W, x, y, HUM_DIG[d], SM_CELL_W, SM_CELL_H, NULL, rgb);
    x += SM_CELL_W;
  }
  putAlpha(W, x + HPCT_DX, y + HPCT_DY, HPCT_A, HPCT_W, HPCT_H, NULL, rgb);
  winPush(W);
}

// ---- thẻ Chế độ: nhãn TỰ ĐỘNG / THỦ CÔNG + giọt nước khi bơm đang chạy
void drawPumpDrop() {
  blit(HDROP_X, HDROP_Y, HDROP_W, HDROP_H, HDROP_SPR[pumpOn ? 1 : 0]);
}

void drawModeCard() {
  blit(HMODE_X, HMODE_Y, HMODE_W, HMODE_H, HMODE_SPR[autoMode ? 0 : 1]);
  drawPumpDrop();
}

// Vẽ toàn bộ TRANG 0 (trang chủ)
void renderHome() {
  curBg = BG_HOME;
  blit(0, 0, UI_W, UI_H, BG_HOME);
  updateClock(true);
  drawWifi();
  drawBattery();
  drawModeCard();
  if (pumpLockout) drawLockBanner();
}

// Đổi trang
void showPage(uint8_t p) {
  page = p;
  if (page == PAGE_DATA) renderAll();
  else                   renderHome();
}

// Rơ-le (đảo mức nếu module relay kích mức thấp)
void relayWrite(bool on) {
  digitalWrite(RELAY_PIN, (on == RELAY_ACTIVE_LOW) ? LOW : HIGH);
}

// ===================== ĐIỀU KHIỂN BƠM / CHẾ ĐỘ =====================
// Mọi thay đổi trạng thái bơm đều đi qua đây: ghi rơ-le + cập nhật màn hình (trang đang xem)
void setPumpState(bool on) {
  if (on == pumpOn) return;
  if (on && pumpLockout) {                          // bơm đang bị khóa: không cho bật (AUTO, nút PUMP và web đều đi qua đây)
    Serial.println("BOM: dang bi KHOA, giu nut PUMP 2 giay hoac bam 'Mo khoa bom' tren web");
    return;
  }
  pumpOn = on;
  relayWrite(on);
  relayChanged = true;
  relayChangedAt = millis();
  if (on) pumpOnSince = relayChangedAt;
  Serial.printf("BOM: %s (%s)\n", on ? "BAT" : "TAT", autoMode ? "AUTO" : "MANUAL");
  animFrame = 0;
  lastAnim = millis();
  if (page == PAGE_DATA) {
    drawPump();
    drawPill();
  } else {
    drawPumpDrop();
  }
}

// Bảo vệ bơm: chạy liên tục quá PUMP_MAX_MS (cạn bình, chạy khô, cảm biến đất kẹt) -> tắt bơm + khóa. Gọi mỗi vòng loop().
void pumpGuard() {
  if (PUMP_MAX_MS == 0 || !pumpOn) return;
  if (millis() - pumpOnSince >= PUMP_MAX_MS) {      // phép trừ không dấu: đúng cả khi millis() tràn
    setPumpState(false);
    pumpLockout = true;
    Serial.printf("CANH BAO: bom chay lien tuc %lu giay -> TAT va KHOA. Kiem tra binh nuoc, roi giu nut PUMP 2 giay (hoac bam 'Mo khoa bom' tren web)\n",
                  (unsigned long)(PUMP_MAX_MS / 1000));
    drawLockBanner();
  }
}

// Người dùng xác nhận đã kiểm tra bình nước: mở khóa (AUTO sẽ tự xét lại độ ẩm ở lần đọc đất kế tiếp)
void unlockPump() {
  if (!pumpLockout) return;
  pumpLockout = false;
  Serial.println("BOM: DA MO KHOA");
  showPage(page);                                   // vẽ lại trang đang xem để xóa biểu ngữ
}

// AUTO: dưới 35% -> bật; từ SOIL_OFF_AT trở lên -> tắt; không nhận cảm biến -> tắt (không tưới "mù").
// respectHold = true (gọi định kỳ): vừa đổi bơm chưa quá AUTO_MIN_HOLD_MS thì giữ nguyên trạng thái (chống bật - tắt dồn dập do nhiễu);
// respectHold = false (vừa đổi sang AUTO / vừa khởi động): áp dụng luật ngay.
void autoControl(bool respectHold) {
  if (!autoMode) return;                            // THỦ CÔNG: không can thiệp
  if (!soilOk) {                                    // mất cảm biến: tắt NGAY, không chờ giữ trạng thái
    if (pumpOn) setPumpState(false);
    return;
  }
  if (pumpLockout) return;                          // bơm đã bị cắt vì chạy quá lâu: chờ người dùng mở khóa
  if (respectHold && relayChanged && millis() - relayChangedAt < AUTO_MIN_HOLD_MS) return;   // giữ >= AUTO_MIN_HOLD_MS (5 s)
  if (!pumpOn && soilPct < SOIL_ON_BELOW) setPumpState(true);            // < 35 %
  else if (pumpOn && soilPct >= SOIL_OFF_AT) setPumpState(false);        // >= 45 %
}

void setMode(bool wantAuto) {
  if (wantAuto == autoMode) return;
  autoMode = wantAuto;
  Serial.printf("CHE DO: %s\n", autoMode ? "AUTO" : "MANUAL");
  if (!autoMode) setPumpState(false);     // vào MANUAL: tắt bơm cho an toàn
  else           autoControl(false);      // vào AUTO : áp dụng ngay luật độ ẩm
  if (page == PAGE_DATA) {
    animateToggle(autoMode);
    drawSubtitle();
  } else {
    drawModeCard();
  }
}

void onPumpButton() {
  if (autoMode) {
    Serial.println("Dang o AUTO: nhan nut MODE de chuyen sang MANUAL truoc");
    return;
  }
  setPumpState(!pumpOn);                            // đang khóa thì setPumpState từ chối bật
}

// ===================== ĐỌC CẢM BIẾN =====================
// Đọc cảm biến đất: 16 mẫu, sắp xếp rồi bỏ 4 mẫu thấp nhất + 4 mẫu cao nhất (xung nhiễu của motor bơm / relay), lấy trung bình 8 mẫu giữa -> raw và phần trăm.
// good = false nếu KHÔNG nhận được cảm biến: raw ngoài dải hợp lệ hoặc 12 mẫu giữa dao động quá mạnh
// (chân ADC bỏ hở / đứt dây / chập).
int readSoilPercent(int &raw, bool &good) {
  int s[16];
  for (int i = 0; i < 16; i++) {
    s[i] = analogRead(SOIL_PIN);
    delayMicroseconds(400);
  }
  for (int i = 1; i < 16; i++) {                    // sắp xếp tăng dần (chèn)
    int v = s[i], j = i - 1;
    while (j >= 0 && s[j] > v) { s[j + 1] = s[j]; j--; }
    s[j + 1] = v;
  }
  long sum = 0;
  for (int i = 4; i < 12; i++) sum += s[i];
  raw = (int)(sum / 8);
  good = (raw >= SOIL_RAW_MIN && raw <= SOIL_RAW_MAX && (s[13] - s[2]) <= SOIL_NOISE_MAX);
  int pct = map(raw, SOIL_DRY, SOIL_WET, 0, 100);
  return constrain(pct, 0, 100);
}

// Điện áp pin (mV) = trung bình 16 mẫu ADC x hệ số cầu phân áp.  < 2500 = không nối pin
static float readBatRaw() {
  long sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += analogReadMilliVolts(BAT_PIN);
    delayMicroseconds(300);
  }
  return (sum / 16.0f) * BAT_DIVIDER;
}

// Pin Li-ion 1 cell: đường cong xả gần đúng, điện áp mV -> %
static int pctFromMv(float mv) {
  static const uint16_t V[] = {3300, 3500, 3600, 3700, 3800, 3900, 4000, 4100, 4200};
  static const uint8_t  P[] = {   0,    5,   15,   30,   50,   65,   78,   90,  100};
  const int N = sizeof(V) / sizeof(V[0]);
  if (mv <= V[0])     return 0;
  if (mv >= V[N - 1]) return 100;
  for (int i = 1; i < N; i++) {
    if (mv <= V[i]) return P[i - 1] + (int)((mv - V[i - 1]) * (P[i] - P[i - 1]) / (V[i] - V[i - 1]));
  }
  return 100;
}

// Gọi ngay khi chgPlugged vừa đổi (cắm / rút sạc), TRƯỚC khi đọc pin
static void onChargeEdge() {
  if (chgPlugged) {                                         // vừa cắm sạc: nhớ điện áp nghỉ trước đó (lấy số thấp hơn trong 2 lần đo gần nhất,
    float r = 0;                                            // phòng khi một lần đo bị đội lên đúng lúc vừa cắm)
    if (batRest1 >= 2500 && batRest2 >= 2500) r = (batRest1 < batRest2) ? batRest1 : batRest2;
    else if (batRest1 >= 2500)                r = batRest1;
    chgRestMv = r;
    chgPlugAt = millis();
    chgOffReady = false;
    chgCv = false;
    batCeil = -1;
  } else {                                                  // vừa rút sạc
    batCeil = batPct;
    batCeilAt = millis();
  }
}

// Đo pin -> batPct.  Không nối pin (< 2.5 V) -> batPct = -1
// fast = true: không lọc làm mượt, lấy số mới ngay (dùng lúc khởi động và lúc vừa cắm/rút sạc)
void readBattery(bool fast) {
  const unsigned long now = millis();
  float mv = readBatRaw();
  if (mv < 2500) { batPct = -1; batMv = 0; batRest1 = batRest2 = 0; return; }       // không nối pin

  if (chgPlugged) {
    // Đang sạc: điện áp đo = điện áp pin thật + mức đội do dòng sạc  ->  phải trừ mức đội đi, và không dùng số đo thô để suy ra "đầy"
    if (!chgOffReady) {                                     // vừa cắm sạc
      if (chgRestMv >= 2500 && now - chgPlugAt < CHARGE_SETTLE_MS) return;          // chờ dòng sạc ổn định, giữ nguyên %
      const float d = mv - chgRestMv;                       // mức đội = điện áp đo bây giờ - điện áp nghỉ trước lúc cắm
      if (chgRestMv < 2500)           chgOffMv = CHARGE_V_OFFSET_MV;                // không biết điện áp nghỉ (bật máy khi đã cắm sạc sẵn)
      else if (mv >= CHARGE_CV_MV)    chgOffMv = constrain(d, 0.0f, 450.0f);        // pin gần đầy: mức đội nhỏ là bình thường
      else                            chgOffMv = (d >= 40 && d <= 450) ? d : CHARGE_V_OFFSET_MV;   // số đo vô lý -> dùng mức mặc định
      chgOffReady = true;
      chgCv = false;
      fast = true;
      chgEma = chgRefMv = mv;
      chgRefAt = now;
    }
    chgEma = 0.9f * chgEma + 0.1f * mv;
    if (now - chgRefAt >= 600000UL) {                       // sau 10 phút điện áp gần như không tăng thêm (< 10 mV) -> mạch sạc đang giữ áp không đổi
      if (!chgCv && chgEma - chgRefMv < 10 && mv >= 4000) { chgCv = true; chgCvAt = now - 300000UL; }
      chgRefMv = chgEma;
      chgRefAt = now;
    }
    if (!chgCv && mv >= CHARGE_CV_MV) { chgCv = true; chgCvAt = now; }
    float v = mv - chgOffMv;                                // giai đoạn dòng không đổi: điện áp pin = đo - mức đội
    if (chgCv) {                                            // giai đoạn áp không đổi: dòng sạc giảm dần theo hàm mũ -> điện áp pin tiến dần về 4.2 V
      float tau = (float)CHARGE_CV_TAU_MS * chgOffMv / 190.0f;                      // mức đội nhỏ (nội trở bé) -> dòng giảm nhanh hơn
      tau = constrain(tau, 600000.0f, 3600000.0f);
      const float el = (float)(now - chgCvAt);
      v = (el >= 2.5f * tau) ? 4200.0f : 4200.0f - chgOffMv * expf(-el / tau);      // sau 2.5 tau dòng còn < 10% (mạch sạc tự ngắt): coi là đầy
    }
    batMv = (fast || batMv < 2500) ? v : (0.7f * batMv + 0.3f * v);
    int p = pctFromMv(batMv);
    if (!fast && p < batPct) p = batPct;                    // đang sạc thì % chỉ tăng, không tụt
    batPct = p;
    return;
  }

  batMv = (fast || batMv < 2500) ? mv : (0.7f * batMv + 0.3f * mv);   // lọc nhiễu: số % không nhảy lung tung
  int p = pctFromMv(batMv);
  if (batCeil >= 0) {                                       // vừa rút sạc: điện áp pin còn dư một lúc, đừng để % cao hơn lúc đang sạc
    if (now - batCeilAt < CHARGE_CEIL_MS) { if (p > batCeil) p = batCeil; }
    else batCeil = -1;
  }
  batPct = p;
  batRest2 = batRest1;
  batRest1 = batMv;
}

// ===================== NHẬN BIẾT ĐANG SẠC =====================
// Cập nhật chgPlugged (có sạc hay không) từ chân CHARGE_PIN. Gọi mỗi vòng loop, tự giới hạn tần suất.
static void updateCharge(unsigned long now) {
  // Đọc chân báo: 5 lần/giây; phải khác trạng thái hiện tại 3 lần LIÊN TIẾP mới đổi (lọc nhiễu)
  if (now - lastChg < 200) return;
  lastChg = now;
  const bool in = (digitalRead(CHARGE_PIN) == HIGH);        // có nguồn sạc -> mức CAO
  if (in != chgPlugged) {
    if (++chgUp >= 3) { chgPlugged = in; chgUp = 0; }
  } else {
    chgUp = 0;
  }
}

// Từ chgPlugged + % pin suy ra trạng thái hiển thị: không sạc / đang sạc / đã đầy
static uint8_t chargeStateNow() {
  if (!chgPlugged || batPct < 0) return CHGS_NONE;          // không cắm sạc, hoặc không nối pin
  return (batPct >= 98 || (chgState == CHGS_FULL && batPct >= 95)) ? CHGS_FULL : CHGS_RUN;   // từ 98% coi là đầy; trễ 3% để không nhấp nháy
}

// Đổi trạng thái hiển thị nếu cần; vẽ lại thẻ Pin (khi đang ở trang chủ)
static void applyChargeState() {
  const uint8_t st = chargeStateNow();
  if (st == chgState) return;
  chgState = st;
  chgFrame = 0;
  lastChgAnim = millis();
  Serial.printf("SAC: %s\n", st == CHGS_RUN ? "DANG SAC" : (st == CHGS_FULL ? "DA DAY (con cam sac)" : "KHONG SAC"));
  if (page == PAGE_HOME) drawBattery();
}

void updateDht() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  // chỉ vẽ lại khi chuỗi hiển thị thay đổi (so sánh đúng chuỗi sẽ hiện: nhiệt độ 1 số lẻ, độ ẩm số nguyên)
  char sOld[12], sNew[12];
  fmtTemp(sOld, sizeof(sOld), temperature); fmtTemp(sNew, sizeof(sNew), t);
  const bool tChanged = (strcmp(sOld, sNew) != 0);
  fmtHum(sOld, sizeof(sOld), humidity);     fmtHum(sNew, sizeof(sNew), h);
  const bool hChanged = (strcmp(sOld, sNew) != 0);
  temperature = t;
  humidity    = h;
  if (page == PAGE_DATA) {                          // trang chủ không hiện nhiệt độ / độ ẩm KK
    if (tChanged) drawTemp();
    if (hChanged) drawHum();
  }
  Serial.printf("DHT11: %.1f C, %.0f %%\n", temperature, humidity);
}

// Đọc đất, theo dõi "có/không có cảm biến", chạy luật AUTO (luôn chạy), vẽ lại phần liên quan khi giá trị đổi.
// Báo MẤT cảm biến sau SOIL_LOST_COUNT lần đọc xấu liên tiếp, báo CÓ LẠI sau SOIL_BACK_COUNT lần đọc tốt liên tiếp
// -> nhiễu thoáng qua không làm màn hình nháy.
void updateSoil() {
  if (relayChanged && millis() - relayChangedAt < SOIL_BLANK_MS) return;   // vừa đóng / ngắt rơ-le: nhiễu + sụt nguồn, bỏ qua lần đọc này
  int raw;
  bool good;
  int pct = readSoilPercent(raw, good);
  int prevPct = soilPct;
  bool prevOk = soilOk;

  if (good) {
    soilBad = 0;
    if (soilOk) {
      soilPct = pct;
    } else if (++soilGood >= SOIL_BACK_COUNT) {
      soilOk = true;
      soilPct = pct;
      soilGood = 0;
    }
  } else {
    soilGood = 0;
    if (soilOk && ++soilBad >= SOIL_LOST_COUNT) {
      soilOk = false;
      soilBad = 0;
    }
  }

  // AUTO: luật 35%/45% (autoControl giữ trạng thái bơm >= AUTO_MIN_HOLD_MS; mất cảm biến thì tắt bơm NGAY, không chờ).
  autoControl(true);

  if (soilOk != prevOk) {                           // vừa mất / vừa có lại cảm biến: vẽ lại phần đất
    Serial.println(soilOk ? "Cam bien dat: DA NHAN LAI"
                          : "CANH BAO: KHONG NHAN CAM BIEN DAT (kiem tra day / chan SOIL_PIN)");
    if (page == PAGE_DATA) {
      drawSoilValue();
      drawBadge();
      drawBar();
      drawSubtitle();
    }
  } else if (soilOk && soilPct != prevPct) {
    if (page == PAGE_DATA) {
      drawSoilValue();
      drawBar();
      if (zoneOf(soilPct) != zoneOf(prevPct)) drawBadge();
    }
  }

  Serial.printf("Dat: raw=%d, %d %%, cam bien=%s, che do=%s, bom=%s\n", raw, soilPct,
                soilOk ? "OK" : "MAT", autoMode ? "AUTO" : "MANUAL", pumpOn ? "BAT" : "TAT");
}

// ===================== WEB (xem số liệu + điều khiển bằng trình duyệt) =====================
// Trang web nằm trong web_page.h và chỉ gọi 4 địa chỉ (cộng nhóm /api/wifi bên dưới):
//   GET  /api/status          -> số liệu hiện tại (JSON)
//   POST /api/mode?auto=1|0   -> TỰ ĐỘNG / THỦ CÔNG   (= bấm nút MODE trên máy)
//   POST /api/pump?on=1|0     -> bật / tắt bơm        (= bấm nút PUMP: chỉ khi THỦ CÔNG; đang TỰ ĐỘNG trả mã 409; bơm đang bị khóa trả mã 423)
//   POST /api/pump/unlock     -> mở khóa bơm          (= nhấn giữ nút PUMP 2 giây khi bơm bị khóa vì chạy quá PUMP_MAX_MS)
// Địa chỉ nào cũng hỏi tên + mật khẩu. Hai lệnh POST còn bắt buộc có header "X-Req: 1" (trang của mình tự gửi);
// trang web lạ mở trên điện thoại trong nhà không gửi được header này tới thiết bị -> không ai bật bơm ngầm được.
WebServer web(80);

static bool webGate(bool isPost) {
  if (!web.authenticate(WEB_USER, WEB_PASS)) {
    web.requestAuthentication(BASIC_AUTH, "Tuoi cay thong minh");
    return false;
  }
  if (isPost && web.header("X-Req") != "1") {
    web.send(403, "text/plain", "Forbidden");
    return false;
  }
  return true;
}

// Trả số liệu hiện tại (JSON) với mã 200 / 409. Không có số liệu (DHT lỗi, không pin) thì ghi null; mất cảm biến đất thì soilOk = 0.
static void webSendStatus(int code) {
  char t[12] = "null", h[12] = "null", b[12] = "null", out[240];
  if (!isnan(temperature)) snprintf(t, sizeof t, "%.1f", temperature);
  if (!isnan(humidity))    snprintf(h, sizeof h, "%.0f", humidity);
  if (batPct >= 0)         snprintf(b, sizeof b, "%d", batPct);
  snprintf(out, sizeof out,
           "{\"temp\":%s,\"hum\":%s,\"soil\":%d,\"soilOk\":%d,\"zone\":%d,\"bat\":%s,\"chg\":%d,"
           "\"auto\":%d,\"pump\":%d,\"rssi\":%d,\"onBelow\":%d,\"offAt\":%d,\"lock\":%d,\"maxS\":%lu}",
           t, h, soilPct, soilOk, soilOk ? zoneOf(soilPct) : ZONE_NONE, b, chgState,
           autoMode, pumpOn, WiFi.RSSI(), SOIL_ON_BELOW, SOIL_OFF_AT, pumpLockout, (unsigned long)(PUMP_MAX_MS / 1000));
  web.sendHeader("Cache-Control", "no-store");
  web.send(code, "application/json", out);
}

static int webFlag(const char *name) {              // tham số lệnh 0 / 1 ; thiếu hoặc sai -> -1
  String v = web.arg(name);
  return (v == "0" || v == "1") ? (v == "1") : -1;
}

static void webRoot()     { if (webGate(false)) web.send_P(200, "text/html; charset=utf-8", WEB_PAGE); }
static void webStatus()   { if (webGate(false)) webSendStatus(200); }
static void webNotFound() { web.send(404, "text/plain", "Not found"); }

static void webMode() {
  if (!webGate(true)) return;
  const int a = webFlag("auto");
  if (a < 0) { web.send(400, "text/plain", "Bad request"); return; }
  setMode(a == 1);                                  // y hệt bấm MODE (đã ở chế độ đó thì không làm gì)
  webSendStatus(200);
}

static void webPump() {
  if (!webGate(true)) return;
  const int on = webFlag("on");
  if (on < 0) { web.send(400, "text/plain", "Bad request"); return; }
  if (autoMode) { webSendStatus(409); return; }     // y hệt nút PUMP: đang TỰ ĐỘNG thì không bật / tắt tay
  if (on == 1 && pumpLockout) { webSendStatus(423); return; }   // bơm đang bị khóa: phải mở khóa trước
  setPumpState(on == 1);
  webSendStatus(200);
}

static void webPumpUnlock() {
  if (!webGate(true)) return;
  unlockPump();                                     // y hệt nhấn giữ PUMP 2 giây (không bị khóa thì không làm gì)
  webSendStatus(200);
}

// ---- đổi Wi-Fi bằng web:  GET /api/wifi (trạng thái) | GET /api/wifi/scan[?new=1] (quét mạng, chạy nền) | POST /api/wifi/save (ssid, pass)
// Cùng bảo vệ như các lệnh khác: hỏi tên + mật khẩu; POST bắt buộc có header "X-Req: 1".
// (Không dùng struct làm tham số hàm: Arduino IDE tự chèn khai báo hàm lên đầu file, trước khi struct được định nghĩa -> lỗi biên dịch)
static char  *jBuf = NULL;
static size_t jCap = 0, jLen = 0;
static void jbegin(char *b, size_t cap) { jBuf = b; jCap = cap; jLen = 0; b[0] = 0; }
static void jput(const char *s) {
  size_t l = strlen(s);
  if (jLen + l + 1 > jCap) l = jCap - jLen - 1;
  memcpy(jBuf + jLen, s, l); jLen += l; jBuf[jLen] = 0;
}
static void jint(int v) { char t[16]; snprintf(t, sizeof t, "%d", v); jput(t); }
static void jstr(const char *s) {                      // chuỗi JSON: thoát dấu " \ và ký tự điều khiển
  jput("\"");
  for (; *s; s++) {
    const unsigned char c = (unsigned char)*s;
    char t[8];
    if (c == '"' || c == '\\') { t[0] = '\\'; t[1] = (char)c; t[2] = 0; }
    else if (c < 0x20)         { snprintf(t, sizeof t, "\\u%04x", c); }
    else                       { t[0] = (char)c; t[1] = 0; }
    jput(t);
  }
  jput("\"");
}

static void webSendWifi(int code) {
  static char buf[800];
  jbegin(buf, sizeof buf);
  const bool conn = (WiFi.status() == WL_CONNECTED);
  jput("{\"ssid\":");   jstr(wifiSsid);
  jput(",\"conn\":");   jint(conn);
  jput(",\"ip\":");     jstr(conn ? WiFi.localIP().toString().c_str() : "");
  jput(",\"ap\":");     jint(apOn);
  jput(",\"apSsid\":"); jstr(AP_SSID);
  jput(",\"apIp\":");   jstr(apOn ? WiFi.softAPIP().toString().c_str() : "");
  jput(",\"trial\":");  jint(wifiTrial);
  jput(",\"trySsid\":"); jstr(wifiTrial ? pendSsid : "");
  jput(",\"host\":");  jstr(WEB_HOST);
  jput("}");
  web.sendHeader("Cache-Control", "no-store");
  web.send(code, "application/json", buf);
}
static void webWifiStatus() { if (webGate(false)) webSendWifi(200); }

// Quét mạng chạy nền (không chặn máy): ?new=1 bắt đầu quét -> {"scanning":1}; hỏi lại không có new: đang quét -> scanning:1, xong -> danh sách mạnh -> yếu
static void webWifiScan() {
  if (!webGate(false)) return;
  static char buf[2600];
  jbegin(buf, sizeof buf);
  const int cnt = WiFi.scanComplete();
  if (web.hasArg("new") && cnt != WIFI_SCAN_RUNNING) {
    WiFi.scanDelete();
    if (WiFi.scanNetworks(true, false) == WIFI_SCAN_FAILED) jput("{\"scanning\":0,\"nets\":[],\"err\":1}");
    else                                                    jput("{\"scanning\":1}");
  } else if (cnt == WIFI_SCAN_RUNNING) {
    jput("{\"scanning\":1}");
  } else if (cnt < 0) {
    jput("{\"scanning\":0,\"nets\":[]}");
  } else {
    int idx[32], m = 0;                                            // mỗi tên một lần (giữ sóng mạnh nhất), bỏ mạng ẩn
    for (int i = 0; i < cnt; i++) {
      const String s = WiFi.SSID(i);
      if (s.length() == 0 || s.length() > 32) continue;
      int d = -1;
      for (int k = 0; k < m; k++) if (WiFi.SSID(idx[k]) == s) { d = k; break; }
      if (d >= 0) { if (WiFi.RSSI(i) > WiFi.RSSI(idx[d])) idx[d] = i; }
      else if (m < 32) idx[m++] = i;
    }
    for (int a = 1; a < m; a++) {                                  // sắp xếp: sóng mạnh trước
      const int v = idx[a]; int b = a - 1;
      while (b >= 0 && WiFi.RSSI(idx[b]) < WiFi.RSSI(v)) { idx[b + 1] = idx[b]; b--; }
      idx[b + 1] = v;
    }
    jput("{\"scanning\":0,\"nets\":[");
    for (int k = 0; k < m && k < 16; k++) {
      if (jLen + 240 > jCap) break;
      if (k) jput(",");
      jput("{\"s\":"); jstr(WiFi.SSID(idx[k]).c_str());
      jput(",\"r\":"); jint((int)WiFi.RSSI(idx[k]));
      jput(",\"l\":"); jput(WiFi.encryptionType(idx[k]) == WIFI_AUTH_OPEN ? "0" : "1");
      jput("}");
    }
    jput("]}");
    WiFi.scanDelete();                                             // giải phóng bộ nhớ kết quả quét
  }
  web.sendHeader("Cache-Control", "no-store");
  web.send(200, "application/json", buf);
}

// Lưu ý: chỉ GHI NHỚ yêu cầu rồi trả lời ngay; việc đổi mạng thật làm ở handleWifi() sau đó (để câu trả lời kịp gửi đi trước khi Wi-Fi đổi)
static void webWifiSave() {
  if (!webGate(true)) return;
  const String s = web.arg("ssid"), p = web.arg("pass");
  if (s.length() < 1 || s.length() > 32 || (p.length() != 0 && (p.length() < 8 || p.length() > 63))) { web.send(400, "text/plain", "Bad request"); return; }
  strlcpy(pendSsid, s.c_str(), sizeof pendSsid);
  strlcpy(pendPass, p.c_str(), sizeof pendPass);
  wifiTrial = 1;
  trialBegun = false;
  Serial.printf("WI-FI: nhan lenh doi sang mang \"%s\"\n", pendSsid);
  webSendWifi(200);
}

static void webAnnounce() {
  if (WiFi.status() == WL_CONNECTED)
    Serial.printf("WEB: http://%s  hoac  http://%s.local   (dang nhap: %s)\n", WiFi.localIP().toString().c_str(), WEB_HOST, WEB_USER);
  if (apOn)
    Serial.printf("WEB (Wi-Fi rieng): noi Wi-Fi \"%s\" (mat khau %s) roi mo http://%s   (dang nhap: %s)\n", AP_SSID, AP_PASS, WiFi.softAPIP().toString().c_str(), WEB_USER);
}

static void webBegin() {
  static const char *keys[] = { "X-Req" };
  web.collectHeaders(keys, 1);                      // mặc định chỉ đọc header Authorization; cần đọc thêm X-Req
  web.on("/", HTTP_GET, webRoot);
  web.on("/api/status", HTTP_GET, webStatus);
  web.on("/api/mode", HTTP_POST, webMode);
  web.on("/api/pump", HTTP_POST, webPump);
  web.on("/api/pump/unlock", HTTP_POST, webPumpUnlock);
  web.on("/api/wifi", HTTP_GET, webWifiStatus);
  web.on("/api/wifi/scan", HTTP_GET, webWifiScan);
  web.on("/api/wifi/save", HTTP_POST, webWifiSave);
  web.onNotFound(webNotFound);
  web.begin();
  webStarted = true;
  webAnnounce();
  if (strcmp(WEB_PASS, "MAT_KHAU_WEB") == 0) Serial.println("CANH BAO: chua doi WEB_PASS (mat khau trang web) o dau file");
}

// ===================== WI-FI =====================
// Số vạch sóng từ RSSI (dBm), có "trễ" 3 dB quanh mỗi ngưỡng để vạch không nhấp nháy:
//   3 vạch: tốt hơn -60 | 2 vạch: -60..-70 | 1 vạch: yếu hơn -70
static uint8_t barsFromRssi(int rssi, uint8_t cur) {
  int t3 = (cur >= 3) ? -63 : -57;
  int t2 = (cur >= 2) ? -73 : -67;
  if (rssi >= t3) return 3;
  if (rssi >= t2) return 2;
  return 1;
}

// Cập nhật trạng thái Wi-Fi hiển thị; vẽ lại thẻ Wi-Fi (chỉ khi đang ở trang chủ) nếu có thay đổi
static void setWifiUi(uint8_t st, uint8_t bars) {
  if (st == wifiUi && (st != WU_ON || bars == wifiBars)) return;
  const bool stateChanged = (st != wifiUi);
  wifiUi = st;
  if (st == WU_ON) wifiBars = bars;
  if (st == WU_ING) wifiFrame = 0;
  if (stateChanged) {
    static const char *NM[] = { "MAT KET NOI", "DANG KET NOI", "DA KET NOI" };
    Serial.printf("WiFi: %s\n", NM[st]);
  }
  if (page == PAGE_HOME) {
    drawWifi();
    if (stateChanged && !timeSynced) drawDate(NULL);   // dòng "Đang lấy giờ..." / "Chưa có giờ - kiểm tra Wi-Fi"
  }
}

// Mạng Wi-Fi đang dùng nằm trong bộ nhớ flash (Preferences, vùng "tuoicay"); chưa lưu thì dùng WIFI_SSID / WIFI_PASS ở đầu file.
static void loadWifi() {
  strlcpy(wifiSsid, WIFI_SSID, sizeof wifiSsid);
  strlcpy(wifiPass, WIFI_PASS, sizeof wifiPass);
  Preferences pr;
  if (pr.begin("tuoicay", false)) {
    char s[33] = "", p[65] = "";
    if (pr.isKey("ssid")) pr.getString("ssid", s, sizeof s);
    if (pr.isKey("pass")) pr.getString("pass", p, sizeof p);
    pr.end();
    if (s[0]) { strlcpy(wifiSsid, s, sizeof wifiSsid); strlcpy(wifiPass, p, sizeof wifiPass); }
  }
}

static void saveWifi() {
  Preferences pr;
  if (pr.begin("tuoicay", false)) { pr.putString("ssid", wifiSsid); pr.putString("pass", wifiPass); pr.end(); }
}

// Wi-Fi riêng (AP) để cài đặt: chạy cùng lúc với việc dò mạng nhà (AP + STA)
static void apStart() {
  if (apOn) return;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.setAutoReconnect(false);                     // đang cài đặt: không để máy tự dò kênh liên tục làm đứt Wi-Fi riêng (vẫn tự thử nối lại mỗi WIFI_RETRY khi chưa có ai nối vào)
  apOn = true;
  if (!webStarted) webBegin();
  Serial.printf("WI-FI CAI DAT: chua noi duoc Wi-Fi nen mo Wi-Fi rieng \"%s\" (mat khau %s)\n", AP_SSID, AP_PASS);
  webAnnounce();
  if (page == PAGE_HOME) drawWifi();
}

static void apStop() {
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  apOn = false;
  Serial.println("WI-FI CAI DAT: da tat Wi-Fi rieng");
  if (page == PAGE_HOME) drawWifi();
}

void handleWifi(unsigned long now) {
  if (now - lastWifi < WIFI_POLL) return;
  lastWifi = now;
  const bool conn = (WiFi.status() == WL_CONNECTED);

  if (wifiTrial == 1) {                               // người dùng vừa chọn mạng mới trên trang web
    if (!trialBegun) {                                // (làm ở đây, sau khi câu trả lời cho trang web đã gửi đi)
      trialBegun = true;
      trialStart = now;
      WiFi.disconnect(false);
      WiFi.begin(pendSsid, pendPass);
    } else if (conn && now - trialStart >= 1500 && strcmp(WiFi.SSID().c_str(), pendSsid) == 0) {   // nối được: LƯU lại
      strlcpy(wifiSsid, pendSsid, sizeof wifiSsid);
      strlcpy(wifiPass, pendPass, sizeof wifiPass);
      saveWifi();
      wifiTrial = 2;
      Serial.printf("WI-FI: da noi va luu mang \"%s\"\n", wifiSsid);
      if (page == PAGE_HOME) drawWifi();
    } else if (now - trialStart >= WIFI_TRIAL_MS) {   // không nối được: bỏ mạng mới, quay lại mạng cũ
      wifiTrial = 3;
      Serial.printf("WI-FI: khong noi duoc \"%s\" (sai mat khau / khong co song) -> quay lai mang cu \"%s\"\n", pendSsid, wifiSsid);
      WiFi.disconnect(false);
      WiFi.begin(wifiSsid, wifiPass);
      wifiTryStart = now;
    }
  }

  if (conn) {
    if (!wifiUpSeen) { wifiUpSeen = true; wifiUpSince = now; }
    if (!ntpStarted) {                                // vừa nối được lần đầu: xin giờ từ Internet
      configTime(GMT_OFFSET_SEC, 0, NTP_SERVER1, NTP_SERVER2);
      ntpStarted = true;
    }
    if (!webStarted) webBegin();                      // nối được Wi-Fi lần đầu: mở trang web
    if (!mdnsOk && (lastMdns == 0 || now - lastMdns >= 10000)) {   // tên http://tuoicay.local (cần đã có mạng; lỗi thì thử lại sau 10 giây)
      lastMdns = now ? now : 1;
      mdnsOk = MDNS.begin(WEB_HOST);
      if (mdnsOk) MDNS.addService("http", "tcp", 80);
    }
    if (now - lastWebMsg >= WEB_MSG_EVERY) {          // nhắc địa chỉ trang web (đỡ lỡ khi mở Serial Monitor muộn)
      lastWebMsg = now;
      webAnnounce();
    }
    if (apOn && wifiTrial != 1 && now - wifiUpSince >= AP_KEEP_MS && WiFi.softAPgetStationNum() == 0) apStop();   // đã có mạng nhà lại và không ai dùng Wi-Fi riêng nữa
    wifiWasOn = true;
    setWifiUi(WU_ON, barsFromRssi(WiFi.RSSI(), wifiBars));
  } else {
    wifiUpSeen = false;
    if (wifiWasOn) {                                  // vừa rớt mạng: đếm lại thời gian "đang kết nối"
      wifiWasOn = false;
      wifiTryStart = now;
    }
    if (now - wifiTryStart < WIFI_GIVEUP) {
      setWifiUi(WU_ING, 0);
    } else {
      setWifiUi(WU_OFF, 0);
      if (now - wifiRetry >= WIFI_RETRY && wifiTrial != 1 && !(apOn && WiFi.softAPgetStationNum() > 0)) {   // vẫn tự thử nối lại định kỳ (không làm phiền người đang cài đặt)
        wifiRetry = now;
        WiFi.disconnect();
        WiFi.begin(wifiSsid, wifiPass);
      }
    }
    if (!apOn && now - wifiTryStart >= AP_AFTER_MS) apStart();   // mất mạng quá lâu: mở Wi-Fi riêng để chọn mạng khác
  }
}

// ===================== NÚT BẤM =====================
bool pressed(Btn &b) {
  const unsigned long now = millis();               // lấy thời gian MỘT lần cho cả hàm
  const bool r = digitalRead(b.pin);
  if (r != b.last && now - b.t > BTN_DEBOUNCE) {
    b.t = now;                                      // b.t = lúc nút đổi trạng thái gần nhất (nhấn xuống hoặc nhả ra)
    b.last = r;
    if (r == LOW) return true;                       // nhấn xuống
  }
  return false;
}

void handleButtons() {
  if (pressed(btnPage)) showPage(page == PAGE_HOME ? PAGE_DATA : PAGE_HOME);
  if (pressed(btnMode)) setMode(!autoMode);
  if (pressed(btnPump)) onPumpButton();

  // Nhấn GIỮ PUMP (đủ PUMP_UNLOCK_HOLD_MS) khi bơm đang bị khóa -> mở khóa; mỗi lần giữ chỉ mở một lần
  static bool holdDone = false;
  if (btnPump.last == LOW) {                         // nút đang được giữ (đã qua lọc rung)
    if (pumpLockout && !holdDone && millis() - btnPump.t >= PUMP_UNLOCK_HOLD_MS) {
      holdDone = true;
      unlockPump();
    }
  } else {
    holdDone = false;
  }
}

#if TFT_COLOR_TEST
// Ô thử màu (chỉ khi TFT_COLOR_TEST = 1): ĐỎ / XANH LÁ / XANH DƯƠNG / TRẮNG / ĐEN
static void colorTest() {
  static const uint16_t cc[5] = { 0xF800, 0x07E0, 0x001F, 0xFFFF, 0x0000 };
  static const char *nm[5] = { "RED", "GREEN", "BLUE", "WHITE", "BLACK" };
  tft.setTextSize(2);
  for (int i = 0; i < 5; i++) {
    tft.fillRect(i * 64, 0, 64, 240, cc[i]);
    tft.setTextColor(i == 3 ? 0x0000 : 0xFFFF);     // chữ đen trên ô trắng, chữ trắng trên các ô còn lại
    tft.setCursor(i * 64 + 4, 112);
    tft.print(nm[i]);
  }
  delay(6000);
}
#endif

// ===================== SETUP / LOOP =====================
#if defined(ARDUINO_ARCH_ESP32)
// Vì sao máy vừa khởi động lại? (in lên Serial Monitor; SUT NGUON = điện áp tụt, thường do bơm / relay khởi động)
static void printResetReason() {
  const char *t;
  switch (esp_reset_reason()) {
    case ESP_RST_POWERON:   t = "bat nguon"; break;
    case ESP_RST_EXT:       t = "nut reset / chan EN"; break;
    case ESP_RST_SW:        t = "phan mem (restart)"; break;
    case ESP_RST_PANIC:     t = "LOI CHUONG TRINH (panic)"; break;
    case ESP_RST_INT_WDT:
    case ESP_RST_TASK_WDT:
    case ESP_RST_WDT:       t = "WATCHDOG (chuong trinh bi treo)"; break;
    case ESP_RST_DEEPSLEEP: t = "thuc day tu deep sleep"; break;
    case ESP_RST_BROWNOUT:  t = "SUT NGUON (brownout): nguon 5V / 3V3 tut, thuong do bom hoac relay khoi dong -> xem huong dan chong nhieu o dau file"; break;
    default:                t = "khac"; break;
  }
  Serial.printf("KHOI DONG LAI: %s\n", t);
}
#endif

void setup() {
  Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32)
  printResetReason();
#endif

  pinMode(RELAY_PIN, OUTPUT);
  relayWrite(false);
  pinMode(PAGEBTN_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(PUMPBTN_PIN, INPUT_PULLUP);
  pinMode(CHARGE_PIN, INPUT);                       // KHÔNG bật điện trở kéo nội (~45k, song song với trở 33k -> điện áp tụt thấp); trở 33k của cầu phân áp đã kéo chân xuống khi rút sạc

  analogReadResolution(12);

  // ---- Bước 0: màn hình (bơm đã tắt từ dòng đầu; từ đây màn hình khởi động chạy cùng các bước thật bên dưới)
  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
  tft.init(240, 320);
  tft.setSPISpeed(40000000);
  tft.invertDisplay(TFT_INVERT);                    // màu sai? xem hướng dẫn ở đầu file (TFT_INVERT / TFT_BGR)
  tft.setRotation(TFT_ROTATION);
#if TFT_BGR
  {
    uint8_t madctl = ((TFT_ROTATION == 3) ? 0x60 : 0xA0) | 0x08;   // giữ hướng xoay, bật bit BGR
    tft.sendCommand(0x36, &madctl, 1);                              // 0x36 = MADCTL
  }
#endif
#if TFT_COLOR_TEST
  colorTest();
#endif
  bootBegin();                                      // hiện logo + thanh tiến trình 0%
  bootTo(15);

  // ---- Bước 1: cảm biến
  bootLabel(1);
  dht.begin();
  // Kiểm tra cảm biến đất ngay lúc khởi động (thử tối đa 3 lần cho chắc)
  int raw, p = 0;
  bool good = false;
  for (int i = 0; i < 3 && !good; i++) {
    p = readSoilPercent(raw, good);
    if (!good) delay(30);
  }
  soilOk = good;
  if (good) soilPct = p;
  else Serial.println("CANH BAO: KHONG NHAN CAM BIEN DAT (kiem tra day / chan SOIL_PIN)");
  bootTo(45);

  // ---- Bước 2: Wi-Fi (kết nối chạy nền, không chờ)
  bootLabel(2);
  WiFi.setHostname(WEB_HOST);                       // tên trong danh sách thiết bị của router (đặt trước WiFi.mode)
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  loadWifi();                                       // mạng đã lưu trong flash (chưa có thì WIFI_SSID / WIFI_PASS)
  WiFi.begin(wifiSsid, wifiPass);                   // không chờ: kết nối nền, giao diện vẫn chạy
  wifiTryStart = millis();
  bootTo(70);

  // ---- Bước 3: pin
  bootLabel(3);
  chgPlugged = (digitalRead(CHARGE_PIN) == HIGH);   // đang sạc ngay từ lúc bật nguồn?
  readBattery(true);
  chgState = chargeStateNow();                      // (chưa vẽ: showPage bên dưới sẽ vẽ cả thẻ Pin)
  bootTo(90);

  // ---- Xong
  bootLabel(4);
  bootEnd();                                        // chạy nốt tới 100%, giữ "Sẵn sàng!" một lúc
  showPage(PAGE_HOME);                              // mở máy vào TRANG CHỦ
  autoControl(false);                               // AUTO: áp dụng luật độ ẩm ngay từ đầu (có thể bật bơm)
}

void loop() {
  unsigned long now = millis();

  handleButtons();
  pumpGuard();                                       // bơm chạy quá PUMP_MAX_MS -> tắt + khóa
  handleWifi(now);
  if (webStarted) web.handleClient();                // trang web: xử lý yêu cầu của điện thoại / máy tính

  if (now - lastDht >= DHT_INTERVAL) {
    lastDht = now;
    updateDht();
  }

  if (now - lastSoil >= SOIL_INTERVAL) {
    lastSoil = now;
    updateSoil();
  }

  {
    const bool wasPlugged = chgPlugged;
    updateCharge(now);                              // cắm / rút sạc?
    if (chgPlugged != wasPlugged) {                 // vừa đổi: tính lại % ngay (có / không bù sụt áp) rồi cập nhật hiệu ứng
      lastBat = now;
      onChargeEdge();                               // nhớ điện áp nghỉ (lúc cắm) / % lúc đang sạc (lúc rút)
      readBattery(true);
      applyChargeState();
      if (page == PAGE_HOME && batPct != shownBat) drawBattery();
    }
  }

  // Không đo pin đúng lúc chân báo sạc vừa đổi mà chưa kịp xác nhận (3 lần liên tiếp): điện áp vừa bị đội lên sẽ làm % nhảy rồi lại về
  if (now - lastBat >= BAT_INTERVAL &&
      ((chgUp == 0 && (digitalRead(CHARGE_PIN) == HIGH) == chgPlugged) || now - lastBat >= 3 * BAT_INTERVAL)) {
    lastBat = now;
    readBattery(false);
    applyChargeState();                             // % chạm / rời 100% có thể đổi "Đang sạc" <-> "Đã đầy"
    if (page == PAGE_HOME && batPct != shownBat) drawBattery();
  }

  if (page == PAGE_HOME) {
    if (now - lastClk >= CLOCK_POLL) {              // kiểm tra giờ 5 lần/giây, chỉ vẽ khi giây đổi
      lastClk = now;
      updateClock(false);
    }
    if (wifiUi == WU_ING && now - lastWifiAnim >= WIFI_ANIM) {   // vạch sóng chạy khi đang nối Wi-Fi
      lastWifiAnim = now;
      wifiFrame = (wifiFrame + 1) & 3;
      drawWifiIcon();
    }
    if (chgState == CHGS_RUN && now - lastChgAnim >= CHARGE_ANIM) {   // hiệu ứng sạc: viền "thở" + vệt sáng chạy
      lastChgAnim = now;
      chgFrame = (chgFrame + 1) % CHG_FRAMES;
      drawBattery();
    }
  } else if (pumpOn && now - lastAnim >= ANIM_INTERVAL) {
    lastAnim = now;
    animFrame = (animFrame + 1) % PUMP_FRAMES;
    drawPump();
  }
}
