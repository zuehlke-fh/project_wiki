[⬅ Back to Main Page](README.md)



# Timeline: Shazamers Prototype



## Description

Add a detailed explanation of this subsection here.



At the beginning of the project, the main idea and concept were defined. The goal was to build an interactive prototype that can recognize music using audio input. During this phase, different technical approaches were discussed and basic research on music recognition APIs was done.



In the next phase, the basic hardware setup was built. The Raspberry Pi was set up as the central unit, and the ESP32 with a TFT display was prepared for output. Initial tests with audio input revealed limitations when trying to sing directly into a microphone, which led to changes in the concept.



After that, the focus shifted to implementing audio recording and API communication. A button-controlled recording system was developed, and the Shazam API was integrated for song recognition. This phase required several iterations due to API response times and configuration issues.



Once music recognition was working, additional features were added. Audio playback via the headset was implemented, and synchronized lyrics were retrieved using the LRCLIB API. The lyrics were then sent via UDP to the ESP32 and displayed on the screen.



In the final phase, the prototype was tested, refined, and stabilized. Audio device settings were adjusted, timing issues were improved, and the overall system reliability was increased. The current prototype represents the latest iteration of an ongoing development process.





[⬅ Back to Main Page](README.md)



