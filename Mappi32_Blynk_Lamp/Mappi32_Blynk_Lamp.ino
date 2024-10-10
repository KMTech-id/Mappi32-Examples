// How to Build a Smart Light System Using Blynk and Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //


#define BLYNK_TEMPLATE_ID "*******************"
#define BLYNK_TEMPLATE_NAME "*******************"
#define BLYNK_AUTH_TOKEN "*******************"
#define BLYNK_PRINT Serial
#include <WiFi.h>            // Jika menggunakan ESP8266, ganti dengan <ESP8266WiFi.h>
#include <BlynkSimpleEsp32.h> // Jika menggunakan ESP8266, ganti dengan <BlynkSimpleEsp8266.h>

char auth[] = "*******************"; // Ganti dengan Auth Token dari Blynk
char ssid[] = "*******************";      // Ganti dengan nama WiFi Anda
char pass[] = "*******************";  // Ganti dengan password WiFi Anda

int relayPin = 5; // Ganti dengan pin yang terhubung ke relay

void setup()
{
  // Debug console
  Serial.begin(9600);
  // Menghubungkan ke Blynk
  Blynk.begin(auth, ssid, pass);
  // Mengatur pin relay sebagai output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Awal relay dimatikan
}

// Fungsi ini akan dipanggil saat tombol di aplikasi Blynk ditekan
BLYNK_WRITE(V1)
{
  int relayState = param.asInt(); // Mendapatkan nilai dari aplikasi Blynk (0 atau 1)
  if(relayState == 1)
  {
    digitalWrite(relayPin, HIGH); // Nyalakan relay
  }
  else
  {
    digitalWrite(relayPin, LOW); // Matikan relay
  }
}

void loop()
{
  Blynk.run(); // Jalankan Blynk
}
