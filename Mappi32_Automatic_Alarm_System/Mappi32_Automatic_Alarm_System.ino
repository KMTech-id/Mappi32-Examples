// Automatic Alarm System with Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 20, 4); // Inisialisasi LCD 20x4

// Array untuk menyimpan nama hari dalam bahasa Inggris
char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Pin untuk buzzer
const int buzzerPin = 25;

// Variabel untuk menyimpan waktu alarm (diatur pada 07:00:00 sebagai contoh)
int alarmHour = 7;
int alarmMinute = 0;
int alarmSecond = 0;

// Flag untuk alarm
bool alarmActive = true; // Deklarasi variabel alarmActive di sini

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

    // Inisialisasi pin buzzer sebagai output
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, HIGH); // Set buzzer ke HIGH pada awalnya (low-triggered buzzer)

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
    lcd.setCursor(0, 3);
    lcd.print("Day : ");
    lcd.print(days[now.dayOfTheWeek()]);
    lcd.print("   "); // Menambahkan spasi untuk menghapus karakter yang tersisa

    // Cek apakah waktu saat ini sesuai dengan waktu alarm
    if (alarmActive && now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == alarmSecond) {
        activateBuzzer();
    }

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

void activateBuzzer() {
    // Bunyikan buzzer selama 10 detik dengan delay 500ms
    for (int i = 0; i < 20; i++) { // 20 loop = 10 detik (20 * 500ms)
        digitalWrite(buzzerPin, LOW); // Bunyikan buzzer
        lcd.setCursor(5, 3); // Posisi kursor di baris keempat
        lcd.print("Bangun woy   "); // Tampilkan pesan "Bangun woy"
        delay(250); // Tahan buzzer LOW selama 250ms (setengah dari 500ms)
        digitalWrite(buzzerPin, HIGH); // Matikan buzzer
        lcd.setCursor(0, 3); // Posisi kursor di baris keempat
        lcd.print("                   "); // Hapus pesan "Bangun woy"
        delay(250); // Tahan buzzer HIGH selama 250ms (setengah dari 500ms)
    }
    // Set alarmActive ke false setelah buzzer selesai berbunyi
    alarmActive = false;
}
