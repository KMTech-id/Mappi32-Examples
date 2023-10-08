//  Deklarasi Sensor Suhu Kelembaban Udara DHT
#include "DHT.h"
#define DHTPIN 23
float hum, tem;               // deklarasi variabel "hum" dan "tem"
String temString, humString;  // deklarasi variabel "temString" dan "humString"
// Uncomment sesuai sensor yang digunakan
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

//WiFi
#include <WiFi.h>
#include <HTTPClient.h>
int status = WL_IDLE_STATUS;
const char* ssid     = "SSID";
const char* password = "PASSWORD1";
WiFiClient client;

// Ubidots
char iot_server[] = "http://things.iot.ubidots.com";
const int port = 80;
String token = "XXXX-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
String DEVICE_LABEL = "esp32";
String temperatureVarLabel = "suhu";
String humidityVarLabel = "kelembaban";

void setup() {
  Serial.begin(9600);
  Serial.println(F("Mulai Program Baca DHT dan kirim ke Ubidots!"));
  delay(1000);
  wifiInit();
  dht.begin();
}

void loop() {
  readSensor();
  sendData();
  delay(20000);  // jeda
}
