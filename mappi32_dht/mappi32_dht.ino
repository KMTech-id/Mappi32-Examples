#include <dht.h>                // memasukkan library "dht.h"
dht sensor;                     // deklarasikan nama sensor
int readResp;                   // deklarasi variabel "readResp"
float hum, tem;                 // deklarasi variabel "hum" dan "tem"
String temString, humString;    // deklarasi variabel "temString" dan "humString"
const int dhtPin = 23;          // deklarasi pin 23 untuk pin DHT sensor

//#define DHT11                 // define "DHT11" jika menggunakan sensor DHT11
#define DHT22                   // define "DHT22" jika menggunakan sensor DHT22
                                // *uncomment yang diperlukan saja

void setup() {
  Serial.begin(115200);         // aktifkan komunikasi serial ke serial monitor dengan kecepatan 115200 bps
}

void loop() {
  Serial.println("Read DHT Sensor...");                   // 

#if defined DHT11
  readResp = sensor.read11(dhtPin); // pembacaan saat menggunakan DHT11 
#else
  readResp = sensor.read22(dhtPin); // pembacaan saat menggunakan DHT22
#endif
 

  tem = sensor.temperature;         // masukkan nilai temperature ke variabel "tem" 
  hum = sensor.humidity;            // masukkan nilai humidity ke variabel "hum"
  
  temString = String(tem);          // ubah nilai variabel "tem" ke bentuk string lalu salin ke dalam variabel "temString"
  humString = String(hum);          // ubah nilai variabel "hum" ke bentuk string lalu salin ke dalam variabel "humStirng"

  Serial.print(" Temperature = ")   // print temperature ke serial monitor
  Serial.print(temString);          
  Serial.print(" C");
  Serial.print('\t');
  Serial.print("  Humidity = ")     // print humidity ke serial monitor
  Serial.print(humString);
  Serial.println(" %RH");
  
  delay(2000);                      // tunda 2000 milidetik untuk pembacaan selanjutnya
}
