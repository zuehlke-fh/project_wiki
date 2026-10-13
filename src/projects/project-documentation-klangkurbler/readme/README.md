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
  - [Sketch](https://github.com/zuehlke-fh/project_wiki/blob/main/src/projects/project-documentation-klangkurbler/sketch.ino)

---

## Basic Topic Description
Klangkurbel is a digital hurdy-gurdy which allows playback control of mp3 files by turning its crank through an ESP32 and a Adafruit Music Maker FeatherWing. The ESP also provides a web interface, where songs can be added or removed from the queue.

All the electronical and mechanical parts are neatly stored in a wooden box, which starts up the system on opening. After having uploaded a song onto the sd card through the web interface, 5 seconds of turning the crank are needed to "power up" the music box. Music starts playing by turning then crank. If turned too slowly, the playback will also be slower than the original. If turning stops, the song will be paused and be resumed as soon as turning continues. To save some energy the crank also charges the battery, which gets disconnected by a small switch on the side of the box when it is closed and not used. Song selection is not possible - it has to be cranked through to get to the song of choice ;-)

## Idea and Inspiration
The topic of the project was "Human powered treasures". This immedietely gave the idea of a device using a crank for energy generation, like hand crank flashlights.
Since one of the team members is very keen to music, the idea of a hurdy-gurdy popped up quite quickly, since it already fits the theme of human power through cranking.

To make the hurdy-gurdy digital and justifying the incorporation of a microcontroller, we crafted the idea of not only playing digital music through the ESP, but also controlling the playback like pause/play and the playback speed through the crank, by measuring the generated voltage and mapping those values for these operations.

During construction the idea slightly changed, thus some features, like exceeding original playback speed through faster cranking were cut from the final prototype.
The original idea also involved playing a song from a USB-stick and having multiple USBs acting like music tapes which can be exchanged to play other tracks. Sadly, time and money didn't make this possible, so instead it was opted for playing music from an sd-card through the already present sd slot on the wing.

![Image example](/projects/project-documentation-klangkurbler/images/open.JPEG)

## Resources
- [Adafruit HUZZAH32 - ESP32 Feather](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather/overview)
- [Adafruit Music Maker FeatherWing](https://learn.adafruit.com/adafruit-music-maker-featherwing)
- [VS1053 Library](https://github.com/adafruit/Adafruit_VS1053_Library)
---

## Subsections
This wiki is divided into the following sections:
- [Construction](readme/construction.md): Detailed construction process.
- [Timeline](readme/timeline.md): Key milestones and deadlines.
- [Learnings](readme/learnings.md): Insights gained from the project.
- [Result](readme/result.md): Final outcomes and evaluation.
- [Sketch](https://github.com/zuehlke-fh/project_wiki/blob/main/src/projects/project-documentation-klangkurbler/sketch.ino): Final Esp32 Sketch.
