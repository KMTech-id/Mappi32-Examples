// sender

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module

/*silakan pilih board yang digunakan*/
//#define mappi32
#define esp32

#ifdef esp32
#define ss 5
#define rst 14
#define dio0 2
#endif

#ifdef mappi32
#define ss 15
#define rst 0
#define dio0 27
#endif

int counter = 0;

String datasensor;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  Serial.println("1");

  while (!Serial);
  Serial.println("LoRa Sender");

#ifdef mappi32
  SPI.begin(14, 12, 13, 15);
#endif

  //setup LoRa transceiver module
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

  datasensor = 29; // data pembacaan sensor
  Serial.println(datasensor);
  Serial.print("Sending packet: ");
  Serial.println(datasensor);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(datasensor);
  LoRa.endPacket();

  counter++;

  delay(10000);
}
