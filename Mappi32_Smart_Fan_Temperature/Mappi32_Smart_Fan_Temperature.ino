#include <DHT.h>

#define DHTPIN 19      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // Define DHT type as DHT11
#define RELAY_PIN 18   // Pin where the relay is connected
#define TEMP_THRESHOLD 36 // Temperature threshold in degrees Celsius

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  pinMode(RELAY_PIN, OUTPUT);  // Set relay pin as output
  digitalWrite(RELAY_PIN, LOW); // Initialize relay as off
}

void loop() {
  delay(2000); // DHT11 sampling rate is 1Hz, 2 seconds delay
  
  // Read temperature and humidity
  float temp = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity(); // Read humidity
  
  // Check if any reading failed
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  // Print temperature and humidity to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  // Check if the temperature is above the threshold
  if (temp >= TEMP_THRESHOLD) {
    digitalWrite(RELAY_PIN, HIGH);  // Turn relay ON
    Serial.println("Relay ON - Temperature too high!");
  } else {
    digitalWrite(RELAY_PIN, LOW);   // Turn relay OFF
    Serial.println("Relay OFF - Temperature normal.");
  }
}
