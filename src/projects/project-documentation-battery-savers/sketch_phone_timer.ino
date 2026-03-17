#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include <Stepper.h>


const int stepsPerRevolution = 500;

Stepper myStepper(stepsPerRevolution, 13, 12, 27, 33);


// WiFi credentials
const char* ssid = "EjectorHotspot";
const char* password = "EjectorPassword";

// Web server
AsyncWebServer server(80);

// Timer variables
bool timerRunning = false;
unsigned long timerDuration = 0;
unsigned long timerStartMillis = 0;

// HTML page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      margin: 0;
      height: 100vh;
      background: #121212;
      color: #e0e0e0;
      font-family: Arial, sans-serif;
      display: flex;
      align-items: center;
      justify-content: center;
    }

    .card {
      background: #1e1e1e;
      padding: 25px 30px;
      border-radius: 12px;
      width: 280px;
      box-shadow: 0 10px 25px rgba(0,0,0,0.4);
      text-align: center;
    }

    h2 {
      margin-top: 0;
      margin-bottom: 20px;
      font-weight: normal;
    }

    .inputs {
      display: flex;
      justify-content: space-between;
      margin-bottom: 20px;
    }

    .inputs div {
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    input {
      width: 70px;
      padding: 6px;
      font-size: 16px;
      text-align: center;
      border-radius: 6px;
      border: none;
      margin-top: 5px;
      background: #2a2a2a;
      color: #ffffff;
    }

    input:focus {
      outline: none;
      background: #333;
    }

    button {
      width: 100%;
      padding: 10px;
      font-size: 16px;
      border-radius: 8px;
      border: none;
      background: #3fa9f5;
      color: #000;
      cursor: pointer;
      transition: background 0.2s ease;
    }

    button:hover {
      background: #66b9f7;
    }

    #countdown {
      margin-top: 25px;
      font-size: 36px;
      letter-spacing: 2px;
      font-weight: bold;
    }
  </style>
</head>

<body>
  <div class="card">
    <h2>Countdown Timer</h2>

    <div class="inputs">
      <div>
        <label>Hours</label>
        <input type="number" id="hours" min="0" value="0">
      </div>

      <div>
        <label>Minutes</label>
        <input type="number" id="minutes" min="0" value="0">
      </div>
    </div>

    <button onclick="startTimer()">Start Timer</button>

    <div id="countdown">00:00:00</div>
  </div>

<script>
function startTimer() {
  var hours = parseInt(document.getElementById("hours").value) || 0;
  var minutes = parseInt(document.getElementById("minutes").value) || 0;

  var seconds = (hours * 3600) + (minutes * 60);

  if (seconds < 0) {
    alert("Please enter a valid time.");
    return;
  }

  fetch("/setTimer?seconds=" + seconds);
}

function updateCountdown() {
  fetch("/timeRemaining")
    .then(function(response) {
      return response.text();
    })
    .then(function(seconds) {
      seconds = parseInt(seconds);

      var h = Math.floor(seconds / 3600);
      var m = Math.floor((seconds % 3600) / 60);
      var s = seconds % 60;

      document.getElementById("countdown").innerText =
        String(h).padStart(2, '0') + ":" +
        String(m).padStart(2, '0') + ":" +
        String(s).padStart(2, '0');
    });
}

setInterval(updateCountdown, 1000);
</script>

</body>
</html>
)rawliteral";


void setup() {
  myStepper.setSpeed(60);
  Serial.begin(115200);
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/setTimer", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("seconds")) {
      timerDuration = request->getParam("seconds")->value().toInt();
      timerStartMillis = millis();
      timerRunning = true;

      Serial.print("Timer started for ");
      Serial.print(timerDuration);
      Serial.println(" seconds");

      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Missing parameter");
    }
  });

  server.on("/timeRemaining", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (!timerRunning) {
      request->send(200, "text/plain", "0");
      return;
    }

    unsigned long elapsed = (millis() - timerStartMillis) / 1000;

    if (elapsed >= timerDuration) {
      request->send(200, "text/plain", "0");
    } else {
      request->send(200, "text/plain",
        String(timerDuration - elapsed));
    }
  });

  server.begin();
}

void loop() {
  if (timerRunning) {
    unsigned long elapsed = (millis() - timerStartMillis) / 1000;
    if (elapsed >= timerDuration) {
      timerRunning = false;
      onTimerFinished();
    }
  }
}

void onTimerFinished() {
  Serial.println("TIMER DONE!");

  myStepper.step(stepsPerRevolution);
  delay(500);
  myStepper.step(-stepsPerRevolution);
}
