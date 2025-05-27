#define BLYNK_TEMPLATE_ID "TMPL6nCZkVa1N"
#define BLYNK_TEMPLATE_NAME "pred"
#define BLYNK_AUTH_TOKEN "wpfJ3fgcR4rXYJ_veS2uhfGjf35ZgybV"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Data WiFi dan Token Blynk
char auth[] = "wpfJ3fgcR4rXYJ_veS2uhfGjf35ZgybV";
char ssid[] = "ISI DENGAN NAMA WIFI KALIAN";
char pass[] = "ISI DENGAN PASWORD WIFI KALIAN";

#define DHTPIN 25
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

BlynkTimer timer;

const float ketinggian = 172.0;  // Ketinggian tempat kamu dalam meter
const float tekananOffset = -4.0;

// Fungsi untuk prediksi cuaca
String prediksiCuaca(float suhu, float kelembapan, float tekanan) {
  String prediksi = "";

  if (tekanan >= 1010 && tekanan <= 1013 && suhu >= 28 && suhu <= 34 && kelembapan >= 60 && kelembapan <= 75) {
    prediksi = "Cerah";
  } 
  else if (tekanan >= 1010 && tekanan <= 1012 && suhu >= 26 && suhu <= 33 && kelembapan >= 70 && kelembapan <= 86) {
    prediksi = "Cerah Berawan";
  } 
  else if (tekanan >= 1007 && tekanan <= 1011 && suhu >= 24 && suhu <= 32 && kelembapan >= 75 && kelembapan <= 92) {
    prediksi = "Berawan";
  }
  else if (tekanan >= 1006 && tekanan <= 1009 && suhu >= 22 && suhu <= 29 && kelembapan >= 85 && kelembapan <= 98) {
    prediksi = "Hujan";
  }
  else {
    prediksi = "Tidak pasti";
  }

  return prediksi;
}

// Fungsi koreksi tekanan
float koreksiTekananKePermukaanLaut(float tekananTerukur, float suhu, float ketinggian) {
  return tekananTerukur * pow((1 - (0.0065 * ketinggian) / (suhu + (0.0065 * ketinggian) + 273.15)), -5.257);
}

void kirimDataKeBlynk() {
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();

  sensors_event_t event;
  bmp.getEvent(&event);
  float tekananTerukur = event.pressure;

  if (isnan(suhu) || isnan(kelembapan) || tekananTerukur <= 0) {
    Serial.println("Gagal membaca data sensor!");
    return;
  }

  float tekananPermukaanLaut = koreksiTekananKePermukaanLaut(tekananTerukur , suhu, ketinggian) + tekananOffset;

  Serial.println("===== Monitor Cuaca =====");
  Serial.print("Tekanan (lokasi): ");
  Serial.print(tekananTerukur);
  Serial.println(" hPa");

  Serial.print("Tekanan (permukaan laut): ");
  Serial.print(tekananPermukaanLaut);
  Serial.println(" hPa");

  Serial.print("Suhu       : ");
  Serial.print(suhu);
  Serial.println(" °C");

  Serial.print("Kelembapan : ");
  Serial.print(kelembapan);
  Serial.println(" %");

  String prediksi = prediksiCuaca(suhu, kelembapan, tekananPermukaanLaut);
  Serial.print("Prediksi   : ");
  Serial.println(prediksi);

  Serial.println("=========================");

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, kelembapan);
  Blynk.virtualWrite(V2, tekananPermukaanLaut);
  Blynk.virtualWrite(V3, prediksi);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  if (!bmp.begin()) {
    Serial.println("Sensor BMP180 tidak terdeteksi!");
    while (1);
  }

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung!");
  Blynk.begin(auth, ssid, pass);

  timer.setInterval(5000L, kirimDataKeBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
}
