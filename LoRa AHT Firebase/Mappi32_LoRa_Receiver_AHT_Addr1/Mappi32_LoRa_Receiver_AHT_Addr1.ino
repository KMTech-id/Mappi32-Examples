#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define ss 15
#define rst 0
#define dio0 27

#define WIFI_SSID "*************"
#define WIFI_PASSWORD "*********"

#define API_KEY "AIzaSyBOX-j8Gg7B8d_fvPbbsh2F7ur9C2xVycM"
#define DATABASE_URL "https://aht10-v2-default-rtdb.asia-southeast1.firebasedatabase.app/"

// LoRa device address
const int myAddress = 0x02; // Receiver's address

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Receiver with Firebase Integration");

  // Setup WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.println("Connected to WiFi");

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase Authentication Successful");
    signupOK = true;
  } else {
    Serial.println("Firebase Authentication Failed");
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize LoRa
  SPI.begin(14, 12, 13, ss);
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    int sender = LoRa.read(); // Read sender address
    int receiver = LoRa.read(); // Read receiver address

    // Check if the packet is for this receiver
    if (receiver == myAddress) {
      String LoRaData;

      Serial.println("Received packet from sender address: " + String(sender));

      while (LoRa.available()) {
        LoRaData += (char)LoRa.read(); // Read LoRa data
      }

      Serial.println("Received Data: " + LoRaData);

      // Send received data to Firebase
      if (Firebase.RTDB.setString(&fbdo, "SensorData/AHT10Data", LoRaData)) {
        Serial.println("Data sent to Firebase successfully");
      } else {
        Serial.println("Failed to send data to Firebase");
        Serial.println(fbdo.errorReason());
      }
    } else {
      Serial.println("Packet not for this receiver.");
    }
  }
}
