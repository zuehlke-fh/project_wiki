# Master

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>

// --- WLAN KONFIGURATION ---
const char* ssid = "S23 von Sabrina";
const char* password = "43809441";

// --- JINGLE ESP KONFIGURATION ---
// Die IP des anderen ESPs (Slave)
const char* jingleEspIp = "10.97.71.171"; 

// --- HARDWARE PIN DEFINITIONEN ---
#define VRX_PIN 36      // Joystick X-Achse
#define VRY_PIN 39      // Joystick Y-Achse

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Variable für den Jingle-Trigger
bool triggerJingle = false;

// --- HTML/CSS/JS ---
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
    <title>Saving Christmas</title>
    <link href="https://fonts.googleapis.com/css2?family=Mountains+of+Christmas:wght@700&family=Nunito:wght@400;700;900&family=Share+Tech+Mono&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-deep: #050b14;
            --bg-gradient: radial-gradient(circle at center, #152036 0%, #000000 100%);
            --glass-bg: rgba(20, 25, 40, 0.85);
            --glass-border: rgba(255, 255, 255, 0.15);
            --c-green: #2ecc71; --c-red: #e74c3c; --c-yellow: #f1c40f;
            --c-orange: #e67e22; --c-blue: #3498db; --c-brown: #795548;
            --cell-size: 40px; --grid-gap: 8px; --box-size: 160px;
        }
        * { box-sizing: border-box; user-select: none; }
        body {
            margin: 0; min-height: 100vh;
            background: var(--bg-deep); background-image: var(--bg-gradient);
            color: white; font-family: 'Nunito', sans-serif; overflow: hidden;
            display: flex; align-items: center; justify-content: center;
        }
        #snow-canvas { position: fixed; inset: 0; pointer-events: none; z-index: 1; opacity: 0.6; }
        .vignette { position: fixed; inset: 0; pointer-events: none; z-index: 2; background: radial-gradient(circle, transparent 40%, #000 130%); }

        #start-screen {
            position: fixed; inset: 0; z-index: 500; background: var(--bg-deep); background-image: var(--bg-gradient);
            display: flex; flex-direction: column; align-items: center; justify-content: center; transition: opacity 0.6s;
        }
        .hero-title {
            font-family: 'Mountains of Christmas', cursive; font-size: 5rem;
            background: linear-gradient(to bottom, #ffffff 0%, #9999ff 100%); -webkit-background-clip: text; -webkit-text-fill-color: transparent;
            text-shadow: 0 0 20px rgba(153, 153, 255, 0.5); margin: 0; text-align: center;
        }
        .btn-start {
            margin-top: 50px; background: linear-gradient(180deg, #ff6b6b 0%, #c0392b 100%);
            color: white; border: none; padding: 20px 80px; font-size: 2rem; border-radius: 100px;
            font-family: 'Mountains of Christmas'; font-weight: bold; cursor: pointer;
            box-shadow: 0 8px 0 #8e2a20, 0 0 30px rgba(231, 76, 60, 0.4); transition: 0.15s;
        }
        .btn-start:active { transform: translateY(4px); box-shadow: 0 4px 0 #8e2a20; }
        .hidden { opacity: 0; pointer-events: none; }

        #main-layout {
            display: flex; flex-direction: column; align-items: center; gap: 20px; z-index: 10;
            opacity: 0; transform: scale(0.95); transition: all 0.8s;
        }
        .layout-visible { opacity: 1 !important; transform: scale(1) !important; }

        #ui-bar {
            width: 100%; max-width: 500px; background: #111; border: 1px solid #333; border-radius: 12px;
            padding: 10px 20px; display: flex; justify-content: space-between; align-items: center;
            box-shadow: 0 5px 20px rgba(0,0,0,0.5);
        }
        .status-text { font-family: 'Share Tech Mono'; color: var(--c-yellow); letter-spacing: 1px; }
        .progress-track { width: 150px; height: 8px; background: #222; border-radius: 4px; overflow: hidden; }
        .progress-bar { height: 100%; width: 0%; background: var(--c-green); transition: width 0.5s; }

        #game-board-frame {
            position: relative; padding: 20px; background: rgba(10, 15, 25, 0.8);
            border-radius: 20px; border: 1px solid var(--glass-border); box-shadow: 0 30px 80px rgba(0,0,0,0.6);
        }
        #timer-strip {
            position: absolute; top: 0; left: 20px; right: 20px; height: 3px;
            background: linear-gradient(90deg, transparent, var(--c-red), transparent);
            width: 0%; margin: 0 auto; transition: width 4s linear; opacity: 0;
        }

        #grid { display: grid; grid-template-columns: repeat(10, 1fr); gap: var(--grid-gap); }
        .cell-wrapper { width: var(--cell-size); height: var(--cell-size); position: relative; display: flex; align-items: center; justify-content: center; }

        .bulb {
            width: 100%; height: 100%; border-radius: 50%; background: #1e2530;
            box-shadow: inset 0 -3px 5px rgba(0,0,0,0.5), inset 0 2px 5px rgba(255,255,255,0.1);
            transition: all 0.3s; z-index: 1;
        }
        .color-1 { background: var(--c-green); box-shadow: 0 0 15px var(--c-green); }
        .color-2 { background: var(--c-red); box-shadow: 0 0 15px var(--c-red); }
        .color-3 { background: var(--c-yellow); box-shadow: 0 0 15px var(--c-yellow); }
        .color-4 { background: var(--c-orange); box-shadow: 0 0 15px var(--c-orange); }
        .color-5 { background: var(--c-blue); box-shadow: 0 0 15px var(--c-blue); }
        .color-6 { background: var(--c-brown); opacity: 0.9; }

        .dimmed .bulb { opacity: 0.15; transform: scale(0.6); filter: grayscale(80%); }
        .cursor-ring {
            position: absolute; inset: -6px; border: 3px dashed white; border-radius: 50%;
            z-index: 10; pointer-events: none; animation: rotate 4s linear infinite;
            box-shadow: 0 0 15px rgba(255,255,255,0.5);
        }
        @keyframes rotate { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }

        #reward-overlay {
            position: fixed; inset: 0; background: rgba(0,0,0,0.95); z-index: 9999;
            display: none; flex-direction: column; align-items: center; justify-content: center; opacity: 0; transition: opacity 1s;
        }
        .overlay-visible { display: flex !important; opacity: 1 !important; }
        .win-text { font-family: 'Mountains of Christmas'; font-size: 4rem; color: var(--c-yellow); text-shadow: 0 0 20px var(--c-yellow); margin-bottom: 20px; }
        .servo-text { color: #2ecc71; margin-top: 30px; font-family: 'Share Tech Mono'; font-size: 1.2rem; animation: blink 2s infinite; }
        @keyframes blink { 50% { opacity: 0.5; } }
    </style>
</head>
<body>
    <canvas id="snow-canvas"></canvas>
    <div class="vignette"></div>

    <div id="start-screen">
        <h1 class="hero-title">Rettet<br>Weihnachten!</h1>
        <button class="btn-start" onclick="startGame()">Starten</button>
        <p style="margin-top:20px; color: #888; font-family: monospace;">Warte auf Verbindung...</p>
    </div>

    <div id="main-layout">
        <div id="ui-bar">
            <span id="status-txt" class="status-text">SYSTEM START...</span>
            <div class="progress-track"><div id="prog-bar" class="progress-bar"></div></div>
        </div>

        <div id="game-board-frame">
            <div id="timer-strip"></div>
            <div id="grid"></div>
        </div>
    </div>

    <div id="reward-overlay">
        <div class="win-text">Geschenk geöffnet!</div>
        <div style="font-size: 100px;">🎁</div>
        <p class="servo-text">Servo-Motor Signal gesendet...</p>
    </div>

    <script>
        let gateway = ws://${window.location.hostname}/ws;
        let websocket;

        function initWebSocket() {
            websocket = new WebSocket(gateway);
            websocket.onopen = () => {
                console.log('Verbunden');
                document.querySelector('#start-screen p').innerText = "System Bereit!";
            };
            websocket.onclose = () => { setTimeout(initWebSocket, 2000); };
            websocket.onmessage = (event) => {
                const cmd = event.data;
                if (cmd === "UP") handleHardwareInput('ArrowUp');
                if (cmd === "DOWN") handleHardwareInput('ArrowDown');
                if (cmd === "LEFT") handleHardwareInput('ArrowLeft');
                if (cmd === "RIGHT") handleHardwareInput('ArrowRight');
                if (cmd === "FIX") handleHardwareInput('Enter');
            };
        }
        window.onload = initWebSocket;

        const targetGrid = [
            [0,0,0,0,3, 3,0,0,0,0], [0,0,0,1,1, 1,1,0,0,0], [0,0,1,2,1, 1,4,1,0,0], [0,1,1,1,1, 1,1,1,1,0],
            [0,1,5,1,2, 1,4,1,1,0], [1,1,1,1,1, 1,1,1,1,1], [1,2,1,4,1, 1,5,1,2,1], [1,1,1,1,1, 1,1,1,1,1],
            [0,0,0,0,6, 6,0,0,0,0], [0,0,0,0,6, 6,0,0,0,0]
        ];

        const zones = [
            {x1:0, x2:4, y1:0, y2:4}, {x1:5, x2:9, y1:0, y2:4},
            {x1:0, x2:4, y1:5, y2:9}, {x1:5, x2:9, y1:5, y2:9}
        ];

        let state = { grid: Array(10).fill().map(()=>Array(10).fill(0)), phase: 0, cursor: {x:0, y:0}, memorizing: false, active: false };
        const gridEl = document.getElementById('grid');
        let cellEls = [];

        for(let y=0; y<10; y++) {
            for(let x=0; x<10; x++) {
                let wrap = document.createElement('div'); wrap.className = 'cell-wrapper';
                let bulb = document.createElement('div'); bulb.className = 'bulb';
                wrap.appendChild(bulb); gridEl.appendChild(wrap);
                cellEls.push({ wrap, bulb, x, y });
            }
        }

        function startGame() {
            document.getElementById('start-screen').classList.add('hidden');
            setTimeout(() => document.getElementById('start-screen').style.display = 'none', 600);
            document.getElementById('main-layout').classList.add('layout-visible');
            state.active = true;
            runPhase(0);
        }

        function runPhase(p) {
            // FIX 1: Balken zuerst aktualisieren
            let percent = p * 25;
            if (percent > 100) percent = 100;
            document.getElementById('prog-bar').style.width = percent + "%";

            // FIX 2: Wenn alle Zonen durch sind -> Gewonnen
            if(p >= zones.length) { 
                setTimeout(triggerWin, 250); 
                return; 
            }

            state.phase = p; state.cursor.x = zones[p].x1; state.cursor.y = zones[p].y1; state.memorizing = true;
            document.getElementById('status-txt').innerText = SEKTOR ${p+1}: MERKEN!;
            document.getElementById('status-txt').style.color = "var(--c-yellow)";
            render();

            const timer = document.getElementById('timer-strip');
            timer.style.opacity = 1; timer.style.width = "100%"; timer.style.transition = "none";
            setTimeout(()=> { timer.style.transition = "width 4s linear"; timer.style.width = "0%"; }, 50);

            setTimeout(() => {
                if(state.phase === p) {
                    state.memorizing = false; timer.style.opacity = 0;
                    document.getElementById('status-txt').innerText = SEKTOR ${p+1}: REPARIEREN;
                    document.getElementById('status-txt').style.color = "var(--c-green)";
                    render();
                }
            }, 4000);
        }

        function render() {
            document.querySelectorAll('.cursor-ring').forEach(e => e.remove());
            
            // Schutz vor Absturz: Wenn phase 99 ist, nehmen wir eine leere Zone
            const currentZone = (state.phase < zones.length) ? zones[state.phase] : {x1:-99, x2:-99, y1:-99, y2:-99};
            
            cellEls.forEach(c => {
                const z = currentZone;
                const inZone = c.x >= z.x1 && c.x <= z.x2 && c.y >= z.y1 && c.y <= z.y2;
                
                let isDone = false;
                
                // FIX 3: Absturz im Loop verhindern!
                // Wir iterieren nur so oft wie es Zonen gibt
                let loopLimit = state.phase;
                if(loopLimit > zones.length) loopLimit = zones.length;

                for(let i=0; i < loopLimit; i++) {
                    const dz = zones[i];
                    if(c.x >= dz.x1 && c.x <= dz.x2 && c.y >= dz.y1 && c.y <= dz.y2) isDone = true;
                }

                c.bulb.className = 'bulb'; c.wrap.classList.remove('dimmed');
                let color = 0;
                
                if(state.phase === 99) color = targetGrid[c.y][c.x];
                else if(state.memorizing && inZone) color = targetGrid[c.y][c.x];
                else if(isDone) color = state.grid[c.y][c.x];
                else if(inZone) color = state.grid[c.y][c.x];
                
                if(color > 0) c.bulb.classList.add(color-${color});
                if(!inZone && !isDone && state.phase !== 99) c.wrap.classList.add('dimmed');
                
                if(state.active && !state.memorizing && state.phase < zones.length && c.x === state.cursor.x && c.y === state.cursor.y) {
                    let ring = document.createElement('div'); ring.className = 'cursor-ring'; c.wrap.appendChild(ring);
                }
            });
        }

        function handleHardwareInput(key) {
            if(!state.active || state.memorizing || state.phase >= zones.length) return;
            const z = zones[state.phase];
            if(key === 'ArrowUp' && state.cursor.y > z.y1) state.cursor.y--;
            if(key === 'ArrowDown' && state.cursor.y < z.y2) state.cursor.y++;
            if(key === 'ArrowLeft' && state.cursor.x > z.x1) state.cursor.x--;
            if(key === 'ArrowRight' && state.cursor.x < z.x2) state.cursor.x++;
            if(key === 'Enter') {
                let next = state.grid[state.cursor.y][state.cursor.x] + 1;
                if(next > 6) next = 0;
                state.grid[state.cursor.y][state.cursor.x] = next;
                checkWinCondition();
            }
            render();
        }

        function checkWinCondition() {
            if(state.phase >= zones.length) return;
            const z = zones[state.phase]; let complete = true;
            for(let y=z.y1; y<=z.y2; y++) for(let x=z.x1; x<=z.x2; x++) if(state.grid[y][x] !== targetGrid[y][x]) complete = false;
            
            if(complete) {
                document.getElementById('status-txt').innerText = "OK!";
                setTimeout(() => runPhase(state.phase + 1), 500);
            }
        }

        function triggerWin() {
            console.log("TRIGGER WIN!");
            state.phase = 99; 
            state.active = false; 
            render();
            
            document.getElementById('prog-bar').style.width = "100%";
            document.getElementById('status-txt').innerText = "SYSTEM ONLINE";
            
            const overlay = document.getElementById("reward-overlay");
            overlay.style.display = 'flex';
            
            setTimeout(() => {
                overlay.classList.add("overlay-visible");
                if (websocket && websocket.readyState === WebSocket.OPEN) {
                    websocket.send("WIN");
                }
            }, 50);
        }

        const canvas = document.getElementById('snow-canvas');
        const ctx = canvas.getContext('2d');
        let w, h, particles = [];
        function resize() { w = canvas.width = window.innerWidth; h = canvas.height = window.innerHeight; }
        window.addEventListener('resize', resize); resize();
        for(let i=0; i<60; i++) particles.push({x: Math.random()*w, y: Math.random()*h, r: Math.random()*2+1, s: Math.random()*1+0.5});
        function drawSnow() {
            ctx.clearRect(0,0,w,h); ctx.fillStyle = "rgba(255,255,255,0.7)";
            particles.forEach(p => { ctx.beginPath(); ctx.arc(p.x, p.y, p.r, 0, Math.PI*2); ctx.fill(); p.y += p.s; if(p.y > h) p.y = -5; });
            requestAnimationFrame(drawSnow);
        }
        drawSnow();
        window.addEventListener('keydown', e => handleHardwareInput(e.key));
    </script>
</body>
</html>
)rawliteral";

static inline void broadcastIfClients(const char* msg) {
  if (ws.count() > 0) ws.textAll(msg);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  analogReadResolution(10); 

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  uint32_t startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
    if (millis() - startAttempt > 20000) ESP.restart();
  }
  Serial.println("\nMaster-ESP bereit! IP: " + WiFi.localIP().toString());

  // --- START TEST: Sendet einmal Signal beim Starten ---
  // Wenn du das nicht willst, nimm die nächste Zeile raus
  Serial.println("Initialer Test: Sende Signal an Jingle ESP...");
  triggerJingle = true; 
  // ----------------------------------------------------

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
      AwsFrameInfo info = (AwsFrameInfo)arg;
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        String msg; msg.reserve(len);
        for (size_t i = 0; i < len; i++) msg += (char)data[i];
        
        if (msg == "WIN") {
          Serial.println("WebSocket: WIN empfangen -> Trigger Jingle!");
          triggerJingle = true;
        }
      }
    }
  });

  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send_P(200, "text/html", index_html); });
  server.on("/press", HTTP_GET, [](AsyncWebServerRequest *request){ broadcastIfClients("FIX"); request->send(200, "text/plain", "Button"); });
  server.begin();
}

unsigned long lastMove = 0;
const int debounceTime = 150;
const int TH_LOW  = 150;
const int TH_HIGH = 850;

void loop() {
  ws.cleanupClients();

  if (triggerJingle) {
    triggerJingle = false; 
    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = String("http://") + jingleEspIp + "/win";
      
      Serial.print("Rufe Jingle ESP auf: "); Serial.println(url);
      http.begin(url);
      int httpCode = http.GET();
      
      if(httpCode > 0) {
        Serial.printf("Jingle Status: %d\n", httpCode);
      } else {
        Serial.printf("Jingle Fehler: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    } else {
      Serial.println("Kein WLAN, kann Jingle nicht rufen.");
    }
  }

  if (ws.count() == 0) { delay(10); return; }

  int xVal = analogRead(VRX_PIN);
  int yVal = analogRead(VRY_PIN);

  if (millis() - lastMove > (unsigned long)debounceTime) {
    bool moved = false;
    if (xVal < TH_LOW)  { ws.textAll("LEFT");  moved = true; }
    else if (xVal > TH_HIGH) { ws.textAll("RIGHT"); moved = true; }
    if (yVal < TH_LOW)  { ws.textAll("UP");    moved = true; }
    else if (yVal > TH_HIGH) { ws.textAll("DOWN");  moved = true; }
    if (moved) lastMove = millis();
  }
}