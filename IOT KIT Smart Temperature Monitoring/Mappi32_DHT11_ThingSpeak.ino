#include <WiFi.h>          // Untuk ESP32
#include <HTTPClient.h>    // Untuk HTTP Request
#include "DHT.h"           // Pustaka DHT

// Konfigurasi DHT11
#define DHTPIN 18           // Pin data DHT11 (sesuaikan dengan wiring)
#define DHTTYPE DHT11      // Tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

// Konfigurasi WiFi
const char* ssid = "alive";       // Ganti dengan nama WiFi Anda
const char* password = "00000000";  // Ganti dengan password WiFi Anda

// ThingSpeak
const char* server = "http://api.thingspeak.com/update";
String apiKey = "8EYHGW7YTFO7H0XI";      // Ganti dengan API Key ThingSpeak Anda

void setup() {
  Serial.begin(115200);           // Memulai komunikasi serial
  dht.begin();                    // Memulai sensor DHT11
  WiFi.begin(ssid, password);     // Menghubungkan ke WiFi
  Serial.print("Menghubungkan ke WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung");
}

void loop() {
  // Membaca data suhu dan kelembapan
  float temperature = dht.readTemperature(); // Membaca suhu (Celsius)
  float humidity = dht.readHumidity();       // Membaca kelembapan (%)

  // Validasi data dari sensor
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca dari sensor DHT11!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print("°C | Kelembapan: ");
  Serial.print(humidity);
  Serial.println("%");

  if (WiFi.status() == WL_CONNECTED) { // Pastikan terhubung ke WiFi
    HTTPClient http;

    // URL dengan parameter data
    String url = String(server) + "?api_key=" + apiKey + 
                 "&field1=" + String(temperature) + 
                 "&field2=" + String(humidity);
    
    http.begin(url);             // Memulai HTTP request ke URL
    int httpResponseCode = http.GET(); // Kirim request GET
    
    if (httpResponseCode > 0) {
      Serial.print("Data dikirim. Response Code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Gagal mengirim data. Error: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Mengakhiri koneksi HTTP
  } else {
    Serial.println("WiFi tidak terhubung");
  }

  delay(5000); // Kirim data setiap 5 detik
}
