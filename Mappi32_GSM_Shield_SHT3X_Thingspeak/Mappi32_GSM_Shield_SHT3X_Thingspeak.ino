/*
  by KMTek Indonesia
  Dapatkan info terupdate dari kami di:
  Website   : https://kmtech.id
  Instagram : https://www.instagram.com/kmtek.indonesia/
  Facebook  : https://www.facebook.com/kmtech.id
  Linkedln  : https://www.linkedin.com/company/kmtek
*/

#define RXD2 16 //setting RX (pengirim) di pin 16/IO16 pada mappi32 untuk komunikasi ke GSM Sield
#define TXD2 17 //setting TX (penerima) di pin 17/IO17 pada mappi32 untuk komunikasi ke GSM Sield
#define SIM Serial2 //membuat variabel SIM untuk Serial2
#include "Wire.h" // memasukkan library Wire.h untuk komunikasi I2C ke Sensor SHT35
#include "SHTSensor.h"

SHTSensor sht; // membuat variabel sht dari library SHT31
// To use a specific sensor instead of probing the bus use this command:
// SHTSensor sht(SHTSensor::SHT3X); //memasukkan library SHT3X

#define api_keyNumber "xxxxxxxxxxxxxxxx" //memasukkan api key number dari dashboard thingspeak

int conLen = 0; // membuat interger conLen sebagai pengkalkulasian Conlen untuk dikirimkan ke dashboard thingspeak
String temperature; // membuat temperature sebagai String untuk menyimpan nilai suhu
String humidity; //membuat humidity sebagai String untuk menyimpan nilai kelembaban

void setup()
{
  Serial.begin(115200);
  while (SIM.available()) //menunggu jika sim tersedia maka akan lanjut ke step selanjutnya
    Serial.println("Mappi32 with Sield SIM is ready"); //print karakter tersebut di serial monitor

  SIM.begin(115200, SERIAL_8N1, RXD2, TXD2); // setting baud rate, protocol serial, dan pin yang telah di definisikan di atas
  Serial.println("     __     __    __  _______   ______   __    ");
  delay(100);
  Serial.println("    / /__  /  | /  / /__  __/  / ____/  / /__  ");
  delay(100);
  Serial.println("   / /'_/ / /|_./ /    / /    / __/    / /'_/  ");
  delay(100);
  Serial.println("  /_/|_| /_/   /_/    /_/    /_____/  /_/|_|   ");
  delay(100);
  Serial.println(" ________________________ Selamat Berkarya!");

  Wire.begin();
  if (sht.init()) {
    Serial.print("init(): success\n");
  } else {
    Serial.print("init(): failed\n");
  }
  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x

  SIM.println("AT+QPOWD");
  delay(1000);
  ShowSerialData();
  Serial.println("Silakan Tunggu Selama 20 detik");
  delay(20000); // jeda waktu 15 detik
}

void loop()
{
  temperature = String(sht.getTemperature(), 1);
  temperature = temperature.toInt();
  Serial.println("Suhu : " + String(temperature) + "°C");
  humidity = String(sht.getHumidity(), 1);
  humidity = humidity.toInt();
  Serial.println("Kelembaban : " + String(humidity) + "RH");

  SendData();
}

void SendData()
{
  
  // --- Pengiriman data dari mappi32 ke GSM Sield yang di teruskan ke dashboard thingspeak --- //
  SIM.read(); //membaca pesan dari GSM Sield
  delay(1000); //jeda waktu 1 detik
  
  SIM.println("AT+CPIN?");
  delay(2000);
  ShowSerialData();

  SIM.println("AT+CGATT=1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QICSGP=1,1,\"iot.1nce.net\",\"\",\"\",1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QIACT=1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QIACT?");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QIOPEN=1,0,\"TCP\",\"api.thingspeak.com\",80,0,1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QISTATE=1,0");
  delay(3000);
  ShowSerialData();

  SIM.println("AT+QISEND=0");
  delay(1000);
  ShowSerialData();

  SIM.println("POST /update.json HTTP/1.1");
  delay(500);
  ShowSerialData();

  SIM.println("Host: api.thingspeak.com");
  delay(500);
  ShowSerialData();

  SIM.println("Content-Type: application/json");
  delay(500);
  ShowSerialData();

  String data = "{\"api_key\":\"" + String(api_keyNumber) + "\",\"field1\":" + String(temperature) + ",\"field2\":" + String(humidity) + "}";
  conLen = data.length();

  SIM.println("Content-Length: " + String(conLen) + "\r\n");
  delay(1000);
  ShowSerialData();

  SIM.println(data);
  delay(1000);
  ShowSerialData();

  delay(1000);

  SIM.write(0x1A);
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QICLOSE=1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QIDEACT=1");
  delay(1000);
  ShowSerialData();

  delay(300000); //delay 300 detik
}

void ShowSerialData()
{
  while (SIM.available() != 0) //pengkondisian jika GSM Shield tersedia maka akan eksekusi ke code berikutnya
    Serial.write(char(SIM.read())); //mengirimkan perintah ke GSM Sield lalu membaca respon daei GSM Sield
}
