[⬅ Back to Main Page](README.md)



# Result: Shazamers Prototype



## Description

Add a detailed explanation of this subsection here.



The final prototype is able to recognize music using audio input and display the corresponding song information and lyrics. A physical button connected to the Raspberry Pi is used to control the recording process. Pressing the button starts the audio recording, and pressing it again stops the recording.



The recorded audio is processed on the Raspberry Pi and sent to the Shazam API for song recognition. If a song is successfully identified, the prototype plays the recognized song through a connected headset. At the same time, synchronized lyrics are retrieved using the LRCLIB API.



The lyrics are sent wirelessly via UDP to an ESP32 microcontroller, which displays the text on a TFT screen. The text size and layout are adjusted dynamically depending on the length of the lyric line.



Overall, the result represents a functional interactive prototype that demonstrates the combination of hardware interaction, audio processing, external APIs, and iterative development. While the prototype still has limitations, it successfully shows the intended core functionality.



## Content

![Result thumbnail](/projects/project-documentation-shazamers/images/ShazamPic.jpg)

[Result video](/projects/project-documentation-shazamers/images/ErgebnisVid.mp4)

[⬅ Back to Main Page](README.md)



