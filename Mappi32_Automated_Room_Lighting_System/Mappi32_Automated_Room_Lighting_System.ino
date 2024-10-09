// KMTek
// Karya Merapi Teknologi
// Automated Room Lighting System with Mappi32
// @diazkibidi

const int trigPin = 16;  // Pin Trig sensor ultrasonik
const int echoPin = 17; // Pin Echo sensor ultrasonik
const int lampuPin = 18; // Pin lampu LED

const long jarakAmbang = 30; // Ambang batas jarak dalam cm

bool lampuState = LOW;  // Status lampu (LOW = mati, HIGH = menyala)
bool objekDetectedPrev = LOW; // Status objek sebelumnya

void setup() {
  pinMode(trigPin, OUTPUT); // Set pin Trig sebagai output
  pinMode(echoPin, INPUT);  // Set pin Echo sebagai input
  pinMode(lampuPin, OUTPUT); // Set pin lampu LED sebagai output
  Serial.begin(9600); // Mulai komunikasi serial
}

void loop() {
  // Kirim pulse ke Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Baca waktu pantulan dari Echo
  long duration = pulseIn(echoPin, HIGH);

  // Konversi waktu pantulan menjadi jarak dalam cm
  long jarak = duration * 0.034 / 2;

  // Tampilkan jarak di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  // Deteksi apakah objek terdeteksi (dalam jarak ambang)
  bool objekDetected = jarak < jarakAmbang;

  // Jika objek terdeteksi dan sebelumnya tidak terdeteksi, ubah status lampu
  if (objekDetected && !objekDetectedPrev) {
    lampuState = !lampuState;
    digitalWrite(lampuPin, lampuState);  // Nyalakan atau matikan lampu

    // Tampilkan status lampu di Serial Monitor
    if (lampuState) {
      Serial.println("Objek terdeteksi! Lampu menyala.");
    } else {
      Serial.println("Objek terdeteksi! Lampu mati.");
    }
  }

  // Update status objek sebelumnya
  objekDetectedPrev = objekDetected;

  delay(500); // Tunggu 500ms sebelum mengukur lagi
}
