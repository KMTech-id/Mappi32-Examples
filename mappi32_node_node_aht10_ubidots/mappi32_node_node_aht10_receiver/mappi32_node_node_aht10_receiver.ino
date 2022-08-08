#include "arduino_secrets.h"

//LoRa

#include <SPI.h>
#include <LoRa.h>

#define FREQ 922000000
String unique_id = "F4022918";
String unique_id_come = "";

float rssi;
float rssi_db;
float pwr;
int percent;

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

String lora_data;
String json_data;
char first_lora_data_char = '*';
char last_lora_data_char = '#';
char separator = '&';

// LED
#define LED 5

// WiFi
#include <WiFi.h>
#include <HTTPClient.h>
int status = WL_IDLE_STATUS;
const char* ssid     = "SSID";
const char* password = "SSIDPASSWORD";
WiFiClient client;

// dashboard
#define HTTP_TIMEOUT 20
char iot_server[] = "http://things.iot.ubidots.com";
const int port = 80;
String token = SECRET_TOKEN;
String device_label = "";

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  while (!Serial);
  Serial.println("\r\n\r\nLoRa Receiver");

  wifiInit();
  lora_init();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // read packet
    while (LoRa.available())
    {
      lora_data = LoRa.readString();
      Serial.println(lora_data);
      calculate_rssi_strength();
    }
//    int will_send_data = parse_data();
//    Serial.println("will_send_data = " + String(will_send_data));
    if(parse_data())
    {
      send_data();
    }
    blink_led();
    Serial.print("\r\n\r\n");
  }
}
