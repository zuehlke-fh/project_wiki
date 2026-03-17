[⬅ Back to Main Page](../README.md)

# Construction: Iterationen & Architektur

## Description
Der Konstruktionsprozess des Projekts verlief iterativ und war von mehreren konzeptionellen sowie technischen Anpassungen geprägt.  
Ausgangspunkt war eine klar definierte Spielidee, deren Umsetzung jedoch mehrfach an reale Rahmenbedingungen angepasst werden musste, um vom ursprünglichen Hardware-Konzept zu einer stabilen hybriden Lösung zu gelangen.

## Content

### Iteration 1 — LED-Panel Puzzle (Originalplan)
In der ersten Konzeptionsphase war geplant, das Puzzle auf einer großen physischen LED-Matrix umzusetzen, die als greifbares Puzzle-Board fungiert.

- **Problem:**  
  Es war kein LED-Panel in der benötigten Größe verfügbar und der Verkabelungsaufwand sowie die Stromversorgung wären für den Scope zu komplex geworden.

- **Entscheidung:**  
  Pivot auf ein webbasiertes LED-Grid, um die Kernidee beizubehalten, ohne an Hardware-Limitierungen zu scheitern.

### Iteration 2 — Webserver statt LED-Matrix
Das Puzzle wurde auf eine Weboberfläche verlagert, die vom Master ESP gehostet wird.

- **Visualisierung:**  
  Ein 10×10 Raster stellt virtuelle LEDs dar, was mehr gestalterische Kontrolle über das Pixelbild ermöglichte.

- **Logik:**  
  Das Feld wurde in vier Sektoren unterteilt.  
  Jeder Sektor besitzt eine „Merken“-Phase (Muster wird gezeigt) und eine anschließende „Reparieren“-Phase (Spieler bauen nach).

### Iteration 3 — Steuerung über zwei Geräte (Santa & Rudolf)
Die Eingaben wurden bewusst auf zwei separate Geräte aufgeteilt, um den kooperativen Charakter zu stärken.

- **Santa (Navigation):**  
  Der Master ESP liest den Joystick (VRX/VRY) aus und sendet Bewegungsbefehle via WebSocket an die UI.

- **Rudolf (Aktion):**  
  Ein eigener Button-ESP erfasst Tastendrücke und sendet diese per HTTP-Request an den Master.  
  Dieser broadcastet das Signal als „FIX“-Befehl an die UI, um Farben zu setzen.

- **Ergebnis:**  
  Diese Trennung sorgte für ein deutlich wahrnehmbares Zwei-Spieler-Gefühl und stabilere Eingaben.

### Iteration 4 — Reward-System & 3-ESP-Architektur
Um Timing-Probleme und Blockierungen bei der gleichzeitigen Steuerung von Audio, Motor und Webserver zu vermeiden, wurde die finale Architektur auf drei Boards verteilt.

1. **Master ESP:**  
   Übernimmt ausschließlich Spiel-Logik, Webserver und Input-Verarbeitung.

2. **Button ESP:**  
   Dient rein als Eingabegerät für Rudolf.

3. **Jingle ESP:**  
   Ist exklusiv für den Belohnungsmechanismus (Audio + Motor) zuständig.

**Kommunikationsfluss:**

- `Button ESP` sendet bei Druck `/press` an `Master ESP`
- `Master ESP` ruft bei Sieg `/win` auf `Jingle ESP` auf
- Der `Jingle ESP` führt daraufhin autonom den Jingle und die Motorbewegung aus, ohne Daten zurückzusenden  
  (One-Way-Kommunikation für mehr Robustheit)

---

[⬅ Back to Main Page](README.md)
