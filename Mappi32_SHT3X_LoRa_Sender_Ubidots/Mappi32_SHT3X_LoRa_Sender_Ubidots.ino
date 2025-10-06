//Mappi32 by KMTek
// Yogyakarta - Indonesia

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#include "SHTSensor.h"

SHTSensor sht;
String datasensor;
//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

int counter = 0;

void setup() {
  //initialize Serial Monitor
  Wire.begin();
  Serial.begin(115200);
  Serial.println("1");
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(921E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  if (sht.init()) {
    Serial.print("init(): success\n");
  } else {
    Serial.print("init(): failed\n");
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x
}

void loop() {
  datasensor = String (sht.getTemperature(), 2);
  if (sht.readSample()) {
    //    Serial.print("SHT:\n");
    //    Serial.print("  Humidity: ");
    //    Serial.print(sht.getHumidity(), 2);
    //    Serial.print("\n");
    Serial.print("  Temperature :  ");
    Serial.print(sht.getTemperature(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Error in readSample()\n");
  }
  LoRa.beginPacket();
  LoRa.print(datasensor);
  LoRa.endPacket();
  counter++;

  delay(5000);
}
