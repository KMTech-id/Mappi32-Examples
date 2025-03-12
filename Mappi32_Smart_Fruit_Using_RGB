#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_TCS34725.h"

// Konfigurasi OLED SSD1306
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Inisialisasi sensor TCS34725
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

void setup() {
    Serial.begin(115200);
    Wire.begin();  // Gunakan default I2C ESP32

    // Inisialisasi OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F(" OLED tidak terdeteksi!"));
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println("OLED Ready!");
    display.display();
    delay(1000);
    
    // Inisialisasi sensor TCS34725
    if (!tcs.begin()) {
        Serial.println(" TCS34725 tidak terdeteksi! Periksa koneksi.");
        while (1);
    }
    Serial.println(" TCS34725 Siap!");
}

// Fungsi untuk mendeteksi warna buahnya edit aja manggonya sesuai dengan jenis buah yg ingin diuji
String detectManggoRipeness(uint8_t r, uint8_t g, uint8_t b) {
    // *Batas warna berdasarkan pengujian*
    // edit data kalian disini
    if (r > 180 && g < 100 && b < 100) return " Matang"; 
    if (r > 150 && g > 100 && b < 80) return " Setengah Matang";
    if (r < 130 && g > 120 && b > 90) return " Belum Matang";
    return " Coba lagi";
}

//perhatikan lagi kurang dari dan lebih darinya yaa

void loop() {
    uint16_t r, g, b, c;
    tcs.getRawData(&r, &g, &b, &c);

    if (c == 0) c = 1; // Mencegah pembagian dengan nol

    // Normalisasi nilai RGB ke 0-255
    uint8_t r_norm = (r * 255) / c;
    uint8_t g_norm = (g * 255) / c;
    uint8_t b_norm = (b * 255) / c;
//jangan lupa edit sting disini juga sesuai nama buah yg akan diuji
    String manggoStatus = detectManggoRipeness(r_norm, g_norm, b_norm);

    Serial.print("RGB Normalized: ");
    Serial.print(r_norm); Serial.print(", ");
    Serial.print(g_norm); Serial.print(", ");
    Serial.print(b_norm); Serial.print(" → ");
//edit disini juga sesuai nama buah yg ingin di uji
    Serial.print("Status Manggo: ");
    Serial.println(manggoStatus);

    // Tampilkan di OLED
    display.clearDisplay();
  //tampilan display buah pada oled  
    display.setTextSize(2);
    display.setCursor(10, 5);
    display.println("Apel:");
//memanggil data yg akan ditampilkan
    display.setTextSize(2);
    display.setCursor(10, 25);
    display.println(manggoStatus);
// menampilkan nilai r, g , b 
    display.setTextSize(1);
    display.setCursor(10, 50);
    display.print("R: "); display.print(r_norm);
    display.print(" G: "); display.print(g_norm);
    display.print(" B: "); display.print(b_norm);
    
    display.display();

    delay(1000);
}
