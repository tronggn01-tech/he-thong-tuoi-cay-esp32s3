#!/bin/sh
# Chạy các bộ kiểm thử logic (mô phỏng trên máy tính, không cần mạch).
# Cần g++ (hỗ trợ C++17). Đặt thư mục kiem_thu này NGANG HÀNG hoặc BÊN TRONG thư mục sketch có
# tuoi_cay.ino, web_page.h, bitmaps.h, boot_bitmaps.h; sửa SKETCH_DIR nếu cần.
SKETCH_DIR="${1:-..}"
set -e
for t in test_pump test_t52 test_json; do
  g++ -std=gnu++17 -I mock -I "$SKETCH_DIR" -DSKETCH="\"$SKETCH_DIR/tuoi_cay.ino\"" $t.cpp -o $t
  echo "=== $t ==="
  ./$t | grep -E "FAIL|lỗi|loi|JSON=" || true
done
