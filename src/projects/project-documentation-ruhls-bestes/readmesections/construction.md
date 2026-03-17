# [⬅ Back to Main Page](../README.md)

# Construction

The project *Delamains Dilemma* consists of a remote-controlled car operated via a smartphone application. Communication between the app and the vehicle is handled wirelessly, allowing real-time control of speed and direction.

A central part of the construction is the behavioral logic implemented in the system. The car continuously monitors how long it is driven at high speed. If a predefined threshold is exceeded, the system enters a non-compliant state.

During this state, the vehicle ignores all incoming control signals and becomes temporarily non-operational. The duration of this self-regulated lockout phase is randomly generated, preventing predictable user strategies.

The construction combines physical components for movement and communication with software logic that manages system states such as normal operation, overload detection, and rebellion.

## Prototype Documentation

The following images document the final physical prototype.

The first image shows the vehicle with its 3D-printed body, representing the external appearance of the object.  
The second image shows the prototype without the casing, exposing all internal components and wiring.

![Final prototype with 3D-printed body](/projects/project-documentation-ruhls-bestes/images/Car.png)
*Figure 1: Fully assembled prototype with 3D-printed casing.*

![Open prototype showing internal components](/projects/project-documentation-ruhls-bestes/images/Prototype.png)
*Figure 2: Prototype with removed casing, showing internal structure and electronics.*

[⬅ Back to Main Page](../README.md)
