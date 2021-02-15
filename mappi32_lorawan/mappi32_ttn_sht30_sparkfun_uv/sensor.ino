void bacaSensorUdara()
{
  airSensor.UpdateData();
  suhu = airSensor.GetTemperature();
  kelembaban = airSensor.GetRelHumidity();
  Serial.println("T = " + String(suhu) + " °C | H = " + String(kelembaban) + " %RH");
}

void bacaSensorUV()
{
  uvA = uvSensor.uva();
  uvB = uvSensor.uvb();
  uvIndex = uvSensor.index();

//  while (uvA < 0 || uvB < 0 || uvIndex < 0)
//  {
//    uvA = uvSensor.uva();
//    uvB = uvSensor.uvb();
//    uvIndex = uvSensor.index();
//  }

  Serial.println("UV A = " + String(uvA) + " µW/cm2 | UV B = " + String(uvB) + " µW/cm2 | UV Index = " + String(uvIndex));
}
