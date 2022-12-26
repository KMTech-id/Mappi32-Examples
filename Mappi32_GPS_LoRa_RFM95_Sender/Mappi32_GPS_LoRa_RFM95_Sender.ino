
//Syafanida-KMTek

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#define ss 15
#define rst 0
#define dio0 27

int counter = 0;

//pasang pin RX ke 17 dan TX ke 16
#include <TinyGPS.h>

float lat;
float lon;
String paket;
String latString;
String lonString;

TinyGPS gps;

void setup() {

  latString = String(lat);
  lonString = String(lon);
  paket = "#" + latString + "," + lonString + "!";

  Wire.begin();
  Serial.begin(57600); // connect serial
  Serial.println("The GPS Received Signal:");
  Serial2.begin(9600); // connect gps sensor
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
  while (!LoRa.begin(433E6)) {
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


  while (Serial2.available()) { // check for gps data
    if (gps.encode(Serial2.read())) // encode gps data
    {
      gps.f_get_position(&lat, &lon); // get latitude and longitude

      Serial.print("Position: ");

      //Latitude
      Serial.print("Latitude: ");
      Serial.print(lat, 6);

      Serial.print(",");

      //Longitude
      Serial.print("Longitude: ");
      Serial.println(lon, 6);

    }
  }
  LoRa.beginPacket();
  LoRa.print(paket);
  LoRa.endPacket();
  counter++;

  delay(1000);
}
