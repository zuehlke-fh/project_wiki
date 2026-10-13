[⬅ Back to Main Page](README.md)

# Construction

## Description
We are three students with practically no electronics knowledge—aside from knowing that electricity exists. So, we needed a bit of help from a colleague. He gave us some valuable tips on solving the dynamo-to-power bank problem: converting AC to DC. Since the dynamo generates AC power, we had to convert it to DC for the power bank.

Step 1: Gathering the Components
First things first—we started by collecting all the necessary components. This part was relatively easy because we have some geeky friends who love microcontrollers and similar tech. Once we had everything, we tackled the first challenge: converting the dynamo's AC output to DC for the power bank.

Step 2: Converting AC to DC
One major issue we faced was that the dynamo's voltage fluctuates depending on how fast the user is cycling. With a bit of guidance from our colleague, we built a bridge rectifier circuit to convert the dynamo’s output to DC.

Since the voltage was still unstable, we added a capacitor to smooth it out and provide a more consistent power supply. Finally, we installed a voltage regulator to ensure a steady 5V output for charging.

Initially, we also added a cooling system because the board would heat up after prolonged cycling. However, due to space limitations (our enclosure was too small), we had to scrap that idea. So, our advice: don’t cycle for too long!

For debugging purposes, we added a small green LED that lights up when there’s sufficient voltage.

Step 3: Connecting the Power Bank
With everything in place, we can now simply plug in the power bank, and it will charge while cycling. Problem solved!

Step 4: Adding the Charging & Lighting System
So, the power bank charges as you cycle, but what about powering a light and a phone charger?

At first, we had no idea how to set that up. However, we figured it out with our ESP microcontroller, which we used to control a relay that switches between charging the phone and powering the bike light.

We ran into some issues with finding the correct pins, but after some trial and error (and a few prayers), we got it working. Once the relay functioned as intended, we installed an LED light, which we affectionately named Ledly.

The Ledly had one small problem—it came with its own ESP, which we were unable to access. So, we had to work around that by using the ESP on the "mainboard". Now, long-pressing the button makes the light blink, while a short press toggles between charging and lighting mode. Surprisingly, this part was easier to implement than expected!

Step 5: Final Assembly
After assembling everything and running a few tests, we mounted the system onto a mountain bike.
And here it is—our LoadingLightBike!


## Content
Include the relevant details, images (from the `images` folder), or diagrams here.

![Put together the first pieces](/projects/project-documentation-handyladen/images/first.jpg)
![Light! It works](/projects/project-documentation-handyladen/images/itworked.png)
![Added a switch](/projects/project-documentation-handyladen/images/button.jpg)
![Measured](/projects/project-documentation-handyladen/images/messungen.jpg)
![Soldered everything](/projects/project-documentation-handyladen/images/soldering.jpg)
![Put it in a box](/projects/project-documentation-handyladen/images/box.jpg)

[⬅ Back to Main Page](README.md)
