int SensorPin = 39;
int soilMoistureValue;
int soilmoisturepercent;
void setup() {
  Serial.begin(115200); // open serial port, set the baud rate to 9600 bps
}
void loop() {
soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, 900, 620, 0, 100);
if (soilmoisturepercent >=100){
    soilmoisturepercent=100;

  }
  else if (soilmoisturepercent <=0){
    soilmoisturepercent=0;
  }
  Serial.print("Presentase kelembaban tanah= ");
  Serial.print(soilmoisturepercent);
  Serial.print("%");
  if (soilmoisturepercent >=0 && soilmoisturepercent<=30){
    Serial.println("Tanah Kering");
  }
  else if (soilmoisturepercent >30 && soilmoisturepercent<=60){
    Serial.println("Tanah kondisi normal");
  }
  else if(soilmoisturepercent >60 && soilmoisturepercent<=100){
    Serial.println("Tanah basah");
  }
  delay(3000);
}
