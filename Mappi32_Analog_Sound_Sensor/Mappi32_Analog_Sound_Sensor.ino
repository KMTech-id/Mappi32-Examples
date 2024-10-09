// KMTek
// Karya Merapi Teknologi
// Accessing Analog Sound Sensor using Mappi32
// @diazkibidi

const int soundSensorPin = A0; // Pin analog untuk sensor suara
int soundValue = 0; // Variabel untuk menyimpan nilai pembacaan sensor suara

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial dengan baud rate 9600
}

void loop() {
  soundValue = analogRead(soundSensorPin); // Membaca nilai dari sensor suara
  Serial.println(soundValue); // Mengirim nilai pembacaan sensor suara ke serial monitor
  delay(1000); // Delay 1 detik sebelum membaca kembali
}
