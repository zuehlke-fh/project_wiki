[⬅ Back to Main Page](README.md)

# Learnings: [Power of the Universe]

Probably the most significant lesson learned form this project is the fact that when you want to
be able to move parts, you should try to let gravity do as much work as possible and have as
few moving parts as possible.

In our case the primary issue with the designs was usually the amount of force we could create
with the tools we had (specifically the stepper motor).

One of the earliest ideas for the device
was to have the platform with the charging pad lift the phone away and off the device

![first concept for phone-removal mechanism](/projects/project-documentation-battery-savers/images/illustration2.png)

But doing this requires a lot of torque which we weren't sure we really even had with the motors
available. Additionally, using multiple motors could lead to asynchronous execution and make
the whole thing unreliable, not to mention requiring even more power.

Another thing we decided to do was to implement the motor in a way where the weight of the
trapdoor rests on the motor in a way that the force cannot shift the position of the rotor in a way
that isn’t intended.

![demonstration of advantageous rotor position](/projects/project-documentation-battery-savers/images/illustration3.png)

While figure left could maybe permit an easier implementation of an automatic device reset,
figure right (which we used) made the whole system more reliable short term, which was
favorable for the prototype we were creating.

---

[⬅ Back to Main Page](README.md)
