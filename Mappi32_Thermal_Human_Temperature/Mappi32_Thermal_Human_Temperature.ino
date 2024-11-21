#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test");

  if (!mlx.begin()) {
    Serial.println("Tidak dapat menemukan sensor MLX90614. Periksa koneksi!");
    while (1);
  }
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Tidak dapat memulai SSD1306"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Adafruit MLX90614");
  display.display();
  delay(2000); 
}

void loop() {
  float ambientC = mlx.readAmbientTempC();
  float objectC = mlx.readObjectTempC();
  float ambientF = mlx.readAmbientTempF();
  float objectF = mlx.readObjectTempF();
  
  Serial.print("Ambient = "); Serial.print(ambientC); 
  Serial.print("*C\tObject = "); Serial.print(objectC); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(ambientF); 
  Serial.print("*F\tObject = "); Serial.print(objectF); Serial.println("*F");
  Serial.println();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1); 
  display.print("Ambient: ");
  display.print(ambientC); 
  display.println(" C");
  display.print("Object: ");
  display.print(objectC); 
  display.println(" C");
  display.print("Ambient: ");
  display.print(ambientF); 
  display.println(" F");
  display.print("Object: ");
  display.print(objectF); 
  display.println(" F");
  display.display();
  
  delay(3000);
}