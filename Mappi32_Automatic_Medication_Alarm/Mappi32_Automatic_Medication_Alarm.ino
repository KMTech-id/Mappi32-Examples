// How to Build a Medication Reminder at Home using Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //

#define BLYNK_TEMPLATE_ID "***********************************"
#define BLYNK_TEMPLATE_NAME "*********************************"
#define BLYNK_AUTH_TOKEN "************************************"
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <WiFi.h>                   
#include <BlynkSimpleEsp32.h>

// Inisialisasi LCD dengan alamat I2C 0x27 dan ukuran 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 rtc;

const int buzzerPin = 19; // Sesuaikan dengan pin yang digunakan

// Waktu alarm yang diinginkan (24-jam format) - 14:37
int alarmHour = 20;
int alarmMinute = 00;

bool alarmSet = true;
bool alarmActive = false; // Menandakan apakah alarm sedang aktif

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "**********";
char pass[] = "**********";

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Inisialisasi RTC
  if (!rtc.begin()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RTC Tidak Terdeteksi");
    while (1);
  }
  
  if (rtc.lostPower()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set Waktu RTC!");
    // Set waktu RTC ke waktu saat ini (komputer)
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    delay(2000);
  }
  
  // Inisialisasi buzzer
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // Inisialisasi Blynk
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run(); // Jalankan Blynk
  
  DateTime now = rtc.now();
  
  if (!alarmActive) {
    // Mode Normal: Menampilkan waktu
    displayTime(now);
    
    // Cek apakah waktu saat ini sama dengan waktu alarm
    if (alarmSet && now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
      alarmSet = false; // Mencegah alarm berulang setiap detik
      alarmActive = true; // Menandakan alarm sedang aktif
      activateAlarm();
      
      // Mengirim event ke Blynk dengan event code
      Blynk.logEvent("minum_obat_woy");
      
      Serial.println("minum_obat_woy");
    }
  } else {
    // Mode Alarm: Menampilkan pesan alarm dan mengaktifkan buzzer
    displayAlarmMessage();
    
    // Buzzer aktif terus-menerus selama alarm aktif
    digitalWrite(buzzerPin, HIGH);
    
    // Cek untuk menonaktifkan alarm setelah beberapa detik (misalnya 10 detik)
    if (now.second() == 10) { // Alarm aktif selama 10 detik
      deactivateAlarm();
    }
  }
  
  delay(1000);
}

// Fungsi untuk menampilkan waktu pada LCD
void displayTime(DateTime now) {
  // Tampilkan "KMTEK" di baris pertama, di tengah
  lcd.setCursor(0, 0);
  lcd.print("                    "); // Membersihkan baris pertama
  lcd.setCursor(7, 0); // Posisi tengah untuk "KMTEK" di LCD 20x4
  lcd.print("KMTEK");
  
  // Tampilkan waktu pada baris kedua
  lcd.setCursor(0, 1);
  lcd.print("Jam : "); // Membersihkan baris kedua
  lcd.setCursor(6, 1);
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second(), DEC);
  
  // Tampilkan waktu alarm pada baris ketiga
  lcd.setCursor(0, 2);
  lcd.print("Alarm Pada:        "); // Membersihkan baris ketiga
  lcd.setCursor(11, 2);
  if (alarmHour < 10) lcd.print("0");
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10) lcd.print("0");
  lcd.print(alarmMinute);
  
  // Membersihkan baris keempat
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}

// Fungsi untuk mengaktifkan alarm
void activateAlarm() {
  // Menampilkan pesan alarm di baris keempat
  lcd.setCursor(0, 3);
  lcd.print("ALARM!             "); // Opsional: Bisa diubah atau dihapus
}

// Fungsi untuk menampilkan pesan alarm
void displayAlarmMessage() {
  // Membersihkan semua baris sebelum menampilkan pesan
  lcd.clear();
  
  // Menampilkan pesan "Minum Obat Woy" di tengah layar
  String message = "Minum Obat Woy";
  int msgLength = message.length();
  int pos = (20 - msgLength) / 2; // Menghitung posisi tengah
  lcd.setCursor(pos, 1); // Menampilkan di baris kedua
  lcd.print(message);
}

// Fungsi untuk menonaktifkan alarm
void deactivateAlarm() {
  alarmActive = false;
  digitalWrite(buzzerPin, LOW); // Matikan buzzer
  lcd.clear(); // Membersihkan LCD setelah alarm
  delay(1000); // Memberi jeda sebelum kembali ke mode normal
}
