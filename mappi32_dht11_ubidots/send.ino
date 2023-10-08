void sendData()
{
  if (!client.connect(iot_server, port))
  {
    Serial.println("No connection");
    while (!client.connect(iot_server, port))
    {
      Serial.print(".");
      delay(1000);
    }
    return;
  }
  else if (client.connect(iot_server, port))
  {
    Serial.println("Connected");
    Serial.println();

    client.print("POST /api/v1.6/devices/" + DEVICE_LABEL + " HTTP/1.1\r\n");
    client.print("X-Auth-Token: ");
    client.println(token);
    client.print("Content-Type: application/json\r\n");
    client.print("Host: things.ubidots.com\r\n");

    String buff = "";
    
    buff += "{";
    buff += "\"" + temperatureVarLabel  +"\" : " +  temString   +  ", ";
    buff += "\"" + humidityVarLabel  +"\" : " +  humString   +  "";
    buff += "}\r\n";

    int dataLength = buff.length() - 1;
    String dataLengthStr = String(dataLength);


    client.print("Content-Length: ");
    client.println(dataLengthStr);
    client.println();
    client.println(buff);

    Serial.print("POST /api/v1.6/devices/" + DEVICE_LABEL + " HTTP/1.1\r\n");
    Serial.print("X-Auth-Token: ");
    Serial.println(token);
    Serial.print("Content-Type: application/json\r\n");
    Serial.println("Host: things.ubidots.com\r\n");
    Serial.print("Content-Length: ");
    Serial.println(dataLengthStr);
    Serial.println();
    Serial.println(buff);


    while (!client.available());
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
    Serial.println("closing connection");
    Serial.println();
  }
}
