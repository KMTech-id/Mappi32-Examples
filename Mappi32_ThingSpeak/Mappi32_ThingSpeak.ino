#include <WiFi.h>           // memasukkan library WiFi
#include <HTTPClient.h>     // memasukkan library HTTP Client

String apiKey = "xxxxxxxxxxxxxxxx";     // deklarasi API Key
const char *ssid =  "SSID";             // deklarasi nama SSID (HOTSPOT)
const char *pass =  "SSIDPASSWORD";     // deklarasi SSID PASSWORD
char server[] = "api.thingspeak.com";   // deklarasi alamat server
const int port = 80;                    // deklarasi port server

int status = WL_IDLE_STATUS;            // deklarasi WiFi status
WiFiClient client;

String value1, value2;                  // deklarasi nama variabel yang akan dikirim

void setup() {
  Serial.begin(115200);                 // mengaktifkan serial pada kecepatan 115200 bps
  delay(100);                           // tunda 100 milidetik untuk persiapan serial
  Serial.println("START...");           // print "START..." ke serial monitor
  wifiInit();                           // panggil fungsi "wifiInit()"
}

void loop() {
  getValue();                           // panggil fungsi "getValue()"
  sendData();                           // panggil fungsi "sendData()"
  delay(5000);                          // tunda 5000 milidetik untuk pembacaan dan pengiriman selanjutnya
}

void wifiInit()                             // fungsi tambahan bernama "wifiInit()"
{
  delay(1000);                              // tunda 1000 milidetik
  Serial.print("Connecting to " + *ssid);   // print proses koneksi ke hotspot
  delay(500);                               // tunda 500 detik
  status = WiFi.begin(ssid, pass);          // memulai koneksi WiFi

  while (WiFi.status() != WL_CONNECTED)     // ulangi terus selama WiFi belum terkoneksi
  {
    delay(1000);                            // tunda 1000 milidetik
    Serial.print(".");                      // print simbol titik "."
  }
  Serial.println();                         // print baris baru
  Serial.println("WiFi connected");         // print tanda WiFi terkoneksi
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());           // print IP lokal
}

void getValue()                             // fungsi tambahan bernama "getValue()"
{
  Serial.println("DUMMY DATA");             // print "DUMMY DATA"
  randomSeed(analogRead(A0));               // mendapatkan angka awal untuk random data dari pembacaan acak pin Analog 0
  float randNumber;                         // deklarasi variabel "randNumber"    

  randNumber = random(250, 331);            // mendapatkan angka acak dari angka 250 - 331
  value1 = String(randNumber / 10);         // nilai variabel "randNumber" dibagi 10 lalu di ubah dalam bentuk String lalu nilai disalin ke variabel "value1" 
  Serial.println("value1 = " + value1);     // print isi dari variabel "value1"

  randNumber = random(610, 851);            // mendapatkan angka acak dari angka 610 - 851 
  value2 = String(randNumber / 10);         // nilai variabel "randNumber" dibagi 10 lalu di ubah dalam bentuk String lalu nilai disalin ke variabel "value2" 
  Serial.println("value2 = " + value2);     // print isi dari variabel "value2"

  Serial.println();
  Serial.println();
}

void sendData()
{
  if (!client.connect(server, port))          // jika belum terkoneksi ke server
  {
    Serial.println("No connection");          // print "No connection"
    while (!client.connect(server, port))     // ulang terus jika  belum terkoneksi ke server
    {
      Serial.print(".");
      delay(1000);
    }
    return;                                   
  }
  else if (client.connect(server, port))      // jika sudah terkoneksi
  {
    String dataStr = apiKey;                  // deklarasi variabel string "dataStr" lalu salin isi "apiKey" kedalamnya
    dataStr += "&field1=";                    // tambahkan string "&field1="
    dataStr += value1;                        // tambahkan isi dari variabel "value1"
    dataStr += "&field2=";                    // tambahkan string "&field2="
    dataStr += value2;                        // tambahkan isi dari variabel "value2"
    dataStr += "\r\n\r\n";                    // tambahkan karakter <CR><NL><CR><NL> (CR = Carriage Return, NL = New Line)

    client.print("POST /update HTTP/1.1\n");                              // kirim string "POST /update HTTP/1.1\n" ke server
    client.print("Host: api.thingspeak.com\n");                           // kirim string "Host: api.thingspeak.com\n" ke server
    client.print("Connection: close\n");                                  // kirim string "Connection: close\n"
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");                 // kirim string "X-THINGSPEAKAPIKEY: " + apiKey + "\n"
    client.print("Content-Type: application/x-www-form-urlencoded\n");    // kirim string "Content-Type: application/x-www-form-urlencoded\n"
    client.print("Content-Length: ");
    client.print(dataStr.length());                                       // kirim panjang data
    client.print("\n\n");                                                 // kirim <NL><NL>
    client.print(dataStr);                                                // kirim data

    while (!client.available());                                          // tunggu balasan server
    while (client.available()) {                                          // ulangi proses selama ada data balasan dari server
      char c = client.read();                                             // salin data balasan dari server per karakter
      Serial.write(c);                                                    // write isi variabel "c" ke serial monitor
    }

    Serial.println();
    Serial.println("DATA STRING =");
    Serial.println(dataStr);                                              // print isi data
    Serial.println("SENDED");

    client.stop();                                                        // stop koneksi ke server
    Serial.println("closing connection");                                 // print "closing connection"
    Serial.println();
  }
}
