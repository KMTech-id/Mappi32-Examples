#define BLYNK_TEMPLATE_ID "TMPL6jSLJ4pJJ"
#define BLYNK_TEMPLATE_NAME "SmartHome IOT kit"
#define BLYNK_AUTH_TOKEN "dpu0af30mHSlm4BtRsmALo7m3airWPpc"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <ESP32Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "hoshinova";
char pass[] = "MoonaBiniGwe";
#define DHTPIN 5
#define DHTTYPE DHT11
#define MQ2PIN 32
#define LEDPIN 26
#define BUZZERPIN 18
#define SERVOPIN 19
const int IR_SENSOR_PIN = 33;
DHT dht(DHTPIN, DHTTYPE);
Servo servo;

BlynkTimer timer;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(IR_SENSOR_PIN, INPUT);
  dht.begin();
  servo.attach(SERVOPIN);
  pinMode(MQ2PIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  timer.setInterval(2000L, sendSensorData);
}

void sendSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int sensorValue = analogRead(IR_SENSOR_PIN);
  int gasValue = analogRead(MQ2PIN);
  int ledState = digitalRead(LEDPIN);
  int servoPosition = servo.read();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send sensor data to Blynk
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, gasValue);

  if (gasValue > 3500) { // Adjust threshold as needed
    digitalWrite(BUZZERPIN, HIGH);
    Blynk.logEvent("gas_bocor", "ADA KEBOCORAN GAS!!!!");
    delay(1000); // Increase delay for better noticeability
    digitalWrite(BUZZERPIN, LOW);
  } else {
    digitalWrite(BUZZERPIN, LOW);
  }

  if (sensorValue < 130) {
    Serial.println("Ada API!!!!");
    digitalWrite(BUZZERPIN, HIGH);
    Blynk.logEvent("ada_api_terdeteksi", "ADA KEBAKARAN DIRUMAH!!!!");
    delay(1000); // Increase delay for better noticeability
    digitalWrite(BUZZERPIN, LOW);
  } else {
    digitalWrite(BUZZERPIN, LOW);
  }

  // Display data on Serial monitor
    Serial.println("-------------------------------------------------------");
  Serial.println("|  Temperature (°C)  |  Humidity (%)  |  IR Sensor  |  Gas Value  |  LED  |  Servo Position  |");
  Serial.println("-------------------------------------------------------");
  Serial.print("|        ");
  Serial.print(temperature);
  Serial.print("        |       ");
  Serial.print(humidity);
  Serial.print("        |       ");
  Serial.print(sensorValue);
  Serial.print("        |     ");
  Serial.print(gasValue);
  Serial.print("     |   ");
  Serial.print(ledState == HIGH ? "ON " : "OFF");
  Serial.print("  |       ");
  Serial.print(servoPosition);
  Serial.println("        |");
  Serial.println("-------------------------------------------------------");
}

BLYNK_WRITE(V4) {
  int pinValue = param.asInt();
  digitalWrite(LEDPIN, pinValue);
}

BLYNK_WRITE(V5) {
  int buttonState = param.asInt(); // Read the state of the button (0 or 1)
  if (buttonState == 1) { // Button pressed
    servo.write(0); // Set servo to 0 degrees position (adjust as needed)
  } else {
    servo.write(90); // Set servo to 90 degrees position (adjust as needed)
  }
}

void loop() {
  Blynk.run();
  timer.run();
} 
