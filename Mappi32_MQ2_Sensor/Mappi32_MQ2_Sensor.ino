int LED = 5;            /*LED pin defined*/
int Sensor_input = 35;    /*Digital pin 5 for sensor input*/
const int pinBuzzer = 12;
void setup() {
  Serial.begin(115200);  /*baud rate for serial communication*/
  pinMode(LED, OUTPUT);  /*LED set as Output*/
  pinMode(12, OUTPUT);
}
void loop() {
  int sensor_Aout = analogRead(Sensor_input);  /*Analog value read function*/
  Serial.print("Gas Sensor: ");
  Serial.print(sensor_Aout);   /*Read value printed*/
  Serial.print("\t");
  Serial.print("\t");
  if (sensor_Aout > 1800) {    /*if condition with threshold 1800*/
    Serial.println("Gas");
    digitalWrite (LED, HIGH) ; /*LED set HIGH if Gas detected */
    digitalWrite (12, LOW) ;
    delay(500);
    digitalWrite (12, HIGH) ;
    delay(100);
    digitalWrite (12, LOW) ;
    delay(100);
    digitalWrite (12, HIGH) ;
    delay(500);
    digitalWrite (12, LOW) ;
    delay(500);
    digitalWrite (12, HIGH) ;
    delay(100);
    digitalWrite (12, LOW) ;
    delay(100);
    digitalWrite (12, HIGH) ;
  }
  else {
    Serial.println("No Gas");
    digitalWrite (LED, LOW) ;  /*LED set LOW if NO Gas detected */
    digitalWrite (12, HIGH) ;
    delay(500);
  }
  delay(1000);                 /*DELAY of 1 sec*/
}
