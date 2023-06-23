//Library dan Deklarasi Variabel
#include <TinyGPS++.h>
#include <SPI.h>
#include <LoRa.h>


static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 115200;

static const int loraFrequency = 915E63;

static const int ss = 15;
static const int rst = 0;
static const int dio0 = 27;


TinyGPSPlus gps;

#define serialGPS Serial2


void setup() {
  Serial.begin(115200);
  serialGPS.begin(GPSBaud);

  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1);
  }

}


void loop() {

  // Proses pembacaan data GPS
  while (serialGPS.available() > 0)
    if (gps.encode(serialGPS.read()))

   // Jika data terbaca fungsi sendLocation dippanggil
      sendLocation();
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

}


// Proses Mengirimkan data melalui LoRa
void sendLocation() {
  if (gps.location.isValid()) {  //  Memeriksa apakah data lokasi GPS
    float lat = gps.location.lat();  //  Mendapatkan nilai latitude dan longitude
    float lng = gps.location.lng();

    String message = "*" + String(lat, 6) + "," + String(lng, 6) + "#";  // Membuat pesan yang akan dikirim melalui modul LoRa.
    
    // Proses Pengiriman melalui LoRa
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket();
    Serial.println("Location sent: " + message);
    delay(1000);
  }
}
