#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "config.h"

// --- FUNGSI BARU: Verifikasi koneksi sensor I2C ---
bool checkI2CConnection() {
  byte error;
  bool success = true;

  // Cek OLED Display (Alamat 0x3C)
  Wire.beginTransmission(0x3C);
  error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("I2C Check Gagal: OLED Display tidak ditemukan.");
    success = false;
  }

  // Cek MAX30100 Pulse Oximeter (Alamat 0x57)
  Wire.beginTransmission(0x57);
  error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("I2C Check Gagal: MAX30100 tidak ditemukan.");
    success = false;
  }
  
  // Cek MLX90614 Temperature Sensor (Alamat 0x5A)
  WireMLX.beginTransmission(0x5A);
  error = WireMLX.endTransmission();
  if (error != 0) {
    Serial.println("I2C Check Gagal: MLX90614 tidak ditemukan.");
    success = false;
  }

  if (success) {
    Serial.println("I2C Check Berhasil: Semua sensor terdeteksi.");
  }
  
  return success;
}


bool btnPressed(int pin) {
  int index = -1;
  for (int i=0; i<4; i++) {
    if (btnPins[i] == pin) {
      index = i;
      break;
    }
  }
  if (index == -1) return false;

  if (digitalRead(pin) == LOW && millis() - lastBtnPressTime[index] > debounceDelay) {
    lastBtnPressTime[index] = millis();
    return true;
  }
  return false;
}

void recoverI2C() {
  Serial.println("Mencoba recovery I2C...");
  Wire.end();
  delay(100);
  Wire.begin();
  WireMLX.end();
  delay(100);
  WireMLX.begin(25, 26);
  delay(100);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { Serial.println("OLED masih gagal"); }
  if (!pox.begin()) { Serial.println("MAX30100 masih gagal"); }
  if (!mlx.begin(0x5A, &WireMLX)) { Serial.println("MLX90614 masih gagal"); }
  
  i2cErrorCount = 0;
  lastSuccessfulRead = millis();
}

void kirimThingSpeak(int id, float hr, float spo2, float temp) {
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey;
    if (id == 1) {
      url += "&field1=" + String(hr, 1);
      url += "&field2=" + String(spo2, 1);
      url += "&field3=" + String(temp, 1);
    } else if (id == 2) {
      url += "&field4=" + String(hr, 1);
      url += "&field5=" + String(spo2, 1);
      url += "&field6=" + String(temp, 1);
    }

    http.begin(url);
    int httpCode = http.GET();
    if(httpCode > 0){
      Serial.println("Data terkirim ke ThingSpeak");
    } else {
      Serial.println("Gagal kirim data");
    }
    http.end();

    pox.shutdown();
    delay(200);
    pox.resume();
    delay(200);
    pox.begin();
    pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);
    Serial.println("MAX30100 direset setelah upload");
  } else {
    Serial.println("WiFi tidak konek!");
  }
}

#endif // HELPER_FUNCTIONS_H