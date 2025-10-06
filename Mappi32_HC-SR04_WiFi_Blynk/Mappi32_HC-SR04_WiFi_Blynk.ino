#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "xxxxxxxxxx"
#define BLYNK_DEVICE_NAME "xxxxxxxx"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "xxxxxxx-xxxxxxxxxxxxxxxxxx";//Paste auth token you copied

char ssid[] = "xxxxxx";///Enter your wifi name
char pass[] = "xxxxxxx";// Enter wifi password

BlynkTimer timer;

const int trigPin = 19;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034

long duration;
float distanceCm;

String keadaan;

void sendSensor()
{
  String distanceCmStr = String(distanceCm);
  Blynk.virtualWrite(V5, distanceCmStr); // select your virtual pins accordingly
  Blynk.virtualWrite(V6, keadaan); // select your virtual pins accordingly
  Serial.println(keadaan);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  if (distanceCm > 200)
  {
    keadaan = "kosong";
  }
  else
  {
    keadaan = "isi";
  }

  delay(1000);
  Blynk.run();
  timer.run();
}
