void ReadStatus() {
  if (windspeed > BATAS_MAX )
  {
    Status = "BAHAYA ANGIN KENCANG\r\n";
    Serial.print("Windspeed = " + windspeedStr + "\tStatus = " + Status);
  }
  else
  {
    Status = "Aman\r\n";
    Serial.print("Windspeed = " + windspeedStr + "\tStatus = " + Status);
    Start = 0;
  }
}
