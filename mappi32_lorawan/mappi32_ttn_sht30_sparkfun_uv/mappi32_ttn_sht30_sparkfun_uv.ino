#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <WiFi.h>

//static const PROGMEM u1_t NWKSKEY[16] = { 0x56, 0x68, 0xCC, 0xB0, 0x15, 0x00, 0xC9, 0x7C, 0x6E, 0xB1, 0x83, 0x98, 0xF6, 0xE9, 0xD8, 0x08 };
//static const u1_t PROGMEM APPSKEY[16] = { 0x81, 0x3E, 0xF0, 0x64, 0x84, 0x3C, 0x02, 0xE9, 0x10, 0x04, 0xAD, 0x6E, 0x53, 0x80, 0xB3, 0xF4 };
//static const u4_t DEVADDR = 0x26041926 ; // <-- Change this address for every node!

//static const PROGMEM u1_t NWKSKEY[16] = {0x82, 0xA5, 0x50, 0xB5, 0xF9, 0x18, 0x9B, 0x71, 0xCF, 0x72, 0xF5, 0x2A, 0x8C, 0x74, 0x77, 0xC1};
//static const u1_t PROGMEM APPSKEY[16] = {0x30, 0x53, 0x58, 0xD4, 0x6C, 0x70, 0xC8, 0xBE, 0xCE, 0x64, 0x33, 0x5F, 0xF4, 0xA4, 0xAB, 0x50};
//static const u4_t DEVADDR = 0x260B882E ; // <-- Change this address for every node!

static const PROGMEM u1_t NWKSKEY[16] = { 0xCE, 0xBD, 0x0E, 0x7A, 0xB5, 0x35, 0x1D, 0xA0, 0xF7, 0x11, 0xE5, 0x02, 0xA5, 0x42, 0x68, 0x65 };
static const u1_t PROGMEM APPSKEY[16] = { 0xEC, 0x74, 0x60, 0xD0, 0xEB, 0xBE, 0x18, 0x59, 0xF3, 0x2A, 0x2B, 0x6E, 0x14, 0x06, 0xB2, 0x45 };
static const u4_t DEVADDR = 0x2604143F ; // <-- Change this address for every node!

void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

static osjob_t sendjob;

const unsigned TX_INTERVAL = 30;

// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = 15,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 0,
  .dio = {27, 2, 4},
};


// Sensor Suhu dan Kelembaban
#include <SHT3x.h>
SHT3x airSensor;
float suhu, kelembaban;

//Sensor Ultra Violet
#include <SparkFun_VEML6075_Arduino_Library.h>
VEML6075 uvSensor;
float uvA, uvB, uvIndex;

// payload
byte payload[20];



void onEvent (ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      break;
    case EV_RFU1:
      Serial.println(F("EV_RFU1"));
      break;
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
        Serial.println("Received payload >> ");
        for (int i = 0; i < LMIC.dataLen; i++)
        {
          if (LMIC.frame[LMIC.dataBeg + i] < 0x10)
          {
            Serial.print("0" + String(LMIC.frame[LMIC.dataBeg + i], HEX) + " ");
          }
          else
          {
            Serial.print(String(LMIC.frame[LMIC.dataBeg + i], HEX) + " ");
          }
        }
        Serial.println("<< received payload");
      }
      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    default:
      Serial.println(F("Unknown event"));
      break;
  }
}

void do_send(osjob_t* j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Prepare upstream data transmission at the next possible time.

    Serial.println();
    Serial.println();
    bacaSensorUdara();
    bacaSensorUV();
    createPayload();
    sendData();

    Serial.println(F("Packet queued"));
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting"));
  Wire.begin();
  airSensor.Begin();
  if (uvSensor.begin() == false)
  {
    Serial.println("VEML6075 Error");
    while (1)
      ;
  }

  Serial.println(WiFi.macAddress());

  SPI.begin(14, 12, 13, 15);

#ifdef VCC_ENABLE
  // For Pinoccio Scout boards
  pinMode(VCC_ENABLE, OUTPUT);
  digitalWrite(VCC_ENABLE, HIGH);
  delay(1000);
#endif

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
#ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
#else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif

#if defined(CFG_eu868)
  LMIC_setupChannel(0, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 921000000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band

  //  LMIC_setupChannel(0, 920100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(1, 920300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  //  LMIC_setupChannel(2, 920500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(3, 920700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(4, 920900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(5, 921100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(6, 921300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  //  LMIC_setupChannel(7, 921500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
#elif defined(CFG_us915)
  LMIC_selectSubBand(1);
#endif
  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
  LMIC_setDrTxpow(DR_SF7, 20);

  // Start job
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}
