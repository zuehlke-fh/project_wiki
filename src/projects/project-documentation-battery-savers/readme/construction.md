[⬅ Back to Main Page](README.md)

# Construction: [Battery Saveinator 2000]

## Description
The Symbiosis of the prototype stems from the fact that the physical device will be controlled via
a web application accessed vie the mobile phone.

![Wifi Credentials](/projects/project-documentation-battery-savers/images/code1.png)

By default, the user has to enable a WIFI Hotspot with the name “EjectorHotspot” and password
“EjectorPassword” using their phone. The device will then automatically connect to it and enable
access to its web app.

The original plan was to read the phone’s battery charge to determine when the power should
disconnect, but that proved to be rather difficult so we opted for a simple timer based disconnect
instead.

![Prototype webapp of device](/projects/project-documentation-battery-savers/images/UiScreenshot.png)

Once the phone connects to the device, the user gains access to the web app which lets them
set a time in hours and minutes. When pressing “Start Timer”, the set time is sent to the ESP32
to initiate the countdown. This makes it so that once the time is set, the device will continue
functioning without the phone itself, meaning the user can turn off their phone and the device
will disconnect the power at the set time regardless.

![Setup code](/projects/project-documentation-battery-savers/images/code2.png)

The built version of the prototype consists of a cardboard box with a built-in trapdoor to support
the wireless charging pad.

![Crosssection diagram of prototype](/projects/project-documentation-battery-savers/images/illustration1.png)

The wireless charging pad is continuously plugged in, but since these pads only have an
effective charging range of ~3 to 5 mm, simply lowering it a bit will cause the phone to stop
charging.

The trapdoor is held up by a stepper motor with a lever. Once the time set by the web app has
elapsed, the lever will rotate out of the way and cause the charger to drop, creating more than
enough clearance to stop the charging process.

![Loop and stepper motor code](/projects/project-documentation-battery-savers/images/code3.png)

Once the sequence is complete, the device has to be manually reset by pushing up the trapdoor
again. The lever can be instantly rotated by using the web app and setting the time to 0h-0m.

---

[⬅ Back to Main Page](README.md)
