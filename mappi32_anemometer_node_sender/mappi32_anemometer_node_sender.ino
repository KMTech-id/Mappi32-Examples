//Mappi32 by KMTek
// Yogyakarta - Indonesia


const int RecordTime = 1;  //Define Measuring Time (Seconds)
const int SensorPin = 23;  //Define Interrupt Pin (GPIO23 @Mappi32)

int InterruptCounter;
float WindSpeed;

#include <SPI.h>
#include <LoRa.h>
#define ss 15
#define rst 0
#define dio0 27

void setup()
{
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
  while (!LoRa.begin(921E6)) //Sesuaikan dengan modul LoRa yang digunakan
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3); //Harus di samakan antara Sender dan Receiver
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  meassure();
  Serial.print("Wind Speed: ");
  Serial.print(WindSpeed);       //Speed in km/h
  Serial.print(" km/h - ");
  //  Serial.print(WindSpeed / 3.6); //Speed in m/s
  //  Serial.println(" m/s");
  LoRa.beginPacket();
  LoRa.print(WindSpeed);
  LoRa.endPacket();
  //  Serial.print("Sent");
  delay(2000);
}

void meassure() {
  InterruptCounter = 0;
  attachInterrupt(digitalPinToInterrupt(SensorPin), countup, RISING);
  delay(1000 * RecordTime);
  detachInterrupt(digitalPinToInterrupt(SensorPin));
  WindSpeed = (float)InterruptCounter / (float)RecordTime * 2.4;
}

void countup() {
  InterruptCounter++;
}
