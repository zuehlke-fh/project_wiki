# Project Wiki

Welcome to the project wiki! This is the central hub for all information related to this project.

## Table of Contents
- [Basic Topic Description](#basic-topic-description)
- [Idea and Inspiration](#idea-and-inspiration)
- [Resources](#resources)
- [Subsections](#subsections)
  - [Construction](readme/construction.md)
  - [Timeline](readme/timeline.md)
  - [Learnings](readme/learnings.md)
  - [Result](readme/result.md)

---

## Basic Topic Description

Saving Christmas! ist ein kooperatives Physical-Prototyping-Projekt im Stil eines kleinen „Symbiotic Machine“-Games.  
Zwei Spieler (Santa und Rudolf) arbeiten gemeinsam daran, ein interaktives Puzzle zu lösen, um den magischen Christbaum zu reparieren und damit Weihnachten zu retten.

Die Steuerung des Systems ist dabei bewusst auf zwei Rollen aufgeteilt:

- **Santa** übernimmt die Navigation innerhalb des Spielfeldes und bewegt den Cursor mithilfe eines Joysticks über das Raster.
- **Rudolf** ist für die inhaltliche Aktion zuständig und setzt oder verändert durch das Betätigen eines Buttons die Farbe der ausgewählten Elemente.

Ziel des Projekts war es, kooperatives Handeln als zentrale Interaktions- und Spielmechanik erfahrbar zu machen.  
Erst durch das Zusammenspiel beider Rollen können korrekte Eingaben entstehen.

Der erfolgreiche Abschluss des Puzzles wird durch einen realen mechanischen Vorgang ergänzt:  
Ein Motor öffnet eine Geschenkbox und stellt den Spielern eine greifbare Belohnung zur Verfügung.

---

## Idea and Inspiration

### Initiale Idee & Story

Ursprünglich sollte das Puzzle auf einer physischen LED-Matrix stattfinden, die Schritt für Schritt ein Weihnachtsbaum-Pixelbild erzeugt.  
Die Story bettet das Spiel in ein weihnachtliches Szenario ein: Ein dichter Nebel verhindert die Auslieferung der Geschenke.  
Rudolfs Licht reicht nicht aus – erst muss der kaputte Christbaum repariert werden, um wieder als Orientierung zu dienen.

### Inspiration / Referenzen

Die spielerische Inspiration orientierte sich an bekannten Mechaniken aus bestehenden Spielen:

- **Simon Says:** Muster merken und reproduzieren
- **Minecraft „Replica“-Minigame:** Rekonstruktion eines vorgegebenen Bildes

Das System ist absichtlich nicht alleine sinnvoll steuerbar.  
Erst in der Kombination aus Position (Santa) und Aktion (Rudolf) entsteht die korrekte Eingabe – Symbiose als Game-Mechanik.

![Sketch](/projects/project-documentation-physis/images/sketch.png)

---

## Resources

### Team

Das Projekt wurde in Teamarbeit umgesetzt von:

- Eibensteiner Hanna  
- Gabriel Sabrina  
- Haider Julian  
- Stögmüller David  

### Hardware (Final Setup)

Für die finale Umsetzung kam eine 3-ESP-Architektur zum Einsatz:

- **Master ESP:** Webserver, WebSocket-Input-Bridge, Joystick-Auswertung (Analog VRX/VRY)
- **Button ESP:** Rudolf-Input (Input Pullup), sendet `/press` an Master
- **Jingle ESP:** Buzzer (Audio), Stepper Motor (28BYJ-48 + ULN2003) für den Belohnungsmechanismus

### Software / Technologien

- **Kommunikation:** WiFi (ESP32), Async WebServer + WebSocket (Master), HTTP Requests (Inter-ESP Kommunikation)
- **Frontend:** Web-UI mit HTML/CSS/JS (10×10 Raster, Phasenlogik, Progressbar, Win-Overlay)

---

## Subsections

This wiki is divided into the following sections:

- [Construction](readme/construction.md): Detailed construction process and iterations.
- [Timeline](readme/timeline.md): Key milestones and deadlines.
- [Learnings](readme/learnings.md): Insights gained from the project.
- [Result](readme/result.md): Final outcomes and evaluation.