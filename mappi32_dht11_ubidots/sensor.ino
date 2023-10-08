void readSensor()
{
  Serial.println("Baca Sensor DHT...");
  float tem = dht.readTemperature();
  float hum = dht.readHumidity();
  // cek sensor
  if (isnan(tem) || isnan(hum)) {
    Serial.println(F("Gagal baca DHT sensor!"));
    return;
  }
  temString = String(tem); // ubah nilai variabel "tem" ke bentuk string lalu salin ke dalam variabel "temString"
  humString = String(hum); // ubah nilai variabel "hum" ke bentuk string lalu salin ke dalam variabel "humStirng"
  Serial.print(" Temperature = ");   // print temperature ke serial monitor
  Serial.print(temString);          
  Serial.print(" C");
  Serial.print('\t');
  Serial.print("  Humidity = ");     // print humidity ke serial monitor
  Serial.print(humString);
  Serial.println(" %RH");
}
