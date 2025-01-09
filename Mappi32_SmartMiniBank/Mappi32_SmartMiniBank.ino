#include <LiquidCrystal_I2C.h>  // Library untuk LCD dengan I2C

// Definisikan pin
const int sensorPin = 25;       // Pin sensor IR untuk deteksi koin
const int buttonPin1 = 26;      // Pin tombol pertama untuk memilih jenis mata uang

// Definisikan LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat LCD I2C, kolom dan baris

// Array nilai nominal koin yang valid
const int coinValues[] = {100, 500, 1000, 5000, 10000};
int coinIndex = 0;  // Indeks awal
int coinValue = coinValues[coinIndex];  // Nilai koin saat ini

// Variabel untuk menghitung jumlah koin dan total uang
long totalAmount = 0;  // Jumlah total uang

// Variabel untuk debouncing tombol dan sensor
unsigned long lastButtonPress1 = 0;
unsigned long lastSensorTrigger = 0;
unsigned long debounceDelay = 300;  // Waktu debounce tombol (300 ms)
unsigned long sensorDelay = 500;   // Waktu untuk mencegah pembacaan ganda sensor (500 ms)

void setup() {
  // Inisialisasi pin
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin1, INPUT_PULLUP);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Coin Bank");
  delay(2000);  // Tunda 2 detik untuk melihat pesan awal
}

void loop() {
  unsigned long currentTime = millis();  // Ambil waktu saat ini

  // Membaca status tombol pertama untuk mengganti nilai koin
  if (digitalRead(buttonPin1) == LOW) {
    // Memeriksa apakah tombol pertama ditekan dan cukup waktu sejak terakhir ditekan
    if (currentTime - lastButtonPress1 > debounceDelay) {
      coinIndex = (coinIndex + 1) % 5;  // Berputar ke indeks berikutnya (0-4)
      coinValue = coinValues[coinIndex];  // Pilih nilai dari array
      lastButtonPress1 = currentTime;  // Catat waktu terakhir tombol pertama ditekan
    }
  }

  // Membaca sensor IR untuk mendeteksi koin
  if (digitalRead(sensorPin) == LOW && currentTime - lastSensorTrigger > sensorDelay) {
    totalAmount += coinValue;  // Menambah total uang sesuai dengan nilai koin
    lastSensorTrigger = currentTime;  // Catat waktu terakhir sensor mendeteksi koin
  }

  // Menampilkan pilihan nominal koin pada LCD baris pertama
  lcd.setCursor(0, 0);
  lcd.print("KOIN: ");
  lcd.print(coinValue);  // Menampilkan nominal koin yang dipilih
  lcd.print("    ");     // Membersihkan sisa karakter sebelumnya

  // Menampilkan total uang yang terdeteksi pada LCD baris kedua
  lcd.setCursor(0, 1);
  lcd.print("Total: Rp");
  lcd.print(totalAmount);
  lcd.print("    ");     // Membersihkan sisa karakter sebelumnya
}
