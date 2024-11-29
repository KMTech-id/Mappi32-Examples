#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_AHTX0.h>

#define ss 15
#define rst 0
#define dio0 27

// Address of this LoRa device
const int senderAddress = 0x01; // Sender's address
const int receiverAddress = 0x02; // Receiver's address

// Create the AHT10/AHT20 instance
Adafruit_AHTX0 aht;

void setup() {
  Serial.begin(115200);
  Serial.println("LoRa Sender with AHT10/AHT20");

  // Initialize LoRa
  SPI.begin(14, 12, 13, ss);
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  // Initialize AHT10 sensor
  if (!aht.begin()) {
    Serial.println("Could not find AHT10? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 found");
}

void loop() {
  // Variables to store sensor data
  sensors_event_t humidity, temp;
  
  // Get temperature and humidity data
  aht.getEvent(&humidity, &temp);
  float temperature = temp.temperature;
  float relative_humidity = humidity.relative_humidity;

  // Start LoRa packet and send data with address
  LoRa.beginPacket();
  LoRa.write(senderAddress);  // Send sender's address
  LoRa.write(receiverAddress); // Send receiver's address

  // Send temperature and humidity data as strings
  LoRa.print("Temperature: " + String(temperature) + "C");
  LoRa.print(" Humidity: " + String(relative_humidity) + "%");
  LoRa.endPacket();

  Serial.println("Packet sent with Temperature: " + String(temperature) + "C, Humidity: " + String(relative_humidity) + "%");

  // Wait for a while before sending the next packet
  delay(5000); // Send data every 5 seconds
}
