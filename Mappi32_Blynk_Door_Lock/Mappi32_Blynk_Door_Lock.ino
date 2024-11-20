#define BLYNK_TEMPLATE_ID "TMPL6XQ5LMuB2"
#define BLYNK_TEMPLATE_NAME "Door Lock"
#define BLYNK_AUTH_TOKEN "428u2YOcgXqksUpexGjFmPbwMzhWG06V"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// Auth Token dari Blynk
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "masamba";
char pass[] = "00000000";


// Pin relay
const int relayPin = 17; // Sesuaikan dengan pin yang Anda gunakan

void setup()
{
  // Inisialisasi Serial untuk debugging
  Serial.begin(9600);

  // Koneksi ke Blynk
  Blynk.begin(auth, ssid, pass);

  // Inisialisasi pin relay sebagai output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Memastikan relay off saat awal
}

void loop()
{
  Blynk.run();
}

// Blynk Write Function untuk mengontrol relay
BLYNK_WRITE(V1)
{
  int relayState = param.asInt(); // Mengambil nilai dari Blynk app (0 atau 1)
  digitalWrite(relayPin, relayState); // Mengatur relay sesuai dengan nilai dari Blynk
}
