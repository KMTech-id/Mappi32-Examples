#define BLYNK_TEMPLATE_ID "TMPL6jlPM7dhM"
#define BLYNK_TEMPLATE_NAME "smart lamp"
#define BLYNK_AUTH_TOKEN "RM1BNWHm1BgMQYkuFBWWcgB0CUh6XODe"
#define BLYNK_PRINT Serial
#include <WiFi.h>            // Jika menggunakan ESP8266, ganti dengan <ESP8266WiFi.h>
#include <BlynkSimpleEsp32.h> // Jika menggunakan ESP8266, ganti dengan <BlynkSimpleEsp8266.h>

char auth[] = "RM1BNWHm1BgMQYkuFBWWcgB0CUh6XODe"; // Ganti dengan Auth Token dari Blynk
char ssid[] = "Lamzu";      // Ganti dengan nama WiFi Anda
char pass[] = "00000000";  // Ganti dengan password WiFi Anda

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
