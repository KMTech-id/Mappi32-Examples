

#define pintrigger 12
#define pinecho 13
#define BUZZER_PIN 17

unsigned long buzzerMillis = 0; // Untuk pengaturan interval blinking
int buzzerState = LOW; // Status awal buzzer (OFF)

void setup() {
  pinMode(pintrigger, OUTPUT);
  pinMode(pinecho, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Pastikan buzzer mati saat awal
  Serial.begin(115200); // Inisialisasi Serial Monitor
}

void loop() {
  // Mengukur jarak menggunakan sensor ultrasonik
  long duration, distance;
  digitalWrite(pintrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pintrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(pintrigger, LOW);
  duration = pulseIn(pinecho, HIGH);
  distance = (duration * 0.034) / 2; // Menghitung jarak dalam cm

  // Menampilkan jarak ke Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Mengatur interval blinking buzzer berdasarkan jarak
  unsigned long currentMillis = millis();
  int interval = 0; // Interval default

  if (distance <= 40 && distance >= 30) {
    // Jarak 30-40 cm -> Blinking setiap 1000 ms
    interval = 2000;
    Serial.println("HATI-HATI");
  } else if (distance < 30 && distance >= 20) {
    // Jarak 20-30 cm -> Blinking setiap 500 ms
    interval = 1000;
    Serial.println("AWAS! BERHENTI!");
  } else if (distance < 20 && distance >= 10) {
    // Jarak 10-20 cm -> Blinking setiap 100 ms
    interval = 500;
    Serial.println("STOP SEKARANG");
  } else if (distance < 10 && distance >= 5) {
    // Jarak 5-10 cm -> Blinking setiap 50 ms
    interval = 300;
    Serial.println("STOP SEKARANG! JARAK SEMPIT");
  } else {
    // Jika jarak di luar kondisi -> Buzzer OFF
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = LOW;
    Serial.println("Buzzer OFF");
    return; // Keluar dari fungsi loop untuk mematikan buzzer
  }

  // Logika blinking berdasarkan interval
  if (currentMillis - buzzerMillis >= interval) {
    buzzerMillis = currentMillis; // Perbarui waktu terakhir
    buzzerState = !buzzerState;   // Toggle buzzer state (ON/OFF)
    digitalWrite(BUZZER_PIN, buzzerState);
  }
}
