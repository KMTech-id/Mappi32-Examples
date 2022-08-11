void aht10_init()
{
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(1000);
  }
}

void aht10_read()
{
  Serial.println("Read AHT10 Sensor...");
  sensors_event_t humi, temp;
  aht.getEvent(&humi, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humi.relative_humidity); Serial.println(" % RH");
  tem_data = temp.temperature;
  hum_data = humi.relative_humidity;
  delay(1000);
}
