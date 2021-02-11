// dimas

#include <math.h>
#include <SPI.h>
#include <LoRa.h>

float rssi;
float rssi_db;
float pwr;
int percent;

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(921E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
//    Serial.print("No: '");
//    Serial.print("\t");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); Serial.print("\t"); 
      digitalWrite(5, HIGH);
      delay(500);
      digitalWrite(5, LOW);
    }

    rssi = LoRa.packetRssi();
//    rssi_db = (rssi/0.474)+112;
    
    // print RSSI of packet
    Serial.print("RSSI: ");
    Serial.print(rssi);
    Serial.print(" dBm"); Serial.print("\t");

//    Serial.print(rssi_db); Serial.print("\t");
//    src:https://www.adriangranados.com/blog/dbm-to-percent-conversion

    if ((rssi <= 1) && (rssi >= -20)){
      percent = 100;
    }
    else if ((rssi <= -21) && (rssi >= -23)){
      percent = 99;
    }
    else if ((rssi <= -24) && (rssi >= -26)){
      percent = 98;
    }
    else if ((rssi <= -27) && (rssi >= -28)){
      percent = 97;
    }
    else if ((rssi <= -29) && (rssi >= -30)){
      percent = 96;
    }
    else if ((rssi <= -31) && (rssi >= -32)){
      percent = 95;
    }
    else if (rssi == -33){
      percent = 94;
    }
    else if ((rssi <= -34) && (rssi >= -35)){
      percent = 93;
    }
    else if (rssi == -36){
      percent = 92;
    }
    else if (rssi == -37){
      percent = 91;
    }
    else if ((rssi <= -38) && (rssi >= -39)){
      percent = 90;
    }
    else if (rssi == -40){
      percent = 89;
    }
    else if (rssi == -41){
      percent = 88;
    }
    else if (rssi == -42){
      percent = 87;
    }
    else if (rssi == -43){
      percent = 86;
    }
    else if (rssi == -44){
      percent = 85;
    }
    else if (rssi == -45){
      percent = 84;
    }
    else if (rssi == -46){
      percent = 83;
    }
    else if (rssi == -47){
      percent = 82;
    }
    else if (rssi == -48){
      percent = 81;
    }
    else if (rssi == -49){
      percent = 80;
    }
    else if (rssi == -50){
      percent = 79;
    }
    else if (rssi == -51){
      percent = 78;
    }
    else if (rssi == -52){
      percent = 76;
    }
    else if (rssi == -53){
      percent = 75;
    }
    else if (rssi == -54){
      percent = 74;
    }
    else if (rssi == -55){
      percent = 73;
    }
    else if (rssi == -56){
      percent = 71;
    }
    else if (rssi == -57){
      percent = 70;
    }
    else if (rssi == -58){
      percent = 69;
    }
    else if (rssi == -59){
      percent = 67;
    }
    else if (rssi == -60){
      percent = 66;
    }
    else if (rssi == -61){
      percent = 64;
    }
    else if (rssi == -62){
      percent = 63;
    }
    else if (rssi == -63){
      percent = 61;
    }
    else if (rssi == -64){
      percent = 60;
    }
    else if (rssi == -65){
      percent = 58;
    }
    else if (rssi == -66){
      percent = 56;
    }
    else if (rssi == -67){
      percent = 55;
    }
    else if (rssi == -68){
      percent = 53;
    }
    else if (rssi == -69){
      percent = 51;
    }
    else if (rssi == -70){
      percent = 50;
    }
    else if (rssi == -71){
      percent = 48;
    }
    else if (rssi == -72){
      percent = 46;
    }
    else if (rssi == -73){
      percent = 44;
    }
    else if (rssi == -74){
      percent = 42;
    }
    else if (rssi == -75){
      percent = 40;
    }
    else if (rssi == -76){
      percent = 38;
    }
    else if (rssi == -77){
      percent = 36;
    }
    else if (rssi == -78){
      percent = 34;
    }
    else if (rssi == -79){
      percent = 32;
    }
    else if (rssi == -80){
      percent = 30;
    }
    else if (rssi == -81){
      percent = 28;
    }
    else if (rssi == -82){
      percent = 26;
    }
    else if (rssi == -83){
      percent = 24;
    }
    else if (rssi == -84){
      percent = 22;
    }
    else if (rssi == -85){
      percent = 20;
    }
    else if (rssi == -86){
      percent = 17;
    }
    else if (rssi == -87){
      percent = 15;
    }
    else if (rssi == -88){
      percent = 13;
    }
    else if (rssi == -89){
      percent = 10;
    }
    else if (rssi == -90){
      percent = 8;
    }
    else if (rssi == -91){
      percent = 6;
    }
    else if (rssi == -92){
      percent = 3;
    }
    else if ((rssi <= -93) && (rssi >= -100)){
      percent = 1;
    }
    else{
      percent = 0;
    }
    Serial.print("Percentage: ");
    Serial.print(percent);
    Serial.print(" %"); Serial.print("\t");
//    pwr = pow(10, (rssi/10));
//    Serial.print("Power: ");
//    Serial.print(pwr, 7);
//    Serial.println(" mW");

    Serial.print("Status: "); 
    //src: https://www.metageek.com/training/resources/understanding-rssi.html
    if(rssi>=-30){
      Serial.print("Amazing");
    }
    else if((rssi>=-67)&&(rssi<=-31)){
      Serial.print("Very Good");
    }
    else if((rssi>=-70)&&(rssi<=-68)){
      Serial.print("Okay");
    }
    else if((rssi>=-80)&&(rssi<=-71)){
      Serial.print("Not Good");
    }else{
      Serial.print("Unusable");
    }
    Serial.println("");
    
  }
}
