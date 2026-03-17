# Button

#include <WiFi.h>
#include <HTTPClient.h>

// --- WLAN ZUGANGSDATEN ---
const char* ssid = "S23 von Sabrina";
const char* password = "43809441";

// --- IP-ADRESSE DES MASTER-ESP ---
// Schau im Seriellen Monitor des Master-ESP nach, welche IP dort steht!
const char* serverIP = "10.97.71.72"; 

// --- HARDWARE SETUP ---
#define BUTTON_PIN 32 // Pin, an dem der Knopf hängt (Verbindung gegen GND)

void setup() {
  Serial.begin(115200);
  
  // Internen Pullup-Widerstand aktivieren (Knopf schaltet gegen Masse/GND)
  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  // WLAN Verbindung herstellen
  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nButton-ESP bereit und verbunden!");
}

bool lastState = HIGH; // Startzustand ist "nicht gedrückt" (PULLUP)

void loop() {
  bool currentState = digitalRead(BUTTON_PIN);

  // Flankenerkennung: Wenn der Zustand von HIGH auf LOW wechselt (Knopf wurde gedrückt)
  if (currentState == LOW && lastState == HIGH) {
    Serial.println("Knopf gedrückt! Sende Signal an Master...");

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      
      // Die URL, die auf dem Master-ESP den "FIX"-Befehl auslöst
      String url = "http://" + String(serverIP) + "/press";
      
      http.begin(url);
      int httpCode = http.GET(); // HTTP GET Request senden
      
      if (httpCode > 0) {
        Serial.printf("Signal erfolgreich gesendet, Server-Antwort: %d\n", httpCode);
      } else {
        Serial.printf("Fehler beim Senden: %s\n", http.errorToString(httpCode).c_str());
      }
      
      http.end(); // Verbindung schließen
    } else {
      Serial.println("WLAN-Verbindung verloren!");
    }

    // Kurze Pause zum Entprellen des Tasters
    delay(250); 
  }

  // Aktuellen Zustand für den nächsten Durchlauf speichern
  lastState = currentState;
}