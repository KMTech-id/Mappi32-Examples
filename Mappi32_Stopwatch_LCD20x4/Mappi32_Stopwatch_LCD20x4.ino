#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned long mulai, selesai, dataStopWatch;
int i = 0;
int fPaus = 0;
long lastButton = 0;
long delayAntiBouncing = 50;
long dataPaus = 0;

void setup() {
    pinMode(16, INPUT_PULLUP); // Button 1 di pin 16
    pinMode(17, INPUT_PULLUP); // Button 2 di pin 17
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    displayCenteredText("KMTek", 1);
    displayCenteredText("Mappi32", 2);
    delay(2000);
    lcd.clear();
    displayCenteredText("Tekan Tombol", 0);
    displayCenteredText("Start / Stop", 1);
}

void loop() {
    // Cek Button 1 (pin 16)
    if (digitalRead(16) == LOW) {
        if ((millis() - lastButton) > delayAntiBouncing) {
            if (i == 0) {
                lcd.clear();
                displayCenteredText("Start Timer", 0);
                mulai = millis();
                fPaus = 0;
            } else if (i == 1) {
                displayCenteredText("Stop Timer", 0);
                dataPaus = dataStopWatch;
                fPaus = 1;
            }
            i = !i;
        }
        lastButton = millis();
    }

    // Cek Button 2 (pin 17)
    if (digitalRead(17) == LOW && fPaus == 1) {
        dataStopWatch = 0;
        dataPaus = 0;
        lcd.clear();
        displayCenteredText("Reset Stopwatch", 0);
        displayCenteredText("0:00:00.0", 1);
        delay(2000);
        lcd.clear();
        displayCenteredText("Tekan Tombol", 0);
        displayCenteredText("Start / Stop", 1);
    }

    // Jika stopwatch berjalan, tampilkan waktu
    if (i == 1) {
        selesai = millis();
        dataStopWatch = selesai - mulai;
        dataStopWatch += dataPaus;

        // Hitung jam, menit, detik, dan milidetik
        unsigned long jam = dataStopWatch / 3600000;
        unsigned long sisa = dataStopWatch % 3600000;
        unsigned long menit = sisa / 60000;
        sisa %= 60000;
        unsigned long detik = sisa / 1000;
        unsigned long milidetik = sisa % 1000;

        // Buat string waktu dengan format yang benar
        char buffer[12];
        snprintf(buffer, sizeof(buffer), "%lu:%02lu:%02lu.%03lu", jam, menit, detik, milidetik);
        
        // Tampilkan waktu di LCD dengan posisi tengah
        displayCenteredText(buffer, 1);
    }
}

void displayCenteredText(const char* text, int row) {
    int textLength = strlen(text);
    int startPosition = (20 - textLength) / 2; // Hitung posisi awal untuk teks tengah
    lcd.setCursor(startPosition, row);
    lcd.print(text);
}
