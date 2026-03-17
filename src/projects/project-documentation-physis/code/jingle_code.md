# Jingle

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// --- WLAN KONFIGURATION ---
const char* ssid = "S23 von Sabrina";
const char* password = "43809441";

WebServer server(80);

// ===================== PINS =====================
#define BUZZER_PIN 21

// ULN2003 Inputs
#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

// ===================== BUZZER CONFIG =====================
#define BUZZER_RESOLUTION 8
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define REST       0

const int Q = 250; 
const int E = Q / 2; 
const int H = Q * 2; 

int melody[] = {
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_G4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_D4, NOTE_E4,
  NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_G4, NOTE_G4, NOTE_F4, NOTE_D4, NOTE_C4
};

int durationsMs[] = {
  E, E, Q, E, E, Q,
  E, E, E, E, H,
  E, E, E, E, E, E, E, E,
  E, E, E, E, Q, Q,
  E, E, Q, E, E, Q,
  E, E, E, E, H,
  E, E, E, E, E, E, E, E,
  E, E, E, E, H
};

const int melodyLen = sizeof(melody) / sizeof(melody[0]);
int noteIndex = 0;
bool noteOn = false;
unsigned long noteDeadlineMs = 0;
const unsigned long noteGapMs = 35;

// ===================== STEPPER CONFIG =====================
const int seq[8][4] = {
  {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,1,1,0},
  {0,0,1,0}, {0,0,1,1}, {0,0,0,1}, {1,0,0,1}
};
int seqIndex = 0;
const long STEPS_PER_REV = 4096; 
long stepsDone = 0;
int stepDir = 1;
unsigned long lastStepMs = 0;
unsigned long stepIntervalMs = 3;

// ===================== STATUS MASCHINE =====================
enum ProgramState {
  STATE_IDLE,      // Warten auf Web-Befehl
  STATE_FORWARD,   // Musik + Motor vor
  STATE_WAIT,      // 10 Sekunden Pause
  STATE_BACKWARD,  // Nur Motor zurück
  STATE_RESET      // Zurücksetzen für nächsten Befehl
};

ProgramState currentState = STATE_IDLE;
unsigned long waitStartTime = 0;
bool songFinished = false;
bool rotationFinished = false;

// ===================== FUNKTIONEN: MOTOR & AUDIO =====================

void setCoils(int a, int b, int c, int d) {
  digitalWrite(IN1, a); digitalWrite(IN2, b); digitalWrite(IN3, c); digitalWrite(IN4, d);
}

void stepOnce(int dir) {
  seqIndex += dir;
  if (seqIndex < 0) seqIndex = 7;
  if (seqIndex > 7) seqIndex = 0;
  setCoils(seq[seqIndex][0], seq[seqIndex][1], seq[seqIndex][2], seq[seqIndex][3]);
}

unsigned long totalSongMs() {
  unsigned long sum = 0;
  for (int i = 0; i < melodyLen; i++) sum += (unsigned long)durationsMs[i] + noteGapMs;
  return sum;
}

void startNote(int freq, int durMs) {
  ledcWriteTone(BUZZER_PIN, freq);
  noteOn = true;
  noteDeadlineMs = millis() + (unsigned long)durMs;
}

void stopNote() {
  ledcWriteTone(BUZZER_PIN, 0);
  noteOn = false;
  noteDeadlineMs = millis() + noteGapMs;
}

bool updateBuzzer() {
  unsigned long now = millis();
  if (now < noteDeadlineMs) return false; 
  if (noteOn) {
    stopNote();
    return false; 
  } else {
    if (noteIndex >= melodyLen) return true; // Song fertig
    int f = melody[noteIndex];
    int d = durationsMs[noteIndex];
    startNote(f, d);
    noteIndex++;
    return false;
  }
}

bool updateStepper() {
  if (stepsDone >= STEPS_PER_REV) {
    setCoils(0,0,0,0);
    return true; 
  }
  unsigned long now = millis();
  if (now - lastStepMs < stepIntervalMs) return false;
  lastStepMs = now;
  stepOnce(stepDir);
  stepsDone++;
  if (stepsDone >= STEPS_PER_REV) {
    setCoils(0,0,0,0);
    return true;
  }
  return false;
}

void prepareForwardRun() {
  songFinished = false;
  rotationFinished = false;
  currentState = STATE_FORWARD; // Status wechseln

  // Reset Audio
  noteIndex = 0;
  noteOn = false;
  noteDeadlineMs = 0;
  ledcWriteTone(BUZZER_PIN, 0);

  // Reset Motor
  stepsDone = 0;
  seqIndex = 0;
  stepDir = 1; 
  setCoils(0,0,0,0);

  // Speed
  unsigned long songMs = totalSongMs();
  unsigned long ideal = songMs / (unsigned long)STEPS_PER_REV;
  if (ideal < 2) ideal = 2;
  if (ideal > 15) ideal = 15;
  stepIntervalMs = ideal;
  
  Serial.println("WEB-Befehl empfangen: Starte Vorwärtslauf.");
}

void prepareBackwardRun() {
  rotationFinished = false;
  stepsDone = 0;
  stepDir = -1; // Rückwärts
  Serial.println("Zeit abgelaufen: Starte Rückwärtslauf.");
}

// ===================== WEB SERVER HANDLER =====================

void handleWin() {
  server.send(200, "text/plain", "Jingle Started!");
  Serial.println("WIN aufgerufen! Prüfe Status...");
  
  // Nur starten, wenn er gerade nichts anderes tut (im IDLE Modus ist)
  if (currentState == STATE_IDLE) {
    prepareForwardRun();
  } else {
    Serial.println("System ist bereits beschäftigt!");
  }
}

// ===================== SETUP & LOOP =====================

void setup() {
  Serial.begin(115200);
  
  // Hardware Setup
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setCoils(0,0,0,0);
  
  ledcAttach(BUZZER_PIN, 2000, BUZZER_RESOLUTION);
  ledcWriteTone(BUZZER_PIN, 0);

  // WiFi Setup
  Serial.print("Verbinde mit WLAN...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nJingle ESP verbunden!");
  Serial.print("IP ADRESSE: ");
  Serial.println(WiFi.localIP()); 

  // Server Routen
  server.on("/win", handleWin);
  server.onNotFound([](){ server.send(404, "text/plain", "Nix hier."); });
  server.begin();
  
  Serial.println("Server bereit. Warte auf /win Aufruf...");
  currentState = STATE_IDLE;
}

void loop() {
  server.handleClient(); // Wichtig: Auf Befehle hören

  switch (currentState) {
    
    // ================= WARTEN AUF WEB-BEFEHL =================
    case STATE_IDLE:
      // Macht nichts, bis handleWin() aufgerufen wird
      break;

    // ================= PHASE 1: VORWÄRTS & MUSIK =================
    case STATE_FORWARD:
      if (!rotationFinished) rotationFinished = updateStepper();
      if (!songFinished)     songFinished     = updateBuzzer();

      if (rotationFinished && songFinished) {
        Serial.println("Aktion fertig. Warte 10s...");
        setCoils(0,0,0,0);
        ledcWriteTone(BUZZER_PIN, 0);
        waitStartTime = millis();
        currentState = STATE_WAIT;
      }
      break;

    // ================= PHASE 2: 10 SEKUNDEN WARTEN =================
    case STATE_WAIT:
      if (millis() - waitStartTime >= 10000) {
        prepareBackwardRun();
        currentState = STATE_BACKWARD;
      }
      break;

    // ================= PHASE 3: ZURÜCK DREHEN =================
    case STATE_BACKWARD:
      if (!rotationFinished) rotationFinished = updateStepper();
      if (rotationFinished) {
        Serial.println("Rücklauf beendet.");
        setCoils(0,0,0,0);
        currentState = STATE_RESET;
      }
      break;

    // ================= PHASE 4: RESET =================
    case STATE_RESET:
      Serial.println("Bereit für neuen Befehl.");
      currentState = STATE_IDLE;
      break;
  }
}