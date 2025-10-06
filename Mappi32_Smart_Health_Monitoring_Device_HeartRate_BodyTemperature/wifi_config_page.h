#ifndef WIFI_CONFIG_PAGE_H
#define WIFI_CONFIG_PAGE_H

#include "config.h"
#include <WebServer.h>
#include <Preferences.h>

// 1. Sertakan file-file HTML yang sudah dipisah
#include "wifi_page.h"
#include "thingspeak_page.h"

// Deklarasi objek WebServer dan Preferences
WebServer webServer(80);
Preferences preferences;

// 2. HAPUS deklarasi string "const char* wifiFormHtml" dan "const char* thingspeakFormHtml" dari sini.

// Fungsi yang dijalankan saat form WiFi disimpan
void handleSaveWifi() {
  Serial.println("Menerima data konfigurasi WiFi baru...");
  String newSsid = webServer.arg("ssid");
  String newPass = webServer.arg("password");

  preferences.begin("device-config", false);
  preferences.putString("ssid", newSsid);
  preferences.putString("password", newPass);
  preferences.end();
  
  String response = "<h1>Konfigurasi WiFi Disimpan!</h1><p>Perangkat akan reboot.</p>";
  webServer.send(200, "text/html", response);

  delay(2000);
  ESP.restart();
}

// Fungsi yang dijalankan saat form ThingSpeak disimpan
void handleSaveThingSpeak() {
  Serial.println("Menerima data konfigurasi ThingSpeak baru...");
  String newApiKey = webServer.arg("apikey");

  preferences.begin("device-config", false);
  preferences.putString("apiKey", newApiKey);
  preferences.end();
  
  String response = "<h1>API Key ThingSpeak Disimpan!</h1><p>Perangkat akan reboot.</p>";
  webServer.send(200, "text/html", response);

  delay(2000);
  ESP.restart();
}

// Fungsi untuk menampilkan halaman yang sesuai
void handleRoot() {
  if (configMode == "wifi") {
    // Gunakan variabel dari file header yang di-include
    webServer.send_P(200, "text/html", wifiFormHtml); 
  } else if (configMode == "thingspeak") {
    // Gunakan variabel dari file header yang di-include
    webServer.send_P(200, "text/html", thingspeakFormHtml);
  } else {
    webServer.send(404, "text/plain", "Mode Konfigurasi Tidak Valid");
  }
}

// Fungsi untuk memulai mode AP dan web server
void startAPMode() {
  const char* apSsid = "SmartHealth-Setup";
  
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSsid, "12345678");

  IPAddress apIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(apIP);

  showAPModeScreen(apIP.toString());

  webServer.on("/", handleRoot);
  webServer.on("/save_wifi", HTTP_POST, handleSaveWifi);
  webServer.on("/save_ts", HTTP_POST, handleSaveThingSpeak);
  webServer.begin();
  Serial.println("Web server dimulai.");
}

#endif // WIFI_CONFIG_PAGE_H