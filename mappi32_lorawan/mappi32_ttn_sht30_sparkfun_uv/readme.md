#  Mappi32 TTN SHT30 & Sparkfun UV


Dokumentasi Mappi32 Mengkoneksikan TTN SHT30 & Sparkfun UV

## Spesifikasi
Frequency plan TTN: Asia 915-928 MHz (AS923 Group 1) with only default channels

LoRaWAN version: LoRaWAN Specification 1.0.3

Activation Mode : ABP


##
Frequensi : 923200000

## Installation 
1. sambungkan Gateway ke Gateway TTN
2. Jika sudah tersambung buatlah sebuah Aplications di TTN
4. Buat juga sebuah end Device dengan Spesifikasi diatas
3. Pindah semua yang ada di folder Libraries ke folder library arduino (Default: "Documents\Arduino\libraries") timpa saja jika sebelumnya tidak ada yang di ubah, jika pernah dirubah cari file bernama lmic_project_config.h samakan dengan yang ada di github
4. buka mappi32_ttn_sht30_sparkfun_uv.ino dan Ganti NWKSKEY, APPSKEY, dan DEVADDR, sesuaikan dengan informasi yang ada di end device
5. Upload

## NB
- Untuk Developing enable Resets frame counter
