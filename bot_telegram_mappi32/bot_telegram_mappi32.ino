#include <CTBot.h>
CTBot mybot;
String ssid = "ssid";
String pass= "986pswdr";
String token = "1449400680:AAHQo0JhIs_O8rnaedfV2SqykVHgVyPVixA";
const int id = 1386224063;

#include <math.h>
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27
void setup() {
  Serial.begin(9600);
  Serial.println("Memulai telegram bot. koneksi ke wifi");
  
  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);

  if(mybot.testConnection())
    Serial.println("koneksi berhasil");
  else
    Serial.println("koneksi gagal");
  
  Serial.println("LoRa Receiver");

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
  TBMessage msg ;    
  while (LoRa.available())
    {
  if(mybot.getNewMessage(msg)) // membaca pesan masuk telegram
  {
    //tampilkan di serial monitor
    Serial.println("pesan Masuk : " + msg.text);

    //variabel penampung isi pesan
    String pesan = msg.text;
    if(pesan == "assalamu'alaikum..")
    {
      Serial.print("masuk");

  
    // received a packet
//    Serial.print("No: '");
//    Serial.print("\t");

    // read packet
//data masuk 
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); Serial.print("\t"); 
      mybot.sendMessage(id,LoRaData);
    }
    
      
    }
  }
}
