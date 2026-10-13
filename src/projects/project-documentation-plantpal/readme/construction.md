[⬅ Back to Main Page](README.md)

# Construction: [PlantPal]

## Description
What did we need for the construction of the prototype?
- Pump
- Tubes
- Valves
- Water reservoir
- Various casings
- ESP32
- LoRaWan gateway
- LoRaWan antenna
- Electronic wires
- Moisture sensors
- Insulating tube
- Flower pots
- Power supply unit
- Pallet

How did we work?
The first thing we did was to draw up a circuit diagram. Then we decided on the structure. 
![Image example](/projects/project-documentation-plantpal/images/PlantPalScheme.jpg)

We put soil in the plant pot and wired the humidity sensor with electricity. We filled the water reservoir and connected the hoses to it. We connected the pump to the water circuit and supplied it with power. 
Then it was time for the control logic. In VsCode, the humidity value was read out on a specific pin in the software. The pump control was also implemented. The pump is switched on for approx. 5 seconds when the plant has a residual humidity of 50%. The humidity value is read out twice a day. 

## Content
![Image example](/projects/project-documentation-plantpal/images/ablauf.png)
![Image example](/projects/project-documentation-plantpal/images/Aufbau.png)


---

[⬅ Back to Main Page](README.md)
