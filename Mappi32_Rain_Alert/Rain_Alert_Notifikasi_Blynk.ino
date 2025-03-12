#define BLYNK_TEMPLATE_ID "TMPL6PnA31vwF"
#define BLYNK_TEMPLATE_NAME "Notifikasi Sensor Hujan"
#define BLYNK_AUTH_TOKEN "RdyT0e5AXqtUA30S4w0SdxygQ0zQaV-m"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Tambahkan library untuk OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // Lebar OLED
#define SCREEN_HEIGHT 64  // Tinggi OLED
#define OLED_RESET -1     // Reset pin (tidak digunakan)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RAINDO_PIN 16 // Pin sensor hujan ke GPIO 16

// Masukkan Auth Token Blynk
char auth[] = "RdyT0e5AXqtUA30S4w0SdxygQ0zQaV-m";

// Masukkan SSID dan Password WiFi
char ssid[] = "matahari";
char pass[] = "mentarirhaa";

BlynkTimer timer;
int lastStatus = -1; // Variabel untuk menyimpan status terakhir

void hujanNotify() {
    int statusHujan = digitalRead(RAINDO_PIN);
    
    if (statusHujan != lastStatus) { // Hanya perbarui jika status berubah
        Serial.print("Nilai Sensor: "); Serial.println(statusHujan);
        display.clearDisplay();
        display.setCursor(0, 10);
        display.setTextSize(2);

        if (statusHujan == LOW) {  
            Serial.println("Hujan Terdeteksi!");
            Blynk.logEvent("hujan", "Awas Hujan !!!");
            display.println("HUJAN!!!");
        } 
        else {
            Serial.println("Tidak Ada Hujan.");
            Blynk.logEvent("cerah", "Cuaca Cerah");
            display.println("CERAH :)");
        } 

        display.display();
        lastStatus = statusHujan; // Simpan status terbaru
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(RAINDO_PIN, INPUT);
    
    // Inisialisasi OLED
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Alamat I2C OLED 0x3C
        Serial.println(F("OLED tidak ditemukan"));
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.println("Menunggu..."); // Default tampilan awal
    display.display();
    
    // Hubungkan ke Blynk
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

    // Jalankan pengecekan setiap 2,5 detik
    timer.setInterval(2500L, hujanNotify);
}

void loop() {
    Blynk.run();
    timer.run();
}