
# 🗜️ Huffman-Coding
Triển khai thuật toán **Huffman Coding** cô đọng, dễ mở rộng và kèm sẵn công cụ dòng lệnh. Dự án nhằm minh hoạ cách nén dữ liệu dựa trên tần suất ký tự, giúp bạn:
* Tăng tốc độ truyền/ghi file  
* Hiểu rõ cấu trúc cây Huffman  
* Thử nghiệm, benchmark và trực quan hóa

<p align="center">
  <img src="https://img.shields.io/github/languages/top/nightshade94/huffman-coding?color=%2318b4f5"/>
  <img src="https://img.shields.io/github/license/nightshade94/huffman-coding"/>
  <img src="https://img.shields.io/github/stars/nightshade94/huffman-coding?style=social"/>
</p>

---

## ✨ Điểm nổi bật
1. **Thuần C/C++** - không phụ thuộc thư viện ngoài  
2. **Tốc độ O(n log n)** với heap nhị phân tự xây  
3. **API rõ ràng**: encode/decode chuỗi, file, luồng  
4. **Bài test đầy đủ** bằng GoogleTest  
5. **CLI đa nền tảng**: Windows · Linux · macOS  

## 📂 Cấu trúc
```
huffman-coding/
├─ include/           // Header công khai
│  └─ huffman.hpp
├─ src/               // Triển khai chi tiết
├─ cli/               // Mã nguồn tiện ích dòng lệnh
├─ tests/             // Unit-test & benchmark
└─ examples/          // Mẫu sử dụng nhanh
```

## 🚀 Cài đặt
```bash
# Clone
git clone https://github.com/nightshade94/huffman-coding.git
cd huffman-coding

# Biên dịch (sử dụng CMake)
cmake -B build
cmake --build build
```

Hoặc thêm vào dự án CMake hiện có:
```cmake
add_subdirectory(external/huffman-coding)
target_link_libraries(my_app PRIVATE huffman)
```

## 🔧 Sử dụng nhanh

### 1. API C++
```cpp
#include "huffman.hpp"

std::string data = "MISSISSIPPI";
auto compressed = huffman::encode(data);
auto original   = huffman::decode(compressed);

assert(original == data);
```

### 2. Dòng lệnh
```bash
# Nén
huff encode input.txt output.huff

# Giải nén
huff decode output.huff restored.txt
```

Thêm flag `-v` để xem thống kê tỉ lệ nén và tốc độ xử lý.

## 📊 Benchmark
| Dataset | Kích thước gốc | Kích thước nén | Tỉ lệ | Thời gian |
|---------|----------------|----------------|--------|-----------|
| book.txt| 4.8 MB         | 2.1 MB         | 56%    | 38 ms     |
| lorem   | 500 KB         | 210 KB         | 42%    | 4 ms      |

*(CPU: Intel i5-1135G7, g++-12 -O3)*

## 🛠️ Đóng góp
1. Fork và tạo nhánh tính năng: `git checkout -b feature/cool-idea`
2. Commit có ý nghĩa: `git commit -m "Thêm hỗ trợ bitstream"`  
3. Push và mở Pull Request  

Hãy bảo đảm `clang-format` pass và tất cả test xanh trước khi gửi PR nhé!

## 📜 Giấy phép
Trước khi sử dụng các phần trong trình bày trên, hãy gửi email xin phép đến nightshade24.dev@gmail.com

## 💡 Nguồn cảm hứng
Thuật toán nguyên bản bởi David A. Huffman (1952). Tham khảo thêm:
* “A Method for the Construction of Minimum-Redundancy Codes” – D. A. Huffman  
* Wikipedia: “Huffman Coding”

> Nếu dự án hữu ích, hãy ⭐ trên GitHub – động lực tuyệt vời cho tác giả!

---

**Have fun compressing!** 🥳
