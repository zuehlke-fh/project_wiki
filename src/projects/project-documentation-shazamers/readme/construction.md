[⬅ Back to Main Page](README.md)



# Construction: [Title]



## Description

Add a detailed explanation of this subsection here.



The construction of the Shazamers prototype started with setting up the Raspberry Pi as the central control unit. A push button was connected directly to the Raspberry Pi using a GPIO pin. This button is used to start and stop the audio recording process.



In parallel, an ESP32 microcontroller was prepared and connected to a TFT display. The ESP32 was programmed to connect to a Wi-Fi network and listen for incoming UDP messages. These messages are later used to display song lyrics on the screen.



Next, the audio hardware was set up. A headset with an integrated microphone was connected to the Raspberry Pi. The microphone is used for audio recording, while the headset speakers are used for audio playback. This setup was chosen to avoid audio feedback and input/output conflicts.



After the hardware setup, the software environment on the Raspberry Pi was prepared. Node.js and the required libraries were installed. A custom Node.js script was written to handle button input, audio recording, API communication, audio playback, and UDP communication with the ESP32.



The recording process was implemented so that pressing the button starts the audio recording and pressing it again stops the recording. The recorded audio file is then processed and sent to the Shazam API for song recognition.



Once a song is recognized, the Raspberry Pi searches for the corresponding audio stream and plays it through the headset. At the same time, synchronized lyrics are fetched from the LRCLIB API. These lyrics are sent line by line via UDP to the ESP32, where they are displayed on the TFT screen.





## Content



SongInSongOut.ino Code:



\#include <WiFi.h>

\#include <WiFiUdp.h>

\#include <TFT\_eSPI.h>



const char\* ssid = "Galaxy S22 1B2C";

const char\* password = "aord2010";



WiFiUDP udp;

const unsigned int localPort = 4210;

char packetBuffer\[512];



TFT\_eSPI tft = TFT\_eSPI();



void setup() {

&nbsp; Serial.begin(115200);



&nbsp; pinMode(15, OUTPUT);

&nbsp; digitalWrite(15, HIGH);



&nbsp; tft.init();

&nbsp; tft.setRotation(1);

&nbsp; tft.fillScreen(TFT\_BLACK);

&nbsp; 

&nbsp; // Zeilenumbruch

&nbsp; tft.setTextWrap(true, true); 

&nbsp; 

&nbsp; tft.setTextColor(TFT\_WHITE);

&nbsp; tft.setTextDatum(MC\_DATUM);

&nbsp; tft.drawString("WLAN verbinden...", 160, 85, 4);



&nbsp; WiFi.begin(ssid, password);

&nbsp; while (WiFi.status() != WL\_CONNECTED) {

&nbsp;   delay(500);

&nbsp;   Serial.print(".");

&nbsp; }



&nbsp; tft.fillScreen(TFT\_BLACK);

&nbsp; tft.setTextColor(TFT\_GREEN);

&nbsp; tft.drawString("Bereit!", 160, 60, 4);

&nbsp; tft.setTextColor(TFT\_YELLOW);

&nbsp; tft.drawString(WiFi.localIP().toString(), 160, 110, 2);



&nbsp; udp.begin(localPort);

}



void loop() {

&nbsp; int packetSize = udp.parsePacket();

&nbsp; if (packetSize) {

&nbsp;   int len = udp.read(packetBuffer, 511);

&nbsp;   if (len > 0) packetBuffer\[len] = 0;



&nbsp;   String lyric = String(packetBuffer);



&nbsp;   tft.fillScreen(TFT\_BLACK);

&nbsp;   tft.setTextColor(TFT\_CYAN);



&nbsp;   if (lyric.length() <= 12) {

&nbsp;     tft.setTextSize(4);

&nbsp;     tft.setTextDatum(MC\_DATUM);

&nbsp;     tft.drawString(lyric, 160, 85);

&nbsp;   } 

&nbsp;   else {

&nbsp;     tft.setTextSize(3);

&nbsp;     tft.setCursor(10, 30);

&nbsp;     tft.print(lyric);    

&nbsp;   }

&nbsp;   

&nbsp;   Serial.println("Empfangen: " + lyric);

&nbsp; }

}



------------------------------------------------------------------------------------------------------------------------------------



songToLyricsOut.js Code: 



const fs = require('fs');

const recorder = require('node-record-lpcm16');

const fetch = require('node-fetch');

const FormData = require('form-data');

const dgram = require('dgram');

const { Gpio } = require('pigpio');

const { exec, spawn } = require('child\_process');



const SHAZAM\_KEY = "NV6MpcnCMbZnTwgpEqe7ciDc7yqRLHqls2M0a4sjwkFOoFFUhAH7az9fWR5AqJRn";

const ESP32\_IP = "10.80.241.32";

const UDP\_PORT = 4210;

const filePath = './recording.wav';

const BUTTON\_GPIO = 17;



// aplay -l hw:1,0 oder hw:2,0

const AUDIO\_DEVICE = "alsa/hw:2,0"; 

//const AUDIO\_DEVICE = "auto";



let recording = null;

let fileStream = null;

let isRecording = false;

let lastPress = 0;

let currentPlayerProcess = null;

let lyricInterval = null;



const button = new Gpio(BUTTON\_GPIO, { mode: Gpio.INPUT, pullUpDown: Gpio.PUD\_UP, alert: true });

const udpClient = dgram.createSocket('udp4');



function sendToESP\_UDP(text) {

&nbsp;   const message = Buffer.from(text);

&nbsp;   udpClient.send(message, 0, message.length, UDP\_PORT, ESP32\_IP, (err) => {

&nbsp;       if (err) console.error("UDP Fehler:", err);

&nbsp;   });

}



function delay(ms) {

&nbsp;   return new Promise(resolve => setTimeout(resolve, ms));

}



function parseTime(timeStr) {

&nbsp;   const match = timeStr.match(/\\\[(\\d+):(\\d+\\.\\d+)\\]/);

&nbsp;   if (!match) return null;

&nbsp;   const minutes = parseInt(match\[1]);

&nbsp;   const seconds = parseFloat(match\[2]);

&nbsp;   return Math.floor((minutes \* 60 + seconds) \* 1000);

}



function cleanUmlauts(text) {

&nbsp;   return text

&nbsp;       .replace(/ä/g, 'ae').replace(/ö/g, 'oe').replace(/ü/g, 'ue')

&nbsp;       .replace(/Ä/g, 'Ae').replace(/Ö/g, 'Oe').replace(/Ü/g, 'Ue')

&nbsp;       .replace(/ß/g, 'ss');

}







function playMusic(title, artist) {

&nbsp;   return new Promise((resolve, reject) => {

&nbsp;       if (currentPlayerProcess) currentPlayerProcess.kill();

&nbsp;       if (lyricInterval) clearInterval(lyricInterval);



&nbsp;       const query = `${title} ${artist} official audio`;

&nbsp;       console.log(`Suche Stream für: ${query}`);



&nbsp;       const cmd = `yt-dlp -f bestaudio -g "ytsearch1:${query}"`;



&nbsp;       exec(cmd, (error, stdout) => {

&nbsp;           if (error) return reject(error);



&nbsp;           const streamUrl = stdout.trim();

&nbsp;           console.log("Stream gefunden. Initialisiere Audio-Ausgabe...");



&nbsp;           currentPlayerProcess = spawn('mpv', \[

&nbsp;               '--no-video',

&nbsp;               `--audio-device=${AUDIO\_DEVICE}`,

&nbsp;               '--volume=100',

&nbsp;               streamUrl

&nbsp;           ]);



&nbsp;           currentPlayerProcess.stdout.on('data', (data) => {

&nbsp;               const msg = data.toString();

&nbsp;               if (msg.includes("AO:") || msg.includes("Audio out")) {

&nbsp;                   console.log("Musik startet! Synchronisiere Lyrics...");

&nbsp;                   resolve(); 

&nbsp;               }

&nbsp;           });



&nbsp;           currentPlayerProcess.on('error', (err) => reject(err));

&nbsp;       });

&nbsp;   });

}





async function startLyricsStreaming(title, artist) {

&nbsp;   console.log(`Suche Lyrics für: ${title}...`);

&nbsp;   const url = `https://lrclib.net/api/search?artist\_name=${encodeURIComponent(artist)}\&track\_name=${encodeURIComponent(title)}`;

&nbsp;   

&nbsp;   try {

&nbsp;       const response = await fetch(url);

&nbsp;       const results = await response.json();

&nbsp;       

&nbsp;       if (!results.length || !results\[0].syncedLyrics) {

&nbsp;           console.log("Keine synchronisierten Lyrics gefunden.");

&nbsp;           sendToESP\_UDP("Keine Lyrics gefunden");

&nbsp;           return;

&nbsp;       }



&nbsp;       const lyricsArray = results\[0].syncedLyrics.split('\\n')

&nbsp;           .map(line => {

&nbsp;               const time = parseTime(line);

&nbsp;               let text = cleanUmlauts(line.replace(/\\\[.\*\\]/, '').trim());

&nbsp;               return { time, text };

&nbsp;           })

&nbsp;           .filter(item => item.time !== null \&\& item.text !== "");



&nbsp;       console.log(`${lyricsArray.length} Zeilen geladen. Modus: 1s Vorlauf.`);

&nbsp;       

&nbsp;       const startTime = Date.now();

&nbsp;       let currentIndex = 0;



&nbsp;       lyricInterval = setInterval(() => {

&nbsp;           const elapsedTime = (Date.now() - startTime) + 4200;

&nbsp;           

&nbsp;           if (currentIndex < lyricsArray.length) {

&nbsp;               if (elapsedTime >= lyricsArray\[currentIndex].time) {

&nbsp;                   const currentLyric = lyricsArray\[currentIndex].text;

&nbsp;                   

&nbsp;                   console.log(`\[Sync +1s] ${currentLyric}`);

&nbsp;                   sendToESP\_UDP(currentLyric);

&nbsp;                   

&nbsp;                   currentIndex++;

&nbsp;               }

&nbsp;           } else {

&nbsp;               clearInterval(lyricInterval);

&nbsp;               console.log("Songtext Ende.");

&nbsp;           }

&nbsp;       }, 10);



&nbsp;       currentPlayerProcess.on('close', () => clearInterval(lyricInterval));



&nbsp;   } catch (error) {

&nbsp;       console.error("Lyrics Fehler:", error.message);

&nbsp;   }

}





function startRecording() {

&nbsp;   console.log("Aufnahme läuft...");

&nbsp;   fileStream = fs.createWriteStream(filePath, { encoding: 'binary' });

&nbsp;   recording = recorder.record({

&nbsp;       sampleRate: 44100,

&nbsp;       channels: 1,

&nbsp;       threshold: 0,

&nbsp;       verbose: false,

&nbsp;       recordProgram: 'arecord',

&nbsp;       device: 'hw:2,0' // Headset Mikrofon

&nbsp;   });

&nbsp;   recording.stream().pipe(fileStream);

&nbsp;   isRecording = true;

}



function stopRecording() {

&nbsp;   console.log("Aufnahme beendet.");

&nbsp;   if (recording) recording.stop();

&nbsp;   if (fileStream) fileStream.close();

&nbsp;   isRecording = false;

}



async function recognize() {

&nbsp;   const fileBuffer = fs.readFileSync(filePath);

&nbsp;   const formData = new FormData();

&nbsp;   formData.append('file', fileBuffer, { filename: 'recording.wav', contentType: 'audio/wav' });



&nbsp;   const response = await fetch('https://shazam-api.com/api/recognize', {

&nbsp;       method: 'POST',

&nbsp;       headers: { 'Authorization': `Bearer ${SHAZAM\_KEY}`, ...formData.getHeaders() },

&nbsp;       body: formData

&nbsp;   });

&nbsp;   const data = await response.json();

&nbsp;   return data.uuid;

}



async function poll(uuid) {

&nbsp;   while (true) {

&nbsp;       const res = await fetch(`https://shazam-api.com/api/results/${uuid}`, {

&nbsp;           method: 'POST',

&nbsp;           headers: { 'Authorization': `Bearer ${SHAZAM\_KEY}` }

&nbsp;       });

&nbsp;       const data = await res.json();

&nbsp;       if (data.status === 'completed') return data.results;

&nbsp;       await delay(2000);

&nbsp;   }

}





button.on('alert', async (level) => {

&nbsp;   const now = Date.now();

&nbsp;   if (level !== 0 || now - lastPress < 500) return;

&nbsp;   lastPress = now;



&nbsp;   if (!isRecording) {

&nbsp;       startRecording();

&nbsp;   } else {

&nbsp;       stopRecording();

&nbsp;       try {

&nbsp;           console.log("Identifiziere...");

&nbsp;           sendToESP\_UDP("Erkenne Song...");

&nbsp;           

&nbsp;           const uuid = await recognize();

&nbsp;           const results = await poll(uuid);

&nbsp;           

&nbsp;           if (results \&\& results\[0]?.track) {

&nbsp;               const track = results\[0].track;

&nbsp;               const title = track.title;

&nbsp;               const artist = track.subtitle;



&nbsp;               console.log(`\\n GEFUNDEN: ${title} von ${artist}`);



&nbsp;               await playMusic(title, artist);

&nbsp;               

&nbsp;               await startLyricsStreaming(title, artist);

&nbsp;           } else {

&nbsp;               console.log("Nichts erkannt.");

&nbsp;               sendToESP\_UDP("Nichts erkannt");

&nbsp;           }

&nbsp;       } catch (err) {

&nbsp;           console.error("Fehler im Ablauf:", err.message);

&nbsp;           sendToESP\_UDP("Fehler");

&nbsp;       }

&nbsp;   }

});





console.log("========================================");

console.log("Raspberry Pi Lyric Player BEREIT");

console.log("Headset Mic: hw:2,0 | Audio Out: hw:1,0");

console.log("========================================");



process.on('SIGINT', () => {

&nbsp;   console.log("\\nBeende System...");

&nbsp;   if (isRecording) stopRecording();

&nbsp;   if (currentPlayerProcess) currentPlayerProcess.kill();

&nbsp;   if (lyricInterval) clearInterval(lyricInterval);

&nbsp;   button.unexport();

&nbsp;   process.exit();

});



------------------------------------------------------------------------------------------------------------------------------------------





!\\\[ButtonRasp](../Images/ButtonRaspPic.jpeg)

!\\\[Raspberry](../Images/RaspPic.jpeg)

!\\\[ConsoleOutput](../Images/ConsoleOutputPic.jpeg)

!\\\[SongLyricOutput](../Images/SongLyricOutputPic.jpeg)

!\\\[SongLyric](../Images/SongLyricVid.mp4)

!\\\[FirstSongFound](../Images/FirstSongFoundVid.mp4)





---



\[⬅ Back to Main Page](README.md)



