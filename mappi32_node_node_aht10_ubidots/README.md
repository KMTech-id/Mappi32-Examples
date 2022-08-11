# Mappi32 Node to Node AHT10 Ubidots

Sensor yang di butuhkan :
* AHT10 dengan komunikasi I2C

## Deskripsi

Fungsi code ini adalah untuk mengirimkan data sensor AHT10 (suhu dan kelembaban) dari Node 1 ke Node 2 melalui jaringan LoRa dengan metode node to node. Selanjutnya Node 2 akan mengirimkan data ke Platform IoT Ubidots via protokol HTTP.

### Yang Perlu Dilakukan

* Install library **LoRa** dari **Sandeep Mistry**.
* Install library **Adafruit_AHTX0** beserta kelengkapannya.
* Rename **arduino_secrets.h.examples** menjadi **arduino_secrets.h**
* Sesuaikan **SECRET_UNIQUE_ID**, **SECRET_SSID**, **SECRET_SSIDPASSWORD**, **SECRET_TOKEN** pada **arduino_secrets.h**
* Frekuensi dapat disesuaikan dengan mengganti nilai variable **FREQ**.