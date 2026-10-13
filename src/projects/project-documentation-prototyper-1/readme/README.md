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

# LockQuest

## Basic Topic Description
Picking a lock using the gyro sensor and grip tools,
combining dexterity and strength.


## Idea and Inspiration

The idea for this project was to combine dexterity and strength in an interactive game. The innovative game “LockQuest” was created by integrating sensor technology and mechanics.

The first idea was with electrodes to build an EMG to measure the muscle voltage, but it was too expensive to afford the electrodes, so it was discarded.
Instead of the electrodes, hand trainers are used in the final prototype.

Virtual space with a fixed point to “loosen” the lock:

The game takes place in an invisible virtual space defined by a coordinate system. Within this space, there is a specific point at which the lock becomes “looser”, which means that the player has the opportunity to pick the lock there. This point is not visible to the player and must be found by skillful movement.

Gyro sensor to detect player movement:

A gyro sensor is used to detect the player's movements. By tilting the chest, the player can influence his position in virtual space. The data from the gyro sensor is continuously sent to the microcontroller (ESP32), which calculates the player's current position in the virtual space.

Acoustic signal to indicate proximity to the target point:

A sound module gives the player feedback about his proximity to the specified point in the virtual space. The closer the player gets to this point, the faster the acoustic signal sounds. This real-time feedback helps the player to locate the hidden point where the lock gets loose.

Grip tools to “pick” the lock:

As soon as the player is close enough to the target point, the sound stops and the player has to use the hand trainers to "open" the chest.
The hand trainers have to be pressed together alternately, wich increases a counter.
When the counter reaches a certain value, the chest opens.

Servo to control the opening of the treasure chest:

A servo motor is mechanically connected to the treasure chest and controls its locking mechanism. When the player has successfully picked the lock, the microcontroller sends a signal to the servo, which then opens the chest and gives the player access to the “treasure”.

## Resources
Hardware components:

Gyro sensor: Detects the player's movements.
Handle tools: When pressed together alternately it counts up to “pick” the lock.
Sound module: Emits acoustic signals to indicate proximity to the target point.
Microcontroller (ESP32): Controls the sensors and other components.
Servo: Controls the opening and closing of the treasure chest.
Powerbank: Supplies the system with energy.
Standard accessories: breadboard, cables, resistors.
Chest materials: wood, hinges, chains, screws, glue, screw hook.

## Subsections
This wiki is divided into the following sections:
- [Construction](readme/construction.md): Detailed construction process.
- [Timeline](readme/timeline.md): Key milestones and deadlines.
- [Learnings](readme/learnings.md): Insights gained from the project.
- [Result](readme/result.md): Final outcomes and evaluation.
