void setup() 
{
  pinMode(5,OUTPUT);       //define pin 5 as output 
  //(pin 5 adalah jalur LED builtin Mappi32)
}

void loop() 
{
  digitalWrite(5, HIGH);   // set pin 5 bernilai HIGH atau terdapat tegangan
  delay(1000);             //waktu tunda 1 detik
  digitalWrite(5, LOW);    // set pin 5 bernilai LOW atau memutuskan tegangan
  delay(1000);             //waktu tunda 1 detik
}
