//Mappi32 by KMTek
// Yogyakarta - Indonesia

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "SHTSensor.h"

SHTSensor sht;

float temperature, humidity;
String tempStr, humStr;
String datakirim;

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
  while (!LoRa.begin(915E6)) {
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


  if (sht.readSample()) {
    Serial.println("Temperature & Humidity ");
    Serial.print("  Temperature :  ");
    Serial.println(sht.getTemperature(), 2);
    Serial.print("  Humidity: ");
    Serial.println(sht.getHumidity(), 2);
    Serial.print("\n");
  } else {
    Serial.print("Error in readSample()\n");
  }

  tempStr = String (sht.getTemperature(), 2);
  humStr = String (sht.getHumidity(), 2);

  datakirim = "#" + tempStr + "," +  humStr + "!";

  LoRa.beginPacket();
  LoRa.print(datakirim);
  LoRa.endPacket();
  counter++;

  delay(5000);
}
