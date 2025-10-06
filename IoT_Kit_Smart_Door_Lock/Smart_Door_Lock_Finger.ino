#define BLYNK_TEMPLATE_ID "TMPL6nwt67E1G"
#define BLYNK_TEMPLATE_NAME "Intruder Alert"
#define BLYNK_AUTH_TOKEN "CZIqw8-R3arbBvLaoIEJWFPTl1HFFCOB"
#include <HardwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "hoshinova";
char pass[] = "Ichizora";
HardwareSerial mySerial(1);
#define RX_PIN 16
#define TX_PIN 17
#define SERVO_PIN 25
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo doorLockServo;
int authorizedID = -1;
void setup() {
  Serial.begin(115200);
  delay(100);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  mySerial.begin(57600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("\n\nSmart Door Lock System");
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  doorLockServo.attach(SERVO_PIN);
  lockDoor();
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();\
  Serial.println("Place your finger to register as the authorized user.");
  while (authorizedID == -1) {
    authorizedID = enrollFirstFinger();
  }
  Serial.print("Authorized user ID: ");
  Serial.println(authorizedID);
  Serial.println("Smart Door Lock is now active.");
}
void loop() {
  Blynk.run();
  int fingerID = getFingerprintID();
  if (fingerID == -1) {
    // No fingerprint detected, do nothing
    return;
  }

  if (fingerID == authorizedID) {
    Serial.println("Access granted. Unlocking door...");
    unlockDoor();
    delay(10000); // Door remains unlocked for 10 seconds
    lockDoor();
    Serial.println("Door locked.");
  } else {
    Serial.println("Access denied. Sending notification...");
    Blynk.logEvent("maling", "ADA YANG MAU MASUK RUMAH!!!!!");
  }

  delay(500); // Small delay to avoid sensor overload
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    return finger.fingerID;
  } else {
    return -1;
  }
}

int enrollFirstFinger() {
  Serial.println("Waiting for valid finger...");
  while (finger.getImage() != FINGERPRINT_OK) {}

  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    Serial.println("Error capturing finger. Try again.");
    return -1;
  }

  Serial.println("Remove finger and place again.");
  delay(2000);
  while (finger.getImage() != FINGERPRINT_NOFINGER) {}

  while (finger.getImage() != FINGERPRINT_OK) {}
  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    Serial.println("Error capturing finger. Try again.");
    return -1;
  }

  if (finger.createModel() != FINGERPRINT_OK) {
    Serial.println("Fingerprints did not match. Try again.");
    return -1;
  }

  if (finger.storeModel(1) != FINGERPRINT_OK) {
    Serial.println("Error storing fingerprint.");
    return -1;
  }

  Serial.println("Fingerprint enrolled successfully!");
  return 1; // ID of the first enrolled fingerprint
}

void lockDoor() {
  doorLockServo.write(0); // Lock position
  Serial.println("Door locked.");
}

void unlockDoor() {
  doorLockServo.write(90); // Unlock position
  Serial.println("Door unlocked.");
}
