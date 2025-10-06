/*silakan pilih board yang digunakan*/
//#define mappi32
#define esp32 

#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module

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

#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect a 4.7K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
String datasensor;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);

  Serial.println("1");

  Serial.println(F("DHTxx test!"));

  dht.begin();
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

  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.println("Humidity: " + String(h) + " %");
  Serial.println("Temperature: " + String(t) + "°C");



  datasensor = (String(h) + "," + String(t));
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
