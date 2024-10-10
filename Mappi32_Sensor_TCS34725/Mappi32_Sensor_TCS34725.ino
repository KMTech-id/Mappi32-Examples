// Building Your Own Color Detector using RGB Sensor TCS34725 & Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //

#include <Wire.h> // Library untuk komunikasi I2C
#include <Adafruit_Sensor.h> // Library untuk sensor
#include "Adafruit_TCS34725.h" // Library untuk sensor warna TCS34725
#include <LiquidCrystal_I2C.h> // Library untuk LCD I2C

byte gammatable[256]; // Tabel gamma untuk koreksi warna
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // Inisialisasi objek sensor warna
LiquidCrystal_I2C lcd (0x27,20,4); // Inisialisasi objek LCD dengan alamat I2C dan ukuran 20x4

#define commonAnode false // Keterangan apakah display menggunakan anode umum atau tidak

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial

  lcd.init(); // Inisialisasi LCD
  lcd.backlight(); // Nyalakan backlight LCD
  
  lcd.begin(20, 4); // Inisialisasi LCD dengan ukuran 20x4
  lcd.clear(); // Bersihkan layar LCD                 
  lcd.setCursor(8, 0); // Atur posisi tulisan di bagian atas tengah
  lcd.print("KMTEK"); // Tampilkan teks "YUGO" pada LCD
  delay(3000); // Tunggu selama 3 detik
  
  lcd.setCursor(7, 1); // Atur posisi pesan di bagian tengah bawah
  if (tcs.begin()) { // Cek apakah sensor siap
    lcd.print("Mappi32"); // Jika siap, tampilkan pesan "Ready"
  } else {
    lcd.print("Sensor Error"); // Jika ada kesalahan pada sensor, tampilkan pesan "Sensor Error"
    while (1); // Berhenti dan tunggu di sini untuk selamanya
  }
}

void loop() {
  float red, green, blue; // Variabel untuk menyimpan nilai komponen warna                        
  tcs.setInterrupt(false); // Matikan interrupt dari sensor      
  delay(60); // Tunggu selama 60 milidetik                   
  tcs.getRGB(&red, &green, &blue); // Ambil nilai RGB dari sensor  
  tcs.setInterrupt(true); // Aktifkan kembali interrupt
  delay(500); // Tunggu selama 500 milidetik                     
  lcd.clear(); // Bersihkan layar LCD                  
  lcd.print("Warna: "); // Tampilkan label "Warna:" pada LCD   
  lcd.setCursor(0, 1); // Posisikan kursor di baris kedua
  lcd.print("R:");
  lcd.print(int(red)); // Tampilkan nilai komponen merah
  lcd.print(" G:");
  lcd.print(int(green)); // Tampilkan nilai komponen hijau
  lcd.print(" B:");
  lcd.print(int(blue)); // Tampilkan nilai komponen biru

  // Cek apakah komponen merah mendominasi
  if (red > green && red > blue && red >= 100) {
    lcd.setCursor(8, 0); // Atur posisi pesan di baris pertama, kolom kesembilan
    lcd.print("red"); // Tampilkan teks "red" pada LCD
  } 
  // Cek apakah komponen hijau mendominasi
  else if (green > red && green > blue && green >= 100) {
    lcd.setCursor(8, 0); // Atur posisi pesan di baris pertama, kolom kesembilan
    lcd.print("green"); // Tampilkan teks "green" pada LCD
  } 
  // Cek apakah komponen biru mendominasi
  else if (blue > red && blue > green && blue >= 100) {
    lcd.setCursor(8, 0); // Atur posisi pesan di baris pertama, kolom kesembilan
    lcd.print("blue"); // Tampilkan teks "blue" pada LCD
  }
  
  delay(1000); // Tunggu selama 1 detik sebelum membaca sensor lagi
}
