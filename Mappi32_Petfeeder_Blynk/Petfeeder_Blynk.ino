#include <ESP32Servo.h>
#include <WiFi.h>

// Deklarasi objek servo
Servo myservo;
int StatusPakan = 0;  // Variabel untuk status pemberian pakan

// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL6ckdJhrLr"
#define BLYNK_TEMPLATE_NAME "Kontrol Servo Petfeeder"
#define BLYNK_AUTH_TOKEN "b3utKeLwcMssG80g5AXgleICHhH_JZHE"

#include <BlynkSimpleEsp32.h>

// Wi-Fi credentials
const char* ssid = "matahari";
const char* password = "mentarirhaa";

void setup() { 
    // Inisialisasi servo
    myservo.attach(19);  // Pin D5 = GPIO 19
    myservo.write(0);   // Posisi awal servo

    Serial.begin(115200);
    Serial.println("Mencoba terhubung ke WiFi...");

    // Koneksi ke WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Terkoneksi");

    // Inisialisasi koneksi Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
    Serial.println("Blynk Terkoneksi");
}

void BeriPakan() {
    // Putar servo dari posisi 0 ke 90
    for (int posisi = 0; posisi <= 90; posisi++) {
        myservo.write(posisi);
        delay(20);
    }
    // Kembalikan servo ke posisi awal
    for (int posisi = 90; posisi >= 0; posisi--) {
        myservo.write(posisi);
        delay(20);
    }
}

void loop() {
    // Jalankan aplikasi Blynk
    Blynk.run();

    // Tampilkan StatusPakan di Serial Monitor
    Serial.println("Status Pakan: " + String(StatusPakan));

    // Jika StatusPakan = 1 maka beri pakan
    if (StatusPakan == 1) {
        BeriPakan();  // Panggil fungsi BeriPakan
        Blynk.virtualWrite(V0, 0);  // Kembalikan StatusPakan ke 0 di Blynk
        StatusPakan = 0;  // Set StatusPakan ke 0
        delay(2000);  // Delay untuk mencegah operasi ganda
    }
}

// Baca datastream StatusPakan
BLYNK_WRITE(V0) {
    StatusPakan = param.asInt();  // Tampung nilai dari virtual pin V0 ke StatusPakan
}