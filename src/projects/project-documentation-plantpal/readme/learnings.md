[⬅ Back to Main Page](README.md)

# Learnings: [PlantPal]

## Description
In this section, we look at the most important findings from the development of the ‘PlantPal’ prototype. In particular, we focus on the practical challenges and solutions that we found during the project.

## Content
Moisture sensor and threshold value:
For the moisture sensor, it was important to find the right threshold value for irrigation. We learnt that small fluctuations in the measured values can influence the control system.

Power supply and relay control:
Switching between the hand crank generator and the power supply unit via a relay was a practical solution in order to always have a power source available. We discovered that it is important to synchronise the switching properly to avoid interruptions.

ESP32 and Mosfet:
The ESP32 can read the sensors and control the pump and the valve. Using a MOSFET to control the pump worked well to provide the necessary voltage.

---

[⬅ Back to Main Page](README.md)
