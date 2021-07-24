#include <SPI.h>
#include <LoRa.h>

float rssi;

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  while (!Serial);
  Serial.println("LoRa Receiver");

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

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      Serial.print("°C");
      Serial.print("\n");
      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
    }

    // print RSSI of packet
    rssi = LoRa.packetRssi();
    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.print(" dBm");
    Serial.print("\n");
  }
}
