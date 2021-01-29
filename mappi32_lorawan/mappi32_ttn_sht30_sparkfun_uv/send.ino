void sendData()
{
  Serial.println("Send Data");
  Serial.print("payload >> ");
  for (int i = 0; i < sizeof(payload); i++)
  {
    if (payload[i] < 0x10)
    {
      Serial.print("0" + String(payload[i], HEX) + " ");
    }
    else
    {
      Serial.print(String(payload[i], HEX) + " ");
    }
  }
  Serial.println(" << payload");
  LMIC_setTxData2(1, payload, sizeof(payload), 0);
}

void createPayload()
{
  long suhuLong = suhu * 100;
  long kelembabanLong = kelembaban * 100;
  long uvA_Long = uvA * 100;
  long uvB_Long = uvB * 100;
  long uvIndex_Long = uvIndex * 100;

  payload[0] = (byte) ((suhuLong & 0xFF000000) >> 24 );
  payload[1] = (byte) ((suhuLong & 0x00FF0000) >> 16 );
  payload[2] = (byte) ((suhuLong & 0x0000FF00) >> 8  );
  payload[3] = (byte) ((suhuLong & 0X000000FF)       );

  payload[4] = (byte) ((kelembabanLong & 0xFF000000) >> 24 );
  payload[5] = (byte) ((kelembabanLong & 0x00FF0000) >> 16 );
  payload[6] = (byte) ((kelembabanLong & 0x0000FF00) >> 8  );
  payload[7] = (byte) ((kelembabanLong & 0X000000FF)       );

  payload[8] = (byte) ((uvA_Long & 0xFF000000) >> 24 );
  payload[9] = (byte) ((uvA_Long & 0x00FF0000) >> 16 );
  payload[10] = (byte) ((uvA_Long & 0x0000FF00) >> 8  );
  payload[11] = (byte) ((uvA_Long & 0X000000FF)       );

  payload[12] = (byte) ((uvB_Long & 0xFF000000) >> 24 );
  payload[13] = (byte) ((uvB_Long & 0x00FF0000) >> 16 );
  payload[14] = (byte) ((uvB_Long & 0x0000FF00) >> 8  );
  payload[15] = (byte) ((uvB_Long & 0X000000FF)       );

  payload[16] = (byte) ((uvIndex_Long & 0xFF000000) >> 24 );
  payload[17] = (byte) ((uvIndex_Long & 0x00FF0000) >> 16 );
  payload[18] = (byte) ((uvIndex_Long & 0x0000FF00) >> 8  );
  payload[19] = (byte) ((uvIndex_Long & 0X000000FF)       );
}
