void send_data()
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
    Serial.println("Server Connected");
    Serial.println();


    String buff = "";
    buff = json_data;

    int dataLength = buff.length();
    String dataLengthStr = String(dataLength);

    client.print("POST /api/v1.6/devices/" + device_label + " HTTP/1.1\r\n");
    client.print("Host: things.ubidots.com\r\n");
    client.print("X-Auth-Token: ");
    client.println(token);
    client.print("Content-Type: application/json\r\n");

    
    client.print("Content-Length: ");
    client.println(dataLengthStr);
    client.println();
    client.println(buff);


    Serial.print("POST /api/v1.6/devices/" + device_label + " HTTP/1.1\r\n");
    Serial.print("Host: things.ubidots.com\r\n");
    Serial.print("X-Auth-Token: ");
    Serial.println(token);
    Serial.print("Content-Type: application/json\r\n");
    Serial.print("Content-Length: ");
    Serial.println(dataLengthStr);
    Serial.println();
    Serial.println(buff);

    int timeout_count = 0;
    while (!client.available())
    {
      if(timeout_count>=HTTP_TIMEOUT)
      {
        break;
      }
      timeout_count++;
      delay(1000);
    }
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
    Serial.println("closing connection");
    Serial.println();
  }
}
