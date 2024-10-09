// KMTek
// Karya Merapi Teknologi
// Automatic Clap Activated Lights
// @diazkibidi

int sensor = 32;  // Ubah nilai pin sensor sesuai dengan koneksi hardware
int led1 = 18;    // Ubah nilai pin LED sesuai dengan koneksi hardware


void setup() {
  pinMode(led1, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  int v_sensor = analogRead(sensor);
  Serial.println(v_sensor);

  if (v_sensor > 650) {
    delay(100);

    while (true) {
      digitalWrite(led1, HIGH);
      v_sensor = analogRead(sensor);

      if (v_sensor > 650) {
        break;
      }
    }
  } else {
    digitalWrite(led1, LOW);
  }

  delay(100);
}
