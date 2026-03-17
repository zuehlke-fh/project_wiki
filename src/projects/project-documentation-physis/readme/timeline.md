[⬅ Back to Main Page](../README.md)

# Timeline: Projektverlauf & Meilensteine

## Description
Dieser Abschnitt vergleicht den ursprünglich geplanten Zeitplan mit dem tatsächlichen Projektverlauf.  
Er dokumentiert die zentralen Meilensteine und erläutert, wie sich der technische Pivot von der LED-Hardware zur Web-UI auf die Zeitplanung und Arbeitsverteilung ausgewirkt hat.

## Content

### Geplanter Zeitplan (Proposal)
Der ursprüngliche Plan sah eine lineare Entwicklung von der Hardware-Basis bis zum Feinschliff vor.

- **12.12.2025:** Präsentation der Idee  
- **17.12.2025:** Grober Aufbau der Hardware-Komponenten  
- **08.01.2026:** Implementierung von Input, Display-Schnittstelle und Musterlogik  
- **14.01.2026:** Fertigstellung von Endscreen und Motor-/Belohnungssystem  
- **21.01.2026:** Gehäusebau, Feinschliff und Playtesting  
- **28.01.2026:** Showdown (Finale Abgabe)

### Anpassungen im realen Projektverlauf
Im tatsächlichen Verlauf kam es zu Verschiebungen, die primär durch die Änderung des Anzeigemediums bedingt waren.

#### Der Pivot: Von LED-Matrix zu Web-Grid
Die Entscheidung, statt einer physischen LED-Matrix eine Web-Oberfläche zu nutzen, verschob den Arbeitsschwerpunkt deutlich von Hardware zu Software.

**Verschiebung der Aufwände:**

- **Mehr Zeitaufwand:**  
  Es wurde deutlich mehr Zeit für die WebSocket-Kommunikation, das UI-Design sowie die Game-State-Logik  
  (Zonen, Timer, Progressbar) und die Netzwerkstabilität der drei ESPs benötigt.

- **Weniger Zeitaufwand:**  
  Im Gegenzug entfielen zeitintensive Hardware-Aufgaben wie die komplexe LED-Verkabelung,  
  das Management der Stromversorgung für viele LEDs und die mechanische Integration einer großen Matrix.

---

[⬅ Back to Main Page](README.md)
