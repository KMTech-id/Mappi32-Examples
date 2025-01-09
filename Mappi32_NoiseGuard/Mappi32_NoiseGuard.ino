#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int soundSensorPin = 32;
const int ledMerah = 13;
const int ledKuning = 14;
const int ledHijau = 16;

float Vref = 0.5;  // Tegangan referensi dari sensor suara (sesuaikan dengan sensor Anda)
float smoothing = 0.9;  // Faktor smoothing untuk moving average
static float smoothVoltage = 0;  // Variabel tegangan stabil

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(ledMerah, OUTPUT);
  pinMode(ledKuning, OUTPUT);
  pinMode(ledHijau, OUTPUT);

  Serial.begin(115200);  // Debugging lewat Serial Monitor
}

void loop() {
  // Baca nilai analog dari sensor suara
  int rawValue = analogRead(soundSensorPin);
  float voltage = rawValue * (3.3 / 4095.0);  // Untuk ESP32 dengan ADC 12-bit

  // Moving average untuk stabilisasi tegangan
  smoothVoltage = (smoothing * smoothVoltage) + ((1 - smoothing) * voltage);

  // Hindari nilai dB yang tidak masuk akal
  float dB = 5 * log10(smoothVoltage / Vref);
  if (dB > 20) dB = 20;  // Batas atas
  if (dB < 0) dB = 0;      // Batas bawah

  Serial.print("Tingkat Kebisingan: ");
  Serial.print(dB);
  Serial.println(" dB");

  // Reset semua LED terlebih dahulu
  digitalWrite(ledMerah, LOW);
  digitalWrite(ledKuning, LOW);
  digitalWrite(ledHijau, LOW);

  lcd.clear();  // Bersihkan layar LCD

  // Kondisi berdasarkan tingkat kebisingan yang lebih rendah
  if (dB > 10) {  // Jika dB di atas 10
    digitalWrite(ledMerah, HIGH);  // LED merah menyala
    lcd.setCursor(0, 0);
    lcd.print("Bahaya Kebisingan");
    lcd.setCursor(0, 1);
    lcd.print("USE EARPLUG");
  } else if (dB > 5 && dB <= 10) {  // Jika dB antara 5 hingga 10
    digitalWrite(ledKuning, HIGH);  // LED kuning menyala
    lcd.setCursor(0, 0);
    lcd.print("Waspada Kebisingan");
    lcd.setCursor(0, 1);
    lcd.print("Disarankan Earplug");
  } else {
    digitalWrite(ledHijau, HIGH);  // LED hijau menyala
    lcd.setCursor(0, 0);
    lcd.print("Aman Kebisingan");
  }

  // Menampilkan tingkat dB pada baris ke-3 LCD
  lcd.setCursor(0, 2);
  lcd.print("Tingkat: ");
  lcd.print(dB);
  lcd.print(" dB");

  delay(500);  // Interval pembacaan berikutnya
}
