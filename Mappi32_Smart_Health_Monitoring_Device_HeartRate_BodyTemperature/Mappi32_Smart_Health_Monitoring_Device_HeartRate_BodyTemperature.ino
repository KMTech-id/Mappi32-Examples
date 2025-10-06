// --- Include file konfigurasi dan fungsi-fungsi bantuan ---
#include "config.h"
#include "display_functions.h"
#include "helper_functions.h"
#include "wifi_config_page.h"

// --- Definisi Variabel Global ---
// Di sini variabel yang dideklarasikan sebagai 'extern' di config.h diberi nilai awal.
State state = HOME; // Atur state awal ke HOME saat perangkat pertama kali menyala
String configMode = ""; // Mode konfigurasi awal kosong
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Inisialisasi objek display
PulseOximeter pox; // Inisialisasi objek sensor detak jantung
TwoWire WireMLX = TwoWire(1); // Membuat instance I2C kedua (jika diperlukan untuk menghindari konflik)
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // Inisialisasi objek sensor suhu
int currentID = 1; // ID pasien default adalah 1
float lastHeartRate = 0;
float lastSpO2 = 0;
float lastTemp = 0;
unsigned long stateStart = 0; // Pencatat waktu kapan sebuah state dimulai
float hrSum = 0; int hrCount = 0; // Variabel untuk menghitung rata-rata HR
float spo2Sum = 0; int spo2Count = 0; // Variabel untuk menghitung rata-rata SpO2
float tempSum = 0; int tempCount = 0; // Variabel untuk menghitung rata-rata Suhu
unsigned long lastBtnPressTime[4] = {0,0,0,0}; // Array untuk debounce tombol
int btnPins[4] = {BTN_NEXT, BTN_PREV, BTN_SELECT, BTN_BACK}; // Array pin tombol
int i2cErrorCount = 0;
const int maxI2cErrors = 5; // Batas error I2C sebelum perangkat masuk mode error
unsigned long lastSuccessfulRead = 0;
unsigned long tempPosFirstGoodTime = 0; // Timer untuk stabilisasi posisi sensor suhu
unsigned long fingerFirstGoodTime = 0; // Timer untuk stabilisasi jari

// --- Fungsi Setup (dijalankan sekali saat boot) ---
void setup() {
  Serial.begin(115200); // Mulai komunikasi serial untuk debugging

  // Atur mode pin untuk semua tombol sebagai INPUT dengan PULLUP internal
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  
  Wire.begin(); // Mulai bus I2C utama
  delay(100);

  // Inisialisasi layar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED gagal!"); // Pesan error jika OLED tidak terdeteksi
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();

  // Memuat kredensial WiFi dan ThingSpeak dari memori permanen (Preferences)
  preferences.begin("device-config", true); // Buka namespace "device-config" (mode read-only)
  ssid = preferences.getString("ssid", ""); // Baca SSID, jika tidak ada, kembalikan string kosong
  password = preferences.getString("password", ""); // Baca Password
  apiKey = preferences.getString("apiKey", "GANTI_API_KEY"); // Baca API Key, jika tidak ada, gunakan default
  preferences.end(); // Tutup preferences

  // Jika SSID sudah tersimpan, coba hubungkan ke WiFi
  if (ssid.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Menghubungkan ke WiFi: ");
    Serial.println(ssid);
    int wifiTimeout = 0;
    // Tunggu koneksi selama 10 detik (20 * 500ms)
    while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20) {
      delay(500);
      Serial.print(".");
      wifiTimeout++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi Terhubung");
    } else {
      Serial.println("\nWiFi Gagal Terhubung");
    }
  } else {
    Serial.println("Tidak ada kredensial WiFi. Masuk ke mode konfigurasi.");
  }

  // Inisialisasi sensor MAX30100
  if (!pox.begin()) {
    Serial.println("Gagal inisialisasi MAX30100");
    i2cErrorCount++;
  } else {
    pox.setIRLedCurrent(MAX30100_LED_CURR_24MA); // Atur kecerahan LED inframerah
    Serial.println("MAX30100 siap");
  }

  // Inisialisasi sensor MLX90614 pada bus I2C kedua
  WireMLX.begin(25, 26); // Tentukan pin untuk bus I2C kedua (SDA, SCL)
  delay(100);
  if (!mlx.begin(0x5A, &WireMLX)) {
    Serial.println("Gagal inisialisasi MLX90614");
    i2cErrorCount++;
  } else {
    Serial.println("MLX90614 siap");
  }

  lastSuccessfulRead = millis(); // Set timer pembacaan sukses
  showHome(); // Tampilkan layar utama
}

// --- Fungsi Loop (dijalankan berulang kali) ---
void loop() {
  // Penanganan khusus untuk mode AP. Hanya jalankan web server, abaikan yang lain.
  if (state == AP_MODE_SCREEN) {
    webServer.handleClient(); // Proses request HTTP yang masuk
    return; // Keluar dari loop lebih awal
  }

  // Cek jika sensor MAX30100 tidak memberikan data valid dalam rentang waktu tertentu
  if (state != ERROR_STATE) {
    pox.update(); // Perbarui data dari sensor MAX30100
    if (state == MEASURE_HR && (millis() - lastSuccessfulRead > maxNoReadTime)) {
      Serial.println("MAX30100 tidak memberikan data yang valid");
      i2cErrorCount++; // Tambah hitungan error
      // Jika error terlalu banyak, masuk ke ERROR_STATE
      if (i2cErrorCount >= maxI2cErrors) {
         state = ERROR_STATE;
         showError("Sensor HR Error!");
      }
    }
  }

  // --- STATE MACHINE UTAMA ---
  // Logika program akan bergantung pada nilai 'state' saat ini
  switch(state) {
    case HOME: // Di layar utama
      if (btnPressed(BTN_SELECT)) { state = SELECT_ID; showSelectID(); }
      if (btnPressed(BTN_NEXT)) { state = WIFI_CONFIG; showWifiConfig(); }
      if (btnPressed(BTN_PREV)) { state = THINGSPEAK_CONFIG; showThingSpeakConfig(); }
      break;

    case WIFI_CONFIG: // Di menu konfigurasi WiFi
      if (btnPressed(BTN_SELECT)) {
        configMode = "wifi"; // Atur mode untuk web server
        state = AP_MODE_SCREEN;
        startAPMode(); // Mulai mode Access Point
      }
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); }
      break;

    case THINGSPEAK_CONFIG: // Di menu konfigurasi ThingSpeak
      if (btnPressed(BTN_SELECT)) {
        configMode = "thingspeak"; // Atur mode untuk web server
        state = AP_MODE_SCREEN;
        startAPMode();
      }
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); }
      break;

    case SELECT_ID: // Di menu pemilihan ID Pasien
      if (btnPressed(BTN_NEXT)) { currentID = (currentID % 2) + 1; showSelectID(); } // Ganti ID antara 1 dan 2
      if (btnPressed(BTN_PREV)) { currentID = (currentID == 1 ? 2 : 1); showSelectID(); }
      if (btnPressed(BTN_SELECT)) {
        // Cek koneksi I2C sebelum mulai mengukur
        if (checkI2CConnection()) {
          state = WAIT_FOR_FINGER;
          showPlaceFinger();
        } else {
          state = ERROR_STATE;
          showError("I2C Sensor Gagal!");
        }
      }
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); }
      break;
      
    case WAIT_FOR_FINGER: { // Menunggu jari diletakkan di sensor
      // Cek apakah sensor mendeteksi detak jantung (indikasi ada jari)
      if (pox.getHeartRate() > 30.0) {
        if (fingerFirstGoodTime == 0) {
          fingerFirstGoodTime = millis(); // Jika baru terdeteksi, mulai timer
        }
        // Jika jari sudah stabil selama waktu yang ditentukan (fingerStableTime)
        if (millis() - fingerFirstGoodTime > fingerStableTime) {
          Serial.println("Jari terdeteksi stabil!");
          state = COUNTDOWN_HR; // Pindah ke state hitung mundur
          stateStart = millis();
          // Reset variabel untuk pengukuran baru
          hrSum = 0; hrCount = 0; lastHeartRate = 0;
          spo2Sum = 0; spo2Count = 0; lastSpO2 = 0;
          lastSuccessfulRead = millis();
          fingerFirstGoodTime = 0; // Reset timer stabilisasi
          showCountdown("HR Test", countdownTime/1000);
        }
      } else {
        fingerFirstGoodTime = 0; // Jika tidak ada jari, reset timer
      }
      if (btnPressed(BTN_BACK)) {
        fingerFirstGoodTime = 0;
        state = HOME;
        showHome();
      }
    } break;

    case COUNTDOWN_HR: { // Hitung mundur sebelum ukur HR
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); break; }
      // Tampilkan sisa waktu hitung mundur
      int remain = countdownTime/1000 - (millis()-stateStart)/1000;
      if(remain >=0) showCountdown("HR Test", remain);
      // Jika waktu habis, mulai mengukur
      if(millis()-stateStart >= countdownTime){
        state=MEASURE_HR;
        stateStart=millis();
        lastSuccessfulRead = millis();
      }
    } break;

    case MEASURE_HR: { // Proses pengukuran HR & SpO2
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); break; }
      float hrNow = pox.getHeartRate();
      float spo2Now = pox.getSpO2();
      // Kumpulkan data hanya jika valid
      if (hrNow >= 40 && hrNow <= 180) {
        hrSum += hrNow;
        hrCount++;
        lastHeartRate = hrNow;
        lastSuccessfulRead = millis(); // Reset timer watchdog sensor
      }
      if (spo2Now > 50 && spo2Now <= 100) {
        spo2Sum += spo2Now;
        spo2Count++;
        lastSpO2 = spo2Now;
        lastSuccessfulRead = millis();
      }
      // Hitung dan tampilkan progress bar
      float progress = float(millis() - stateStart) / measureTime;
      if (progress > 1.0) progress = 1.0;
      
      // Update tampilan di layar
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Heart Rate & SpO2");
      display.println("");
      display.print("ID: "); display.println(currentID);
      display.print("HR : ");
      display.print(lastHeartRate, 0); display.println(" bpm");
      display.print("SpO2: "); display.print(lastSpO2, 0); display.println(" %");
      int barWidth = int(progress * (SCREEN_WIDTH - 4));
      display.drawRect(2, 50, SCREEN_WIDTH-4, 10, SSD1306_WHITE);
      display.fillRect(2, 50, barWidth, 10, SSD1306_WHITE);
      display.display();
      
      // Jika waktu pengukuran selesai
      if(millis()-stateStart >= measureTime){
        // Hitung rata-rata
        if (hrCount > 0) lastHeartRate = hrSum / hrCount; else lastHeartRate = 0;
        if (spo2Count > 0) lastSpO2 = spo2Sum / spo2Count; else lastSpO2 = 0;
        
        // Cek I2C lagi sebelum lanjut ke pengukuran suhu
        if (checkI2CConnection()) {
          state = WAIT_FOR_TEMP_POS;
          showPositionTemp();
        } else {
          state = ERROR_STATE;
          showError("I2C Sensor Gagal!");
        }
      }
    } break;

    case WAIT_FOR_TEMP_POS: { // Menunggu posisi sensor suhu stabil
      float tempNow = mlx.readObjectTempC();
      float ambientNow = mlx.readAmbientTempC();
      // Posisi dianggap baik jika suhu objek deutlich lebih tinggi dari suhu sekitar
      if (!isnan(tempNow) && (tempNow - ambientNow > 2.0)) {
        if (tempPosFirstGoodTime == 0) {
          tempPosFirstGoodTime = millis(); // Mulai timer
        }
        // Jika posisi sudah stabil selama waktu yang ditentukan
        if (millis() - tempPosFirstGoodTime > tempPosStableTime) {
          Serial.println("Posisi sensor suhu STABIL!");
          state = COUNTDOWN_TEMP; // Lanjut ke hitung mundur
          stateStart = millis();
          tempSum = 0; tempCount = 0; lastTemp = 0; // Reset variabel suhu
          tempPosFirstGoodTime = 0;
          showCountdown("Temp Test", countdownTime/1000);
        }
      } else {
        tempPosFirstGoodTime = 0; // Reset timer jika posisi tidak stabil
      }
      if (btnPressed(BTN_BACK)) {
        tempPosFirstGoodTime = 0;
        state = HOME;
        showHome();
      }
    } break;

    case COUNTDOWN_TEMP: { // Hitung mundur sebelum ukur suhu
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); break; }
      int remain = countdownTime/1000 - (millis()-stateStart)/1000;
      if(remain >=0) showCountdown("Temp Test", remain);
      if(millis()-stateStart >= countdownTime){
        state=MEASURE_TEMP;
        stateStart=millis();
      }
    } break;

    case MEASURE_TEMP: { // Proses pengukuran suhu
      if (btnPressed(BTN_BACK)) { state = HOME; showHome(); break; }
      float tempNow = mlx.readObjectTempC();
      // Kumpulkan data suhu hanya jika dalam rentang yang wajar untuk tubuh manusia
      if (!isnan(tempNow) && tempNow > 31.0 && tempNow < 42.5) {
        tempSum += tempNow;
        tempCount++;
        lastTemp = tempNow;
      }
      // Tampilkan progress bar
      float progress = float(millis() - stateStart) / measureTime;
      if (progress > 1.0) progress = 1.0;

      // Update tampilan di layar
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Suhu Tubuh");
      display.println("");
      display.print("ID: "); display.println(currentID);
      display.print("Temp: "); display.print(lastTemp, 1);
      display.println(" C");
      int barWidth = int(progress * (SCREEN_WIDTH - 4));
      display.drawRect(2, 50, SCREEN_WIDTH-4, 10, SSD1306_WHITE);
      display.fillRect(2, 50, barWidth, 10, SSD1306_WHITE);
      display.display();

      // Jika waktu pengukuran selesai
      if(millis()-stateStart >= measureTime){
        if (tempCount > 0) lastTemp = tempSum / tempCount; // Hitung rata-rata
        else lastTemp = 0;
        state=SHOW_RESULT;
        showResult();
      }
    } break;

    case SHOW_RESULT: // Menampilkan hasil akhir
      // Jika tombol back ditekan, kirim data ke ThingSpeak dan kembali ke HOME
      if(btnPressed(BTN_BACK)){
        kirimThingSpeak(currentID, lastHeartRate, lastSpO2, lastTemp);
        state=HOME;
        showHome();
      }
      break;

    case ERROR_STATE: // Jika terjadi error
      // Jika tombol back ditekan, coba pulihkan I2C dan kembali ke HOME
      if (btnPressed(BTN_BACK)) {
        recoverI2C();
        state = HOME;
        showHome();
      }
      break;
  }

  delay(10); // Jeda singkat untuk stabilitas
}