#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definisi Pin
const int sensor_pin = 32; // Sensor Pin

// Inisialisasi LCD (alamat 0x27 untuk display 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Inisialisasi Serial untuk debugging
  Serial.begin(9600);

  // Inisialisasi pin
  pinMode(sensor_pin, INPUT);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight(); // Nyalakan backlight LCD

  // Menampilkan pesan awal pada LCD
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000); // Tampilkan pesan selama 2 detik
  lcd.clear();
}

void loop() {
  // Membaca nilai kelembapan tanah dari sensor
  int sensor_data = analogRead(sensor_pin);
  Serial.print("Sensor_data: ");
  Serial.print(sensor_data);
  Serial.print("\t | ");

  // Menampilkan data kelembapan pada LCD
  lcd.setCursor(0, 0); // Set cursor ke baris pertama
  lcd.print("Kelembaban: ");
  lcd.print(sensor_data);
  lcd.print("   "); // Tambahkan spasi untuk menghapus karakter sebelumnya

  // Menampilkan status kelembapan pada LCD
  lcd.setCursor(0, 1); // Set cursor ke baris kedua
  if (sensor_data > 3000) {
    Serial.println("No moisture, Soil is dry");
    lcd.print("Tanah Kering    ");
  }
  else if (sensor_data >= 2300 && sensor_data <= 2500) {
    Serial.println("There is some moisture, Soil is medium");
    lcd.print("Tanah Rada Basah");
  }
  else if (sensor_data < 2200) {
    Serial.println("Soil is wet");
    lcd.print("Tanah Basah     ");
  }

  delay(1000); // Delay 1 detik sebelum pembacaan berikutnya
}
