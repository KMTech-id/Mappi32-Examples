void sendTelegram() {
  ReadStatus();
  if (mybot.getNewMessage(msg)) {
    Serial.println("Masuk : " + msg.text);
    String Pesan = msg.text;

    if (Pesan == isipesan && Start == 0)
    {
      mybot.sendMessage(id, Status + windspeedStr + " km/h");
      Serial.println("Terkirim ke telegram");
    }
    if ( Pesan == isipesan && windspeed > BATAS_MAX )
    {
      mybot.sendMessage(id, Status + windspeedStr + " km/h");
      Serial.println("Terkirim ke telegram");
    }
  }

  if (windspeed > BATAS_MAX  && Start == 0)
  {

    mybot.sendMessage(id, Status + windspeedStr + " km/h");
    Serial.print("Peringatan Terkirim ke telegram\n");
    Start = 1;
  }

  else if (windspeed < BATAS_MAX)
  {
    Start = 0;
  }

}
