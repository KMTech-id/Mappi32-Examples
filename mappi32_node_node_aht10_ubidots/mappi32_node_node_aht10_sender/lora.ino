void lora_init()
{
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(FREQ)) {
    Serial.println("LoRa Failed");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void lora_merge_data()
{
  Serial.println("Merge Data...");
  json_data = "";
  json_data += "{" ;
  json_data += temperature_label; 
  json_data += ":";
  json_data += tem_data;
  json_data += ",";
  json_data += humidity_label; 
  json_data += ":";
  json_data += hum_data;
  json_data += "}";

  Serial.println("json_data = " + json_data);
  
  String lora_data = "*" + unique_id + "," + device_label + "," + json_data + "#";
  Serial.println("lora_data = " + lora_data);
}

void lora_send_data()
{
  Serial.println("Send Data...");
  LoRa.beginPacket();
  LoRa.print(lora_data);
  LoRa.endPacket();
}
