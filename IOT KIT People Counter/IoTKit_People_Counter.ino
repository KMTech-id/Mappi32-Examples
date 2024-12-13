#include <ESP32Servo.h>

// Define pins
#define BUTTON_PIN 23     // Button to increase the number of people
#define IR_PIN 25         // Infrared sensor pin to decrease the number of people
#define SERVO_PIN 18      // Servo control pin

Servo myServo;
int peopleCount = 0;      // To keep track of the number of people
bool buttonState = false; // To store the button state
bool irDetected = false;  // To track if the IR sensor has detected something

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Setup Servo
  myServo.attach(SERVO_PIN);
  myServo.write(0); // Set initial position to 0 degrees

  // Setup Button and Infrared pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button uses INPUT_PULLUP
  pinMode(IR_PIN, INPUT);             // IR sensor pin

  // Display initial count on Serial Monitor
  Serial.print("People: ");
  Serial.println(peopleCount);
}

void loop() {
  // Check for button press to increase the people count
  if (digitalRead(BUTTON_PIN) == LOW && !buttonState) {
    buttonState = true;
    increasePeople();
    delay(500); // Debouncing delay
  } else if (digitalRead(BUTTON_PIN) == HIGH && buttonState) {
    buttonState = false;
  }

  // Check for infrared detection to decrease the people count
  if (digitalRead(IR_PIN) == HIGH && !irDetected) {
    irDetected = true;
    decreasePeople();
    delay(500); // Avoid multiple detections
  } else if (digitalRead(IR_PIN) == LOW && irDetected) {
    irDetected = false; // Reset IR detection state when object moves away
  }
}

// Function to increase the number of people and move the servo
void increasePeople() {
  peopleCount++;
  moveServo();           // Move the servo to 90 degrees and back
  Serial.print("People: ");
  Serial.println(peopleCount);  // Update the count on Serial Monitor
}

// Function to decrease the number of people and move the servo
void decreasePeople() {
  if (peopleCount > 0) {
    peopleCount--;
    moveServo();         // Move the servo to 90 degrees and back
    Serial.print("People: ");
    Serial.println(peopleCount);  // Update the count on Serial Monitor
  }
}

// Function to move the servo
void moveServo() {
  myServo.write(90);    // Move the servo to 90 degrees
  delay(500);           // Hold for 500ms
  myServo.write(0);     // Move the servo back to 0 degrees
}
