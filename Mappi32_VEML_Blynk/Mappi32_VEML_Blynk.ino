#define BLYNK_TEMPLATE_ID "TMPLa6cIrOrO"
#define BLYNK_DEVICE_NAME "Chart uv"
#define BLYNK_AUTH_TOKEN "yOA0dAZhGalfB-emakusa1KeOlMpFmY3"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv;

#define pinLED  5

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KMTek";   // Nama Hotspot / WiFi
char pass[] = "KMTek123"; // Password WiFi


BlynkTimer timer;


void myTimerEvent()
{
  Blynk.virtualWrite(V5, millis() / 1000);
}


void setup()
{
  digitalWrite (pinLED, OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  Wire.begin();

  if (!uv.begin())
  {
    Serial.println("Unable to communicate with VEML6075.");
    while (1)
      ;
  }
  //connecting to wifi
  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }


  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
void sendSensor()
{
  float Uva = (uv.a());
  float Uvb = (uv.b());

  Serial.println(String(uv.a()));
  Serial.println(String(uv.b()));

  Serial.println(String(uv.a()) + ", " + String(uv.b()));
  delay(250);

  Blynk.virtualWrite(V1, Uva);
  Blynk.virtualWrite(V2, Uvb);
  delay(1000);

  pinMode (pinLED, HIGH);
  delay(250);
  pinMode (pinLED, LOW);
  delay(250);

}
