#define BLYNK_TEMPLATE_ID "********************************"
#define BLYNK_TEMPLATE_NAME "******************************"
#define BLYNK_AUTH_TOKEN "*********************************"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Informasi WiFi
const char* ssid = "******************";       // Ganti dengan nama WiFi Anda
const char* password = "**************"; // Ganti dengan password WiFi Anda

// Sensor HC-SR04
#define trigPin 25
#define echoPin 26

void setup() {
  // Inisialisasi serial dan WiFi
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Konfigurasi pin sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Blynk HC-SR04 Notification");
}

void loop() {
  Blynk.run(); // Jalankan koneksi Blynk

  // Kirim sinyal trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Baca sinyal echo
  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2;

  // Tampilkan jarak di Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  // Jika jarak <= 8 cm, kirim notifikasi
  if (distance <= 4.0 && distance > 0) {
    Serial.println("Object detected within 8 cm!");
    Blynk.logEvent("paket_anda_sudah_datang", "paket_anda_sudah_datang"); // Log event di Blynk
    delay(1000); // Hindari pengiriman notifikasi berulang
  }

  delay(500);
}
