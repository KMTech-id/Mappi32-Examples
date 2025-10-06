#ifndef DISPLAY_FUNCTIONS_H
#define DISPLAY_FUNCTIONS_H

#include "config.h"

void showWifiStatus() {
  display.setCursor(0, SCREEN_HEIGHT - 8);
  if (WiFi.status() == WL_CONNECTED) {
    display.print("WiFi: OK");
  } else {
    display.print("WiFi: NO");
  }
}

void showHome() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== Smart Health ==");
  display.println("");
  display.println("Tekan SELECT");
  display.println("untuk mulai...");
  display.println("");
  display.println("Next/Prev: Config");
  showWifiStatus();
  display.display();
}

void showWifiConfig() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== WiFi Config ==");
  display.println("");
  display.println("Tekan SELECT untuk");
  display.println("memulai Hotspot.");
  showWifiStatus();
  display.display();
}

void showThingSpeakConfig() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== T/Speak Cfg ==");
  display.println("");
  display.println("Tekan SELECT untuk");
  display.println("memulai Hotspot.");
  showWifiStatus();
  display.display();
}

void showAPModeScreen(String ip) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== Mode AP Aktif ==");
  display.println("");
  display.println("SSID:");
  display.println(" SmartHealth-Setup");
  display.println("Password:");
  display.println(" 12345678");
  display.println("IP: " + ip);
  display.display();
}

void showSelectID() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Pilih ID Pasien:");
  display.println("");
  display.print(">> ID: "); display.println(currentID);
  display.println("");
  display.println("Ganti dgn Next/Prev");
  showWifiStatus();
  display.display();
}

void showPlaceFinger() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== Ukur Nadi ==");
  display.println("");
  display.println("Letakkan jari");
  display.println("pada sensor...");
  showWifiStatus();
  display.display();
}

void showPositionTemp() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== Ukur Suhu ==");
  display.println("");
  display.println("Arahkan sensor");
  display.println("ke dahi (stabil)");
  showWifiStatus();
  display.display();
}

void showCountdown(const char* msg, int remain) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(msg);
  display.println("");
  display.setTextSize(2);
  display.setCursor(30, 25);
  display.print("Mulai: ");
  display.print(remain);
  display.setTextSize(1);
  showWifiStatus();
  display.display();
}

void showResult() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== Hasil Akhir ==");
  display.print("ID Pasien: "); display.println(currentID);
  display.println("---------------------");
  display.print("Detak Jantung: "); display.print(lastHeartRate, 0); display.println(" bpm");
  display.print("Oksigen (SpO2): "); display.print(lastSpO2, 0); display.println(" %");
  display.print("Suhu Tubuh: "); display.print(lastTemp, 1); display.println(" C");
  display.println("---------------------");
  display.println("Back: Upload & Home");
  display.display();
}

void showError(const char* msg) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("== ERROR ==");
  display.println("");
  display.println(msg);
  display.println("");
  display.println("Tekan Back untuk");
  display.println("mencoba kembali...");
  display.display();
}

#endif // DISPLAY_FUNCTIONS_H