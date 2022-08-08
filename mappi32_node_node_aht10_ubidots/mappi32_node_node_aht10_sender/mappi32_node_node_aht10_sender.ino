//AHT10
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;

float tem_data = 0, hum_data = 0;

//LoRa

#include <SPI.h>
#include <LoRa.h>

#define FREQ 922E0
String unique_id = "F4022918";

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

String lora_data;
String json_data;


//Ubidots Dashboard
String device_label = "mappi32";
String temperature_label = "temp";
String humidity_label = "humi";


void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  Serial.println("### LoRa Node 1 ###");

  //setup LoRa transceiver module
  lora_init();

  //setup aht10 sensor
  aht10_init();
}

void loop() {
  aht10_read();
  lora_merge_data();
  lora_send_data();
  Serial.println("\r\n");
  delay(10000);
}
