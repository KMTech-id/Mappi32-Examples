#include <LiquidCrystal_I2C.h> // library untuk LCD I2C 
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigpin = 26;
const int echopin = 25;

long durasi;
int jarak;


void setup() {
  Serial.begin(9600);
  lcd.init();
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

}

void loop() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  durasi = pulseIn(echopin, HIGH);
  jarak = durasi * 0.037 / 2;
  
  int tinggibadan = 200 - jarak;
  
  Serial.print(tinggibadan);
  Serial.println(" cm");
  
  lcd.setCursor(0, 0);
  lcd.print("Tinggi :");
  lcd.setCursor(9, 0);
  lcd.print(tinggibadan);
  lcd.setCursor(11, 0);
  lcd.print("cm");
  delay(2000);
  lcd.clear();
}
