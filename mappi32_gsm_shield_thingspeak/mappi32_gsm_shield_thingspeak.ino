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

#define api_keyNumber "9G7H780KFUCBU631" //memasukkan api key number dari dashboard thingspeak
#define nilai "33" // masukkan nilai untuk dikirimkan ke thingspeak

int responSIM;
int conLen = 0; // membuat interger conLen sebagai pengkalkulasian Conlen untuk dikirimkan ke dashboard thingspeak

void setup()
{
  Serial.begin(115200);
  SIM.begin(115200, SERIAL_8N1, RXD2, TXD2); // setting baud rate, protocol serial, dan pin yang telah di definisikan di atas
  delay(1000);
  Serial.print("Mappi32 with Shield SIM\n");
  Serial.print("Waiting");
}

void loop()
{
  SIM.println("AT");
  delay(1000);
  responSIM = int(SIM.read());

  if (responSIM < 1)
  {
    Serial.print(".");
  }

  if (responSIM > 1)
  {
    Serial.print("\n");
    Serial.print("SIM READY");
    delay(2000);

    SIM.println("AT+CPIN?");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+CGATT=1");
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QICSGP=1,1,\"m2minternet\",\"\",\"\",1"); //apn disesuaikan dengan sim card yang digunakan
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
    delay(1000);
    ShowSerialData();

    SIM.println("AT+QISEND=0");
    delay(1000);
    ShowSerialData();

    SIM.println("POST /update.json HTTP/1.1");
    delay(1000);
    ShowSerialData();

    SIM.println("Host: api.thingspeak.com");
    delay(1000);
    ShowSerialData();

    SIM.println("Content-Type: application/json");
    delay(1000);
    ShowSerialData();

    String data = "{\"api_key\":\"" + String(api_keyNumber) + "\",\"field1\":" + String(nilai) + "}";
    conLen = data.length(); //pembacaan conten length secara otomatis

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

    delay(1000);
    Serial.print("\n");
    Serial.print("Waiting");
  }
}

void ShowSerialData()
{
  while (SIM.available() != 0) //pengkondisian jika GSM Shield tersedia maka akan eksekusi ke code berikutnya
    Serial.write(char(SIM.read())); //mengirimkan perintah ke GSM Sield lalu membaca respon daei GSM Sield
}
