// How to Make an Automatic Water Dispenser using Mappi32 //
// @yugoyokaii . On Youtube KMTek Indonesia //

#define TRIG_PIN 25  // Pin untuk Trig pada sensor ultrasonik
#define ECHO_PIN 26 // Pin untuk Echo pada sensor ultrasonik
#define RELAY_PIN 5 // Pin untuk relay

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Pastikan relay dalam keadaan mati saat awal
  Serial.begin(9600);
}

void loop() {
  long duration;
  int distance;

  // Mengirimkan sinyal ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Mengukur waktu yang dibutuhkan untuk menerima sinyal kembali
  duration = pulseIn(ECHO_PIN, HIGH);

  // Menghitung jarak (dalam cm)
  distance = duration * 0.034 / 2;

  // Tampilkan jarak pada serial monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Cek jika jarak kurang dari atau sama dengan 3 cm, nyalakan relay
  if (distance <= 3) {
    digitalWrite(RELAY_PIN, HIGH);  // Nyalakan relay
  } else {
    digitalWrite(RELAY_PIN, LOW);
    delay(500);   // Matikan relay
  }

  delay(100); // Tunggu sebentar sebelum melakukan pengukuran berikutnya
}
