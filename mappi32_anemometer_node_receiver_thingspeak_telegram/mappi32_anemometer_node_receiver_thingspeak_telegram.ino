#include <WiFi.h>
#include <CTBot.h>

#define SECRET_CH_ID  xxxxxxxx  // replace 0000000 with your channel number
#define SECRET_WRITE_APIKEY "xxxxxxxxxxxxxxxx"   // replace with your channel write API Key
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

CTBot mybot;
TBMessage msg ;
String ssid = "SSID"; // wifi name
String pass = "PASSWORD"; // wifi password
String token = "xxxxxxxxxx:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // token API telgram bot
const int id = xxxxxxxxxx; // Id telegram

String isipesan = "Wind speed"; //message from telegram for view the wind speed
String Status;
boolean Start ;
#define BATAS_MAX 19 //maximum speed to send warning to telegram

#include <math.h>
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

int windspeed;
String windspeedStr;
String rssiStr;

void setup() {
  Serial.begin(115200); 
  Serial.println("Memulai telegram bot. koneksi ke wifi");

  // WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);

  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(921E6))//melakukan perulangan sampai Lora terkoneksi
  {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      windspeed = LoRaData.toInt();
      windspeedStr = String(windspeed);
      TestConTelegram();
      sendTelegram();
      
      Serial.print("RSSI: ");
      rssiStr = LoRa.packetRssi();
      Serial.print(rssiStr);
      Serial.print(" dBm"); Serial.print("\n");
      ThingSpeak.setField(1, windspeedStr);
      ThingSpeak.setField(2, rssiStr);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      Serial.println("\nData terkirim ke Thingspeak\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }
  }
}
