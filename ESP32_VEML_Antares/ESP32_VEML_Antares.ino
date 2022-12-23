#include <WiFi.h>
#include <WiFiClient.h>
//#include <ESP32WiFi.h>
//#include "AntaresESP8266HTTP.h"
#include <AntaresESP32HTTP.h>
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv;
//API Key Antares
#define ACCESSKEY "4ee20d7d3239b545:479738207f12da97"

//SSID dan Password WiFi
#define WIFISSID "KMTek"
#define PASSWORD "KMTek123"

//Nama Aplikasi Antares
#define projectName "Chart-uv"
//Nama Device Antares
#define deviceName "uv-value"

AntaresESP32HTTP antares(ACCESSKEY);

void setup() {
  Serial.begin(115200);
  antares.setDebug(true);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.println("");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFISSID);
}

void loop() {

  float Uva = (uv.a());
  float Uvb = (uv.b());

  Serial.println(String(uv.a()));
  Serial.println(String(uv.b()));
  Serial.println ("UV A : ");
  Serial.print (Uva);
   Serial.println ("\n");
  Serial.println ("UV B : ");
  Serial.print (Uvb);
  delay (1000);

  Serial.println(String(uv.a()) + ", " + String(uv.b()));
  delay(250);

  antares.add(" Uv a", Uva);
  antares.add("Uv b", Uvb);
  //  antares.add(uv.a());
  //  antares.add (uv.b());
  // Mengirim data buffer ke server Antares
  // dengan detail Aplikasi dan Device (HTTPS)
  //antares.send(projectName, deviceName);

  // Mengirim data buffer ke server Antares
  // dengan detail Aplikasi dan Device (HTTP)
  antares.sendNonSecure(projectName, deviceName);
  delay(1000);
}
