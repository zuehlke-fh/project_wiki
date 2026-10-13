[⬅ Back to Main Page](README.md)

# Construction


### Box
Since Klangkurbel is not only based on a hurdy-gurdy but rather a combination of a hurdy-gurdy and a classical music box, a wooden box not too big but still being able to house all the parts was needed. Luckily a flee market provided the perfect box for this project. The only things that needed to be done for the box were to drill a hole on one side to fit the crank and remove a little bit of wood on the inside of the lid to make space for the speaker. This was done by carefully drilling into the lid with a big enough drill without going all the way through it.

### Crank
The crank and its generator were take from an old hand crank flashlight.
Since the frame of the box is much thicker than the original frame of the flashlight, a shaft needed to be built to connect the crank back to its generator. This was slightly challenging in the beginning, but after three iterations, a sturdy shaft made out of wood, which connects the crank to the generator's gear through a few screws was made.
The generator was screwed to a thing piece of wood, which then was glued onto the bottom of the box to hold the generator in place.
The crank is perfect for the box since it is not too big and only a few volts are needed for controlling the playback and charging the battery.

### Speaker
The speaker was glued onto the inside of the lid. To make it look nicer and also hold the speaker better in place, a sponge was cut to the right dimensions acting like stuffing around the speaker.

### Electronics
An electronical board was soldered, which inherits a three-phase rectifier for the generator, a voltage devider to get an output under 3.3 Volts for the analog pin of the ESP and a voltage regulator which regulates to 5 Volts for charging the battery.

### Connecting the parts
An analog input of the ESP is connected to the positive output of the voltage devider to read the generated voltage and control the playback with it. For charging, the USB pin is connected to the positive output of the voltage regulator to charge the battery while turning the crank. The EN pin is connected to a switch which grounds the pin on closing the box, which then disconnects the battery from the ESP. Lastly, the speaker is connected to the Wing.

![Image of closeup](/projects/project-documentation-klangkurbler/images/closeUp.JPEG)

### Code
For the [logic](https://github.com/zuehlke-fh/project_wiki/blob/main/src/projects/project-documentation-klangkurbler/sketch.ino) on the ESP, an already existing example patch from the  [VS1053 Library](https://github.com/adafruit/Adafruit_VS1053_Library) was altered to include the logic for controlling playback with the crank and the web interface for uploading songs.


---

[⬅ Back to Main Page](README.md)
