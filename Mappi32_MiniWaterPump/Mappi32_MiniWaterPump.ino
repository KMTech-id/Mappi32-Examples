// Easy DIY Mini Water Pump & RTC with Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;
const int relayPin = 19;
bool wateringInProgress = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);

  if (!rtc.begin()) {
    Serial.println("Modul RTC tidak terdeteksi! Periksa koneksi.");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, silakan atur waktu!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  // Cek jika waktu adalah jam 9 pagi atau jam 3 sore dan menitnya adalah 0
  if ((now.hour() == 13 && now.minute() == 17) || (now.hour() == 15 && now.minute() == 0)) {
    if (!wateringInProgress) {
      // Aktifkan relay untuk 5 detik
      digitalWrite(relayPin, HIGH);
      delay(5000); // 5 detik
      digitalWrite(relayPin, LOW);
      Serial.println("Penyiraman telah dilakukan!");
      wateringInProgress = true;
    }
  } else {
    wateringInProgress = false;
    digitalWrite(relayPin, LOW); // Pastikan relay mati jika tidak ada penyiraman yang sedang berlangsung
  }

  // Tampilkan waktu saat ini setiap detik
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Tunda 1 detik sebelum looping kembali
  delay(1000);
}
