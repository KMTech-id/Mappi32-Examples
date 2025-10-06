#ifndef CONFIG_H // Header guard untuk mencegah file ini di-include lebih dari sekali
#define CONFIG_H

// --- Include semua library yang dibutuhkan ---
#include <Wire.h>                   // Untuk komunikasi I2C dasar
#include <Adafruit_SSD1306.h>       // Library untuk layar OLED SSD1306
#include "MAX30100_PulseOximeter.h" // Library untuk sensor detak jantung & SpO2
#include <Adafruit_MLX90614.h>      // Library untuk sensor suhu non-kontak
#include <WiFi.h>                   // Library untuk fungsionalitas WiFi ESP32
#include <HTTPClient.h>             // Library untuk membuat request HTTP (mengirim data)
#include <WebServer.h>              // Library untuk membuat web server (untuk halaman konfigurasi)
#include <Preferences.h>            // Library untuk menyimpan data secara permanen di ESP32

// --- Konfigurasi WiFi & ThingSpeak ---
String ssid;                     // Variabel untuk menyimpan nama WiFi (SSID)
String password;                 // Variabel untuk menyimpan password WiFi
String apiKey;                   // Variabel untuk menyimpan API Key ThingSpeak
const char* server = "http://api.thingspeak.com/update"; // Alamat server ThingSpeak

// --- Konfigurasi OLED ---
#define SCREEN_WIDTH 128 // Lebar layar dalam pixel
#define SCREEN_HEIGHT 64 // Tinggi layar dalam pixel
#define OLED_RESET -1    // Pin reset OLED (-1 jika berbagi pin reset dengan mikrokontroler)

// --- Pin Tombol ---
#define BTN_NEXT   19 // Pin untuk tombol Next
#define BTN_PREV   18 // Pin untuk tombol Previous
#define BTN_SELECT 5  // Pin untuk tombol Select/OK
#define BTN_BACK   23 // Pin untuk tombol Back/Cancel

// --- State Machine (Mesin Keadaan) ---
// Enum untuk mendefinisikan semua kemungkinan "state" atau "mode" dari perangkat.
// Ini membantu mengelola alur program dengan rapi.
enum State {
  HOME,              // State awal / menu utama
  WIFI_CONFIG,       // State untuk menu konfigurasi WiFi
  THINGSPEAK_CONFIG, // State untuk menu konfigurasi ThingSpeak
  AP_MODE_SCREEN,    // State saat perangkat menjadi Access Point (hotspot)
  SELECT_ID,         // State untuk memilih ID pasien
  WAIT_FOR_FINGER,   // State menunggu jari diletakkan di sensor
  COUNTDOWN_HR,      // State hitung mundur sebelum mengukur detak jantung
  MEASURE_HR,        // State saat mengukur detak jantung & SpO2
  WAIT_FOR_TEMP_POS, // State menunggu sensor suhu diarahkan dengan benar
  COUNTDOWN_TEMP,    // State hitung mundur sebelum mengukur suhu
  MEASURE_TEMP,      // State saat mengukur suhu
  SHOW_RESULT,       // State menampilkan hasil akhir pengukuran
  ERROR_STATE        // State jika terjadi error sensor
};
extern State state; // Deklarasi variabel state global (didefinisikan di .ino)

// Variabel untuk memberitahu web server mode mana yang aktif (WiFi atau ThingSpeak)
extern String configMode;

// --- Pengaturan Waktu (dalam milidetik) ---
const unsigned long countdownTime = 3000;      // Durasi hitung mundur: 3 detik
const unsigned long measureTime   = 10000;     // Durasi pengambilan data: 10 detik
const unsigned long debounceDelay = 200;       // Jeda untuk anti-bouncing tombol
const unsigned long maxNoReadTime = 5000;      // Batas waktu jika sensor tidak merespons: 5 detik
const unsigned long tempPosStableTime = 1500;  // Waktu stabilisasi posisi sensor suhu: 1.5 detik
const unsigned long fingerStableTime = 1000;   // Waktu stabilisasi jari di sensor: 1 detik

// --- Objek Sensor & Variabel Global (dideklarasikan sebagai extern) ---
// 'extern' berarti variabel/objek ini ada, tapi didefinisikan di file lain (SmartHealth.ino)
extern Adafruit_SSD1306 display; // Objek untuk layar OLED
extern PulseOximeter pox;        // Objek untuk sensor MAX30100
extern TwoWire WireMLX;          // Objek I2C kedua khusus untuk sensor MLX90614
extern Adafruit_MLX90614 mlx;    // Objek untuk sensor MLX90614
extern WebServer webServer;      // Objek untuk web server
extern Preferences preferences;  // Objek untuk menyimpan preferensi
extern int btnPins[4];           // Array pin tombol
extern int currentID;            // ID pasien yang sedang dipilih
extern float lastHeartRate;      // Nilai detak jantung terakhir
extern float lastSpO2;           // Nilai SpO2 terakhir
extern float lastTemp;           // Nilai suhu terakhir
extern unsigned long stateStart; // Waktu (millis) saat sebuah state dimulai
extern float hrSum;              // Jumlah total pembacaan HR (untuk dirata-rata)
extern int hrCount;              // Jumlah pembacaan HR yang valid
extern float spo2Sum;            // Jumlah total pembacaan SpO2
extern int spo2Count;            // Jumlah pembacaan SpO2 yang valid
extern float tempSum;            // Jumlah total pembacaan suhu
extern int tempCount;            // Jumlah pembacaan suhu yang valid
extern unsigned long lastBtnPressTime[4]; // Waktu terakhir setiap tombol ditekan (untuk debounce)
extern int i2cErrorCount;        // Penghitung error I2C
extern const int maxI2cErrors;   // Batas maksimal error I2C sebelum masuk ERROR_STATE
extern unsigned long lastSuccessfulRead; // Waktu terakhir pembacaan sensor berhasil
extern unsigned long tempPosFirstGoodTime; // Waktu pertama kali posisi sensor suhu dianggap baik
extern unsigned long fingerFirstGoodTime;  // Waktu pertama kali jari di sensor dianggap baik

#endif // CONFIG_H