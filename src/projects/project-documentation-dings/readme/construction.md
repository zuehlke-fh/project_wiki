[⬅ Back to Main Page](README.md)

# Construction: [Der Freundeszug]

## Description
Die Konstruktion der beiden Roboter erfolgte iterativ und vereinte elektronische Experimente mit pragmatischem Prototypenbau. Ziel war es, zwei eigenständige, kommunizierende Einheiten zu entwickeln, die aufeinander reagieren und gemeinsam ein symbiotisches Verhalten zeigen. Dabei wurden Hardware, Programmierung und Gehäusedesign parallel weiterentwickelt und laufend angepasst.


## Content
Zu Beginn wurde die drahtlose Verbindung zwischen den beiden ESP32-Mikrocontrollern über ESP-Now eingerichtet, um eine stabile Kommunikation ohne externes Netzwerk zu gewährleisten. Darauf folgte der elektronische Aufbau der Prototypen. Ursprünglich war ein Solarmodul für die „Blume“ vorgesehen, dieses wurde jedoch durch einen Lichtsensor ersetzt. Dadurch konnten beiden Robotern unterschiedliche Licht-bezogene Verhaltensweisen zugewiesen werden:

Die Blume nutzte den Sensor, um zu erkennen, wann sie ausreichend Licht erhielt, und sendete diese Information an den Pusher.

Der Pusher verwendete denselben Sensortyp, um selbstständig Lichtquellen zu erkennen und sich in deren Richtung zu bewegen.

Ein Infrarot-Sender-Empfänger-System wurde testweise integriert, aber wegen technischer Schwierigkeiten wieder entfernt. Eine besondere Herausforderung stellte die Stromversorgung dar: Da der ESP32 nur 3,3 V liefert, mussten externe Module die Sensoren und Motoren versorgen. Ein Versuch, über 5 V direkt von einer Powerbank zu arbeiten, scheiterte, da der geringe Stromverbrauch der Motoren die Powerbank nicht aktivierte. Letztlich wurde die Spannungsversorgung über den Input-Pin des ESP32 realisiert – eine kreative, aber erfolgreiche Lösung.

Als audiovisuelles Feedback wurde ein Buzzer eingebaut, der eine kurze Melodie abspielt, wenn die Blume genügend Licht empfängt – ein symbolisches Zeichen für ihre „Zufriedenheit“ im symbiotischen Zusammenspiel.

![Blume](/projects/project-documentation-dings/images/Pic2.jpeg)
![Blume](/projects/project-documentation-dings/images/Pic3.jpeg)

Für das Gehäuse wurde ein Mix aus Lego Technic-Bauteilen, Kunststoffboxen (eines bekannten Schokoladenprodukts) und der Modelliermasse Green Stuff verwendet. Im Wagen der Blume wurden zwei Reifen montiert, um Bewegung zu ermöglichen. Ein Lego-Rahmen hält die Blume leicht erhöht im Container und schafft Stauraum für eine Powerbank. Über ein Verbindungselement wurde der mechanische „Freundeszug“ zum Pusher hergestellt. Ein Arm mit Lochplatte sorgt zusätzlich für Stabilität und Abstand zum Boden.

![Blume](/projects/project-documentation-dings/images/20260127_222420.jpg)
![Blume](/projects/project-documentation-dings/images/20260127_222750.jpg)
![Blume](/projects/project-documentation-dings/images/20260127_222430.jpg)
Der Pusher wurde ebenfalls in eine Kunststoffschale integriert, die Platz für eine weitere Powerbank bietet. Zwei Motoren wurden mithilfe von Klebeband und Zweikomponentenmasse fixiert, um eine zuverlässige Fortbewegung zu gewährleisten. Durch die Kombination aus improvisierten Materialien und angepasster Elektronik entstand ein funktionaler wie charaktervoller Prototyp, der die Idee einer freundschaftlichen Roboter-Symbiose greifbar macht.
![Blume](/projects/project-documentation-dings/images/20260127_222415.jpg)
![Blume](/projects/project-documentation-dings/images/20260127_222418.jpg)
![Blume](/projects/project-documentation-dings/images/20260127_222418.jpg)

![Blume](/projects/project-documentation-dings/images/20260127_222758.jpg)

---

[⬅ Back to Main Page](README.md)
