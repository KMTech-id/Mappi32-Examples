// How to Build a Digital Clock and Calendar with RTC Sensor //
// @yugoyokaii . On Youtube KMTek Indonesia //

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Inisialisasi LCD 20x4

// Array untuk menyimpan nama hari dalam bahasa Inggris
char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
    // Inisialisasi LCD dan lampu latar
    lcd.init();
    lcd.backlight();
    lcd.clear();

    // Menampilkan pesan awal pada LCD
    lcd.setCursor(7, 1);
    lcd.print("KMTek");
    lcd.setCursor(6, 2);
    lcd.print("Mappi32");

    // Memberikan jeda 3 detik sebelum melanjutkan
    delay(3000);

    // Inisialisasi komunikasi serial dan I2C
    Serial.begin(115200);
    Wire.begin();

    // Memeriksa apakah RTC dapat diinisialisasi
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Berhenti di sini jika RTC tidak ditemukan
    }

    // Mengatur waktu RTC dengan waktu kompilasi program
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
    // Mendapatkan waktu dan tanggal saat ini dari RTC
    DateTime now = rtc.now();

    // Menghapus tampilan sebelumnya
    lcd.clear();
    // Menampilkan pesan "KMTek" di baris pertama
    lcd.setCursor(7, 0);
    lcd.print("KMTek");

    // Menampilkan waktu pada baris kedua
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    printTwoDigits(now.hour());
    lcd.print(":");
    printTwoDigits(now.minute());
    lcd.print(":");
    printTwoDigits(now.second());

    // Menampilkan tanggal pada baris ketiga
    lcd.setCursor(0, 2);
    lcd.print("Date: ");
    printTwoDigits(now.day());
    lcd.print("/");
    printTwoDigits(now.month());
    lcd.print("/");
    lcd.print(now.year(), DEC);

    // Menampilkan nama hari pada baris keempat
    lcd.setCursor(0, 3); // Atur posisi kursor ke baris 3, kolom 2
    lcd.print("Day : ");
    lcd.print(days[now.dayOfTheWeek()]); // Cetak nama hari berdasarkan indeks

    // Delay 1 detik sebelum pembaruan berikutnya
    delay(1000);
}

void printTwoDigits(int number) {
    // Fungsi untuk mencetak angka dengan format dua digit
    if (number < 10) {
        lcd.print("0"); // Cetak angka nol jika angkanya di bawah 10
    }
    lcd.print(number); // Cetak angkanya
}
