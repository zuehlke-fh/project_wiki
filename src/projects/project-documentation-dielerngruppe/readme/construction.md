[⬅ Back to Main Page](README.md)

# Construction: [Konstruktion der Kanone]

## Mechanischer Part:
Die Kanonenbauteile wurden einzeln zusammengesucht und teilweise luftdicht gemacht, da unsere Kanone mit Druckluft arbeitet. Es war wichtig, dass kein Druck entweicht, sobald der Riegel und das Magnetschloss verschlossen sind, da ein Druckverlust die Funktion der Kanone beeinträchtigen würde. Folgende Teile wurden mit Tesa-Band abgedichtet, um Druckluftverluste zu vermeiden: das Magnetventil, der Druckluftbehälter und der Adapter für die Fahrradpumpe. Nachdem alle Teile für die Luftdruckpumpe zusammengesetzt wurden, war der mechanische Aufbau abgeschlossen. Nun mussten wir einen Plan entwickeln, wie der elektronische Teil umgesetzt werden kann. Das Magnetventil benötigt etwa 4,5 Volt, um angesteuert zu werden.

![Mechanischer Part der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_13.jpg)


## Elektronischer Part:
Im nächsten Schritt widmeten wir uns dem elektronischen Aufbau, der eine besondere Herausforderung darstellte, da wir Gleichstrom für das Magnetventil umwandeln mussten. Das Magnetventil wird mit positiver Spannung geöffnet und mit negativer Spannung geschlossen. Unser Strom kommt von einem Microcontroller (ESP-32 Dev Kit C V4), der Gleichspannung liefert. Da das Magnetventil jedoch zum Schließen negative Spannung benötigt, verwendeten wir ein 2-Kanal-Relaismodul (2-Channel Relay Module 5V), um die Polarität umzukehren. Zusätzlich war ein weiteres Relais erforderlich (1-Channel Relay 5V KY-019), um das Ventil nach dem Schließen wieder zurückzuschalten.
Die Spannungsversorgung des Microcontrollers war ausreichend, um das Magnetventil erfolgreich zu öffnen und zu schließen. Anschließend haben wir ein Mikrofon als Steuereinheit angeschlossen, das die Kanone auslöst, sobald ein Signal erkannt wird. Ursprünglich war das Mikrofon so programmiert, dass die Kanone durch Klatschen ausgelöst wird; später ersetzten wir das Klatschen jedoch durch Hineinblasen.

![Elektronischer Part](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_11.jpg)


## Programmier Part:
Wir entwickelten ein simples Skript mit folgenden Funktionen:
+ Das Ventil bleibt geschlossen, solange kein Mikrofon-Signal empfangen wird.
+ Sobald ein bestimmter Schwellenwert überschritten wird, öffnet das Skript das Ventil.
+ Nach einer Wartezeit von 4 Sekunden (Sicherheitsmaßnahme) wird das Ventil für 1 Sekunde geöffnet und dann wieder geschlossen.


Während die Kanone aktiv ist, werden keine weiteren Mikrofon-Signale verarbeitet. Erst nach dem Abschluss des Abfeuerns wird das Mikrofon wieder aktiviert, um ein neues Signal aufzunehmen. Diese Sicherheitsvorkehrung soll verhindern, dass die Kanone unkontrolliert ausgelöst wird, z. B. durch unerwartete Geräusche. 
Unsere Kanone ist nun funktional und einsatzbereit. Der nächste große Schritt besteht darin, die Kanone optisch ansprechender zu gestalten, um ihr ein weniger "rohes" Aussehen zu verleihen. Dafür planen wir die Modellierung und das Design der äußeren Hülle.


![Programmier Part](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_12.jpg)

## Modellierung der Kanone:
Nachdem wir die benötigten Bauteile definiert hatten, konnten wir mit der Modellierung beginnen. Zunächst recherchierten wir online, wie Kanonen typischerweise aussehen, um Inspiration für die Gestaltung zu sammeln. Unser Ziel war es, sowohl die mechanischen als auch die elektronischen Komponenten optimal unterzubringen. Der Druckluftbehälter, als größtes Bauteil, diente dabei als Ausgangspunkt. Geplant war, den Druckluftbehälter in ein Kanonenrohr einzubauen, das von einem stabilen Kanonenständer getragen wird.
Die Modellierung erfolgte in Blender, wobei wir ähnliche Modelle aus dem Internet als Referenz heranzogen und teilweise anpassten, damit der Druckluftbehälter und die übrigen Komponenten in die Kanone integriert werden konnten. Unsere Idee war, die Kanone in Einzelteilen zu drucken und anschließend zusammenzubauen. Die Konstruktion wurde in drei Hauptteile unterteilt:

+ Kanonenrohr
+ Halterung für das Kanonenrohr
+ Gehäuse für die elektronischen Komponenten

In diesem Schritt wurden die elektronischen Komponenten genau ausgemessen, um sicherzustellen, dass sie korrekt in die Kanone eingebaut werden können. Nach Abschluss der Modellierung begannen wir mit dem 3D-Druck der Teile.

## Der Zusammenbau der Kanone:
Zunächst wurden alle Teile in verkleinertem Maßstab (65 % der Originalgröße) gedruckt, um die Passform und Funktionalität zu testen. Dadurch konnten wir frühzeitig feststellen, welche Teile optimiert werden mussten. Es gab nur kleinere Anpassungen, wie das Korrigieren von Mesh-Fehlern und das Verlängern bestimmter Teile, um mehr Platz für andere Komponenten zu schaffen.
Nach diesen Optimierungen wurden die Teile in Originalgröße gedruckt, was etwa zwei Tage in Anspruch nahm. Das Kanonenrohr wurde in zwei Teile geteilt, da es zu lang für den Drucker war, und anschließend mit Montagesilikon zusammengeklebt. Im Inneren der Kanone wurden das Druckluftventil und das Mikrofon verbaut. Die Drähte und elektronischen Komponenten wurden in einem Gehäuse unter der Kanone untergebracht und möglichst unauffällig verlegt. Um die Optik zu verbessern, haben wir der Kanone vier Räder hinzugefügt. Zusätzlich wurde eine Ampel integriert, die den Status der Kanone visuell anzeigt - dazu jedoch gleich mehr.

![Kleiner Prototyp der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_8.jpg)
*Kleiner Prototyp der Kanone*

![Zusammenbau der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_3.jpg)
*Zusammenbau der Kanone 1*

![Zusammenbau der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_6.jpg)
*Zusammenbau der Kanone 2*

![Zusammenbau der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_1.jpg)
*Zusammenbau der Kanone 3*

![Zusammenbau der Kanone](/projects/project-documentation-dielerngruppe/images/Kanonen_Bild_2.jpg)
*Fertig zusammengebaute Kanone*

# Optimierungen:
## Mikrofonsignal:
Zu Beginn sollte die Kanone durch Klatschen ausgelöst werden. Allerdings stellte sich heraus, dass das Signal entweder nicht erkannt oder falsch interpretiert wurde. Daher entschieden wir uns dafür, das Mikrofon durch Hineinblasen zu aktivieren. Dies ist nicht nur zuverlässiger, sondern fügt der Kanone auch einen humorvollen Aspekt hinzu, da das Mikrofon an der Stelle angebracht ist, an der sich bei einer echten Kanone der Zündmechanismus befindet.

## Visuelles Feedback:
Während der Tests wurde klar, dass visuelles Feedback notwendig ist, um den Status der Kanone anzuzeigen. Die Ampel bietet eine klare Rückmeldung und erhöht die Sicherheit:

+ Grün: Die Kanone ist bereit, und man kann ins Mikrofon blasen.
+ Gelb: Die Kanone schaltet in den Vorbereitungsmodus (4-Sekunden-Wartezeit).
+ Rot: Die Kanone feuert.

Dieses visuelle Feedback erhöht nicht nur die Sicherheit, sondern sorgt auch für Spannung, da die Kanone nicht sofort schießt.

## Sicherheitsmaßnahme – Trichterform:
Um die Gefahr durch zu starke Schüsse zu minimieren, haben wir das Kanonenrohr mit einer Trichterform versehen. Dadurch kann überschüssige Luft seitlich entweichen, wodurch die Reichweite und Kraft des Schusses reduziert werden. Diese Sicherheitsmaßnahme schützt sowohl die Benutzer als auch die Umgebung.


# Funktionsweise der Kanone:
Die Kanone funktioniert folgendermaßen:

1. Die Munition wird in das Kanonenrohr eingelegt.
2. Mit einer Fahrradpumpe wird Druck aufgebaut, der im Druckluftbehälter gespeichert wird.
3. Durch Hineinblasen in das Mikrofon wechselt die Kanone in den Vorbereitungsmodus (gelbe Ampel).
4. Nach 4 Sekunden Wartezeit feuert die Kanone (rote Ampel).
5. Nach dem Schuss kehrt die Kanone in den Bereit-Modus (grüne Ampel) zurück, und der Vorgang kann wiederholt werden.



---

[⬅ Back to Main Page](README.md)
