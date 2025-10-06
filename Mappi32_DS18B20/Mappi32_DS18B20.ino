#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 23 //pin 23 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
String tempString;
int tempint;


#include <SPI.h>
#include <LoRa.h>
#define ss 15
#define rst 0
#define dio0 27

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  Serial.println("1");
  while (!Serial);
  Serial.println("LoRa Sender");

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
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop(void)
{ 
  baca();
}

void baca()
{
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);
  tempString=String(tempC);
  tempint = int(tempC);
  
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    String datas ="Temperature: "+String(tempint)+"°C";
    Serial.print(datas);
    LoRa.beginPacket();
    LoRa.print(datas);
    LoRa.endPacket();
    Serial.print("\nsent!\n");
    delay(1000);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
    LoRa.beginPacket();
    LoRa.print("Error: Could not read temperature data");
    LoRa.endPacket();
    delay(1000);
  }
}
