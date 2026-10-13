[⬅ Back to Main Page](README.md)

# Construction

## Description
"Tom Peter", a MIDI trumpet controller, was built to capture the feel of a trumpet while translating it into MIDI signals for digital music production. The process involved designing the structure, integrating various sensors, and carefully wiring everything to ensure smooth operation and comfortable playability.

## Structural Design
The frame was modeled after a traditional trumpet. The buttons and sensors were positioned to mimic the natural hand placement and playing technique of a standard trumpet. Because of the size of the circuit boards, the shell turned out bigger than expected.  

## Key Components
- Teensy 4.0 Microcontroller: Chosen for its fast processing and built-in USB MIDI functionality, essential for responsive real-time performance.
- Valve Buttons: Tactile buttons were used to replicate the function of trumpet valves, mapped to different MIDI note changes.
- Breath Sensor: A pressure sensor captured the player’s airflow, converting it into MIDI data for dynamic volume control, making the instrument more expressive.
- Pitch Modulation: A rotary potentiometer allowed smooth pitch bends and slides, giving players more control over the sound.
## Wiring and Electronics
All components were wired to the Teensy microcontroller. Care was taken to ensure clean and secure connections to avoid interference. A custom layout was designed to keep wiring organized and stable during play.

## Software Integration
We were given a Teensy 4.0 to work with for our Midi-Controller. We use it because it is smaller and more optimized for the development of a Midi-Controller than an Arduino Uno, which we have used in our lessons before.
Additionally we use the Github library "Control-Surface", a library optimized for Midi-Controller development, for easier communication with our Teensy 4.0.
The Teensy was programmed to process input from the buttons, breath sensor, and pitch modulation sensor. These inputs were mapped to MIDI messages like:

- Notes: Triggered by the valve buttons.
- Velocity: Controlled by breath pressure to reflect how hard the player "blows."
- Pitch Bends: Adjusted using the rotary control.
- MIDI Expression Control: Smooth dynamic adjustments using breath input.

## Content
![Image example](/projects/project-documentation-tom-peter/images/prototyp.jpg)
![Image example](/projects/project-documentation-tom-peter/images/tompeter_skizzeae.jpg)
![Image example](/projects/project-documentation-tom-peter/images/trompeter2.png)
![Image example](/projects/project-documentation-tom-peter/images/preassure_sensor1.jpeg)
![Image example](/projects/project-documentation-tom-peter/images/innereien3.jpg)
![Image example](/projects/project-documentation-tom-peter/images/diy_huelle.jpg)


---

[⬅ Back to Main Page](README.md)
