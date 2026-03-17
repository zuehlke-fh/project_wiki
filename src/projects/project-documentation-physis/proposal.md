# Physical Prototyping — Project Proposal

## Working Title
Saving Christmas! Ein kooperatives LED-Puzzle zur Reparatur des magischen Christbaums.

### Group Members

- Eibensteiner Hanna
- Gabriel Sabrina
- Haider Julian
- Stögmüller David

## Idea
Ein zweispieler-basiertes Weihnachts-Puzzle, bei dem Santa und Rudolph gemeinsam LED-Muster auf einem Display rekonstruieren müssen, um den magischen Christbaum zu reparieren.

## Detailed Description
In unserem Projekt steuern zwei Spieler gemeinsam ein interaktives Licht-Puzzle, das in eine kleine Weihnachtsgeschichte eingebettet ist: Der magische Christbaum, der Santa und Rudolph durch den dichten Nebel leiten sollte, ist kaputt. Um Weihnachten zu retten, müssen beide Spieler zusammenarbeiten und die Lichtmuster des Baums reparieren. Interaktion: Santa verwendet einen Joystick-Controller, um zwischen den LEDs zu navigieren. Die ausgewählte LED ist weiß. Rudolph steuert gleichzeitig die Farbwahl (Grün, Rot, Gelb, Blau, Braun) durch Klicken des Buttons. Auf dem Display erscheint eine Abfolge von LED-Mustern (insgesamt 4), welche nur für eine gewisse Zeit angezeigt werden und dann verschwinden. Nur wenn die LED Matrix mit dem vorherigen Bild auf dem Display übereinstimmen, schreitet das Puzzle voran. Bei Fehlern wird ein Fehlerton abgespielt, und der Schritt beginnt erneut. Wenn alle Muster erfolgreich rekonstruiert wurden, zeigt das Display den vollständig leuchtenden Christbaum. Musik spielt, und ein Motor öffnet eine Geschenkbox, in der die Spieler eine kleine Belohnung finden können.

Symbiotic Machines/Bezug zum Thema:
Die Maschine ist nicht alleine steuerbar. Santa und Rudolph bilden zusammen ein symbiotisches Kontrollsystem: Santa kontrolliert wo. Rudolph kontrolliert was. Erst im Zusammenspiel entsteht die richtige Eingabe. Die Maschine reagiert nur, wenn beide Systeme korrekt interagieren. Das Puzzle selbst funktioniert wie ein symbiotischer „Organismus“, der aus zwei teilweise unabhängigen, aber voneinander abhängigen Eingabestationen besteht.

## Technical Implementation
Unser Prototyp besteht aus drei zentralen Komponenten: der LED-Display- bzw. Pixelmatrix zur Darstellung der Muster und des finalen „Weihnachtsbaum“-Effekts, zwei Eingabegeräten für Santa und Rudolph sowie einem mechanischen Reward-Mechanismus. Der Santa-Controller repräsentiert einen Joystick, während der Rudolph-Controller die Farbauswahl über einen einzelnen Button ermöglicht. Ergänzt wird das System durch einen Servo-Motor, der am Ende bei erfolgreichem Abschluss aller Muster eine kleine Kiste öffnet. Die gesamte Logik läuft auf einem Hauptcontroller (ESP32), der sämtliche Eingaben verarbeitet, Display, LEDs, Sound und Motor steuert.

Besonders herausfordernd werden die zuverlässige Synchronisierung der beiden Eingabegeräte, eine robuste Farb- und Positionslogik, die Musiksteuerung, die Motoransteuerung passend zum Endscreen sowie eine stabile Stromversorgung, insbesondere bei vielen LEDs. Unterstützung könnten wir vor allem bei der Optimierung der LED-Ansteuerung, einer sauberen Schaltkreisplanung, der sicheren Stromverteilung für LEDs und Motor sowie bei der mechanischen Stabilität des Öffnungsmechanismus gebrauchen.

## References
- Simon Says
- Minecraft „Replica“-Minigame

## Materials
1x ESP32 Dev Board
25x RGB LEDs
1x OLED Display
1x Button
1x Joystick
1x Servo Motor
1x Buzzer (Piezo)
Sonstiges: Kabel, Widerstände, Panzertape, Pappe, Tixo, Kleber, Goodies, Deko

## Sketch

![Sketch](https://www.figma.com/proto/WObwCwWSCOlzXZDuqlUwFu/5.-Semester---Tiny---Physi?node-id=59-3&t=NjDeyN8Np5IxcMiY-1&scaling=contain&content-scaling=fixed&page-id=59%3A2)

### Time Plan

| Due Date | Description |

12.12.2025   Präsi
17.12.2025   Grober Aufbau und Verbindung von Hardware-Komponenten (ESP32, LEDs, Buttons, Joystick)
08.01.2026   Funktionierender Input + Display-Schnittstelle + Musterlogik
14.01.2026   Endscreen + Motor/Belohnungssystem
21.01.2026   Gehäusebau + Feinschliff + Playtesting
28.01.2026   Showdown

