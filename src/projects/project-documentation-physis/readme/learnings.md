[⬅ Back to Main Page](../README.md)

# Learnings: Erkenntnisse & Reflexion

## Description
Dieses Projekt war ein iterativer Prozess, bei dem technische Hürden oft zu Design-Entscheidungen führten, die das Endergebnis verbesserten.  
Dieser Abschnitt fasst die wichtigsten „Aha-Momente“ in Bezug auf Hardware-Architektur, Software-Aufwand und Game-Design zusammen.

## Content

### 1. Hardware-Verfügbarkeit bestimmt das Design
Die ursprüngliche Idee einer physischen LED-Matrix war konzeptionell stark, scheiterte jedoch an der Verfügbarkeit passender Panels und der hohen Komplexität der Verkabelung.

- **Erkenntnis:**  
  Nicht jede Hardware-Idee ist im Prototyping-Scope sinnvoll umsetzbar.

- **Lösung:**  
  Der Pivot zur Web-UI war notwendig, eröffnete aber neue Möglichkeiten (bessere Visuals, weniger Stromprobleme) und rettete die Spielmechanik.

### 2. „Separation of Concerns“ durch mehrere Controller
Die Aufteilung des Systems auf drei ESP32-Boards (Master, Button, Jingle) reduzierte die Komplexität drastisch.

- **Vorteil:**  
  Klare Zuständigkeiten bedeuteten weniger Fehlerquellen und einfacheres Debugging, da jeder Controller nur seine spezifische Aufgabe erfüllte (Input vs. Logik vs. Output).

- **Stabilität:**  
  Solange die Kommunikation (HTTP-Requests) simpel gehalten wird, läuft das System stabiler als ein überladener Einzel-Controller.

### 3. Software-Aufwand bei „Virtuellen Displays“
Die Entscheidung für eine Web-UI (via WebSocket) bot zwar gestalterische Freiheit, verschob den Arbeitsaufwand jedoch massiv von Hardware zu Software.

- **Aufwand:**  
  UI-Elemente wie Progressbars, Timer, Dimming-Effekte und Win-Overlays mussten programmiert werden, was im Prototyping schnell zu einem eigenen großen Arbeitsblock wurde.

### 4. Die Wichtigkeit von „Non-blocking“ Code
Für das Zusammenspiel von Audio (Jingle) und Mechanik (Motor) war eine blockierende Programmierung (z. B. durch lange `delay()`) tödlich für den Ablauf.

- **Lösung:**  
  Das System funktioniert nur zuverlässig, weil Musik Note-für-Note und der Stepper Schritt-für-Schritt über manuelles Timing gesteuert werden, ohne den Loop anzuhalten.

### 5. Physische Trennung stärkt die Kooperation
Die „Symbiose“ der zwei Spieler funktioniert am besten, wenn die Rollen auch physisch stark getrennt sind.

- **Game Design:**  
  Dass Rudolf ein komplett eigenes Gerät (Button-ESP) in der Hand hält, macht den Team-Aspekt erst wirklich spürbar.  
  Es fühlt sich nicht an wie „zwei Inputs an einem Gerät“, sondern wie zwei unabhängige Stationen, die zusammenarbeiten müssen.

---

[⬅ Back to Main Page](README.md)
