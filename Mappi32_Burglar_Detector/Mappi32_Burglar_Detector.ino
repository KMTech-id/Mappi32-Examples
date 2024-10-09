       /* Burglar Detector Using Mappi32 */
      // @diazkibidi // Raffi Diaz Pratama //  

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 26
#define ECHO_PIN 25
#define BUZZER_PIN 19
#define LED_PIN 18
#define DISTANCE_THRESHOLD 50 

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);

  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(LED_PIN, LOW);    

  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Maling Detector!");
  lcd.setCursor(7, 1);
  lcd.print("KMTek!");

  Serial.println("Home Security System Armed");
}

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; 

  return distance;
}

void loop() {
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  lcd.setCursor(2, 2);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm!   ");

  if (distance > 0 && distance < DISTANCE_THRESHOLD) {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);   
    Serial.println("ALERT! Motion Detected!");

    lcd.setCursor(4, 3);
    lcd.print("Ada Orang!!!");

    delay(5000);
  } else {
  
    digitalWrite(BUZZER_PIN, HIGH); 
    digitalWrite(LED_PIN, LOW);    
    Serial.println("Aman Bossss");

    lcd.setCursor(4, 3);
    lcd.print("Aman Bossss!");

    delay(1000);
  }
}
