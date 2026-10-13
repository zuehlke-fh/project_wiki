#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "ESP_Audio";
const char* password = "123456789";

// Custom IP Configuration
IPAddress local_IP(192, 168, 10, 1);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

// Web server on port 80
WebServer server(80);

// These are the pins used
#define VS1053_RESET -1  // VS1053 reset pin (not used!)
// Feather ESP8266
#if defined(ESP8266)
#define VS1053_CS 16   // VS1053 chip select pin (output)
#define VS1053_DCS 15  // VS1053 Data/command select pin (output)
#define CARDCS 2       // Card chip select pin
#define VS1053_DREQ 0  // VS1053 Data request, ideally an Interrupt pin
// Feather ESP32-C6
#elif defined(ARDUINO_ADAFRUIT_FEATHER_ESP32C6)
#define VS1053_CS 6    // VS1053 chip select pin (output)
#define VS1053_DCS 8   // VS1053 Data/command select pin (output)
#define CARDCS 5       // Card chip select pin
#define VS1053_DREQ 7  // VS1053 Data request, ideally an Interrupt pin
// Feather ESP32
#elif defined(ESP32) && !defined(ARDUINO_ADAFRUIT_FEATHER_ESP32S2)
#define VS1053_CS 32    // VS1053 chip select pin (output)
#define VS1053_DCS 33   // VS1053 Data/command select pin (output)
#define CARDCS 14       // Card chip select pin
#define VS1053_DREQ 15  // VS1053 Data request, ideally an Interrupt pin
// Feather Teensy3
#elif defined(TEENSYDUINO)
#define VS1053_CS 3    // VS1053 chip select pin (output)
#define VS1053_DCS 10  // VS1053 Data/command select pin (output)
#define CARDCS 8       // Card chip select pin
#define VS1053_DREQ 4  // VS1053 Data request, ideally an Interrupt pin
// WICED feather
#elif defined(ARDUINO_STM32_FEATHER)
#define VS1053_CS PC7     // VS1053 chip select pin (output)
#define VS1053_DCS PB4    // VS1053 Data/command select pin (output)
#define CARDCS PC5        // Card chip select pin
#define VS1053_DREQ PA15  // VS1053 Data request, ideally an Interrupt pin
#elif defined(ARDUINO_NRF52832_FEATHER)
#define VS1053_CS 30    // VS1053 chip select pin (output)
#define VS1053_DCS 11   // VS1053 Data/command select pin (output)
#define CARDCS 27       // Card chip select pin
#define VS1053_DREQ 31  // VS1053 Data request, ideally an Interrupt pin
// Feather RP2040
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
#define VS1053_CS 8    // VS1053 chip select pin (output)
#define VS1053_DCS 10  // VS1053 Data/command select pin (output)
#define CARDCS 7       // Card chip select pin
#define VS1053_DREQ 9  // VS1053 Data request, ideally an Interrupt pin
// Feather M4, M0, 328, ESP32S2, nRF52840 or 32u4
#else
#define VS1053_CS 6    // VS1053 chip select pin (output)
#define VS1053_DCS 10  // VS1053 Data/command select pin (output)
#define CARDCS 5       // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ 9  // VS1053 Data request, ideally an Interrupt pin
#endif

Adafruit_VS1053_FilePlayer musicPlayer =
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
float playSpeed = 1.5;
File audioFile;
// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
const int potPin = 34;
// Variable for storing the potentiometer value
int potValue = 0;

bool first = true;
// Time tracking variables
unsigned long startTime = 0;
bool isCounting = false;

void handleRoot() {
  File root = SD.open("/");
  if (!root) {
    server.send(500, "text/html", "Failed to open SD card.");
    return;
  }

  String html = R"rawliteral(
    <html>
      <head><title>SD Card Files</title></head>
      <body>
        <h1>Files on SD Card</h1>
        <ul>
  )rawliteral";

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }
    if (!entry.isDirectory()) {
      html += "<li>" + String(entry.name()) + " (" + String(entry.size()) + " bytes) ";
      html += "<button onclick=\"deleteFile('" + String(entry.name()) + "')\">Delete</button></li>";
    } else {
      html += "<li><b>" + String(entry.name()) + "/</b></li>";
    }
    entry.close();
  }
  root.close();

  html += R"rawliteral(
        </ul>
        <h2>Upload a File</h2>
        <form method="POST" action="/upload" enctype="multipart/form-data">
          <input type="file" name="uploadFile">
          <input type="submit" value="Upload">
        </form>
        <script>
          function deleteFile(filename) {
            fetch('/delete?file=' + filename)
              .then(response => response.text())
              .then(data => alert(data))
              .then(() => location.reload());
          }
        </script>
      </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleDelete() {
  if (server.hasArg("file")) {
    String fileName = server.arg("file");

    Serial.print("Attempting to delete: ");
    Serial.println(fileName);

    if (fileName.charAt(0) != '/') {
      fileName = "/" + fileName;  // Ensure leading slash
    }

    if (SD.exists(fileName.c_str())) {
      SD.remove(fileName.c_str());
      server.send(200, "text/plain", "File " + fileName + " deleted.");
    } else {
      server.send(404, "text/plain", "File not found.");
    }
  } else {
    server.send(400, "text/plain", "Bad request.");
  }
}

// Handle file upload
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/" + upload.filename;
    Serial.print("Uploading: "); Serial.println(filename);
    File uploadFile = SD.open(filename, FILE_WRITE);
    if (!uploadFile) {
      server.send(500, "text/plain", "Failed to create file");
      return;
    }
    uploadFile.close();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    File uploadFile = SD.open("/" + upload.filename, FILE_APPEND);
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
      uploadFile.close();
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    server.send(200, "text/plain", "File Uploaded Successfully!");
    Serial.println("Upload complete.");
  }
}


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(1); }
  delay(500);

  Serial.println("Setting up Wi-Fi with static IP...");

  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static IP!");
  }
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Access Point started.");
  Serial.println(WiFi.softAPIP());

  if (!SD.begin(CARDCS)) {
    Serial.println("SD failed, or not present");
    while (1);
  }
  Serial.println("SD OK!");
  printDirectory(SD.open("/"), 0);

  if (!musicPlayer.begin()) {
    Serial.println("Couldn't find VS1053, check pin connections.");
    while (1);
  }
  Serial.println("VS1053 found");
  musicPlayer.setVolume(0, 0);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

  server.on("/delete", HTTP_GET, handleDelete);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_POST, []() {
    server.send(200, "text/html", "<script>window.location='/';</script>");
  }, handleFileUpload);

  server.begin();
  Serial.println("HTTP server started.");
  musicPlayer.sineTest(0x44, 500);
}

void loop() {
  server.handleClient();
  potValue = analogRead(potPin);
  
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  if (potValue > 100) {
    if (!isCounting) {
      isCounting = true;
      startTime = millis();
    } else if (millis() - startTime >= 5000) {
      if(first){
        first = false;
        audioFile = SD.open(getFirstAudioFile());
      }else{
        Serial.print("Current audio file: ");
        Serial.print(audioFile.name());
        audioFile = SD.open(getNextAudioFile(audioFile.name()));
      }
      playTrack();
      isCounting = false;
    }
  } else {
    isCounting = false;
  }
  delay(500);
}

String getFirstAudioFile() {
  File root = SD.open("/");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      Serial.println("No audio files found.");
      root.close();
      return "";  // Return empty string if no audio file is found
    }
    if (!entry.isDirectory()) {
      String filename = entry.name();
      if (filename.endsWith(".mp3") || filename.endsWith(".MP3")) {
        Serial.print("Found audio file: ");
        Serial.println(filename);
        entry.close();
        root.close();
        return "/" + filename;  // Ensure the filename includes the leading slash
      }
    }
    entry.close();
  }
  root.close();
  return "";  // Return empty string if no valid file is found
}

String getNextAudioFile(String currentFile) {
  File root = SD.open("/");
  bool foundCurrent = false;

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      root.close();
      Serial.println("No next audio file found, returning to first file.");
      return getFirstAudioFile();  // Loop back to the first file if none is found
    }

    if (!entry.isDirectory()) {
      String filename = entry.name();
      if (filename.endsWith(".mp3") || filename.endsWith(".MP3")) {
        if (foundCurrent) {
          Serial.print("Next audio file: ");
          Serial.println(filename);
          entry.close();
          root.close();
          return "/" + filename;  // Return next audio file
        }

        if (filename == currentFile) {
          foundCurrent = true;  // Mark current file as found
        }
      }
    }
    entry.close();
  }
  root.close();
  return "";  // Return empty string if no valid file is found
}


void printDirectory(File dir, int numTabs) {
  while (true) {
 
    File entry = dir.openNextFile();
    if (!entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

void playTrack() {
  Serial.println("PlayTrack...");
    musicPlayer.setVolume(100, 15);

  while (audioFile.available()) {
    potValue = analogRead(potPin);
    
    server.handleClient();
 
    if (potValue != 0) {
      if (musicPlayer.readyForData()) {
        uint8_t buffer[32];
        size_t bytesRead = audioFile.read(buffer, sizeof(buffer));
        if (bytesRead == 0) break;
        musicPlayer.playData(buffer, bytesRead);
      }
    } else  {
      
    }
    if (potValue > 700) {
      delay(0);
    } else if (potValue > 500) {
      delay(2);
    } else if (potValue > 400) {
      delay(5);
    } else if (potValue > 200) {
      delay(7);
    } else {
      delay(10);
    }
  }

  Serial.println("Playback finished or stopped.");
  //audioFile.close();
}
