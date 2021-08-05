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
#include "SHT31.h" //memasukkan library SHT31 
/*library SHT31 bisa digunakan juga untuk SHT35*/

#define api_keyNumber "XXXXXXXXXXXXXXX" //memasukkan api key number dari dashboard thingspeak
#define SHT31_ADDRESS   0x44 //membuat address SHT31 menjadi 0x44

uint32_t start;
uint32_t stop;

SHT31 sht; // membuat variabel sht dari library SHT31

int conLen = 0; // membuat interger conLen sebagai pengkalkulasian Conlen untuk dikirimkan ke dashboard thingspeak
String temperature; // membuat temperature sebagai String untuk menyimpan nilai suhu
int tempInt; //membuat tempInt sebagai interger untuk menyimpan nilai suhu

void setup()
{
  Serial.begin(115200);
  while (SIM.available()) //menunggu jika sim tersedia maka akan lanjut ke step selanjutnya
    Serial.println("Mappi32 with Sield SIM is ready"); //print karakter tersebut di serial monitor

  SIM.begin(115200, SERIAL_8N1, RXD2, TXD2); // setting baud rate, protocol serial, dan pin yang telah di definisikan di atas
  delay(1000);

  Wire.begin();
  sht.begin(SHT31_ADDRESS);
  Wire.setClock(100000);

  uint16_t stat = sht.readStatus();
  Serial.print(stat, HEX);
  Serial.println();

}

void loop()
{
  // --- Pembacaan sensor SHT35 ---- //
  start = micros();
  sht.read();         // default = true/fast       slow = false
  stop = micros();
  temperature = String(sht.getTemperature(), 1);
  tempInt = temperature.toInt();
  Serial.print("Suhu Sekarang : ");
  Serial.print(tempInt);
  Serial.print("°C");
  Serial.print("\n");
  //  String humi = String(sht.getHumidity(), 1);
  //  Serial.println(humi);
  //  Serial.print("RH");
  //  Serial.print("\n");
  delay(1000); // jeda waktu 1 detik

  // --- Pengiriman data dari mappi32 ke GSM Sield yang di teruskan ke dashboard thingspeak --- //
  SIM.read(); //membaca pesan dari GSM Sield
  delay(15000); //jeda waktu 15 detik

  SIM.println("AT"); // mengirimkan perintah AT Command ke GSM Sield
  delay(2000);
  ShowSerialData(); // memanggil void ShowSerialData() yang nanti akan dijalankan isi code didalamnya

  SIM.println("AT+CPIN?");
  delay(2000);
  ShowSerialData();

  SIM.println("AT+CGATT=1");
  delay(1000);
  ShowSerialData();

  SIM.println("AT+QICSGP=1,1,\"m2minternet\",\"\",\"\",1");
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

  String data = "{\"api_key\":\"" + String(api_keyNumber) + "\",\"field1\":" + String(tempInt) + "}";
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

  SIM.println("AT+QPOWD");
  delay(1000);
  ShowSerialData();

  delay(3000);
}

void ShowSerialData()
{
  while (SIM.available() != 0) //pengkondisian jika GSM Shield tersedia maka akan eksekusi ke code berikutnya
    Serial.write(char(SIM.read())); //mengirimkan perintah ke GSM Sield lalu membaca respon daei GSM Sield
}
