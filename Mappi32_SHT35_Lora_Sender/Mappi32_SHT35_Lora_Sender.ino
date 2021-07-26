#include "Wire.h"
#include "SHT31.h"
#include <SPI.h>
#include <LoRa.h>

#define SHT31_ADDRESS   0x44

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

uint32_t start;
uint32_t stop;

SHT31 sht;


void setup()
{
  Serial.begin(115200);
  pinMode(5, OUTPUT);

  Serial.println(__FILE__);
  Serial.print("SHT31_LIB_VERSION: \t");
  Serial.println(SHT31_LIB_VERSION);

  Wire.begin();
  sht.begin(SHT31_ADDRESS);
  Wire.setClock(100000);

  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();

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
}


void loop()
{
  start = micros();
  sht.read();         // default = true/fast       slow = false
  stop = micros();
  String temp = String(sht.getTemperature(), 1);
  int tempInt = temp.toInt();
  //  String humi = String(sht.getHumidity(), 1);

  Serial.print(tempInt);
  Serial.print("°C");
  Serial.print("\n");
  //  Serial.println(humi);
  //  Serial.print("RH");
  //  Serial.print("\n");
  delay(100);

  //send to node
  LoRa.beginPacket();
  LoRa.print(tempInt);
  //  LoRa.print(humi);
  LoRa.endPacket();
  digitalWrite(5, HIGH);
  delay(50);
  digitalWrite(5, LOW);
  delay(4850);
}
