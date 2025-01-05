# Motorcycle-Signals
As we know, rear-ends, while not the most fatal, are considered to be the most common type of collision. However, for motorcyclist, rear-ends can still be very dangerous.
Therefore, this is an Arduino project focused on increasing road safety for motorcyclists by making them more visible from behind, as well as better communicate their intentions on roadways.

## The Problems
Motorcyclists know that there are two fundamental problems with their brake lights:
1. The first is that they are far too small, making it hard to judge their distance, sometimes going completely unnoticed. This is one of the problems that this projects aims to fix, by incorporating a center flashing element, which is sure to get drivers attention.
2. The second, and arguably more dangerous problem, is that the rear light is activated solely based on brake input. This means that if the motorcyclist chooses to downshift instead of using their brakes, the brake lights will never turn on. This varies from bike to bike, but regardless, it would be important to communicate this information to the drivers behind us, as all they care about is whether were slowing down or not, regardless of brake activation.
These two effects are hightened even more when we consider the weight of motorcycles, sometimes cutting their braking distances by half in ideal circumstances. 

## The Solutions
Thus, this project will have a few set goals:
1. Use a gyroscope to detect braking instead of brake input (bumps will have to be filtered out).
3. Lightly flash the center of the brake light when braking.
4. Flash the entire brake light when emergency braking is detected.
5. Flash the entire brake light when braking has been initialize (when braking for the first time).
6. The brake light will light up MORE the harder the deceleration (*Ex; at -1m/s^2 have 10 LEDs turn on, when -2m/s^2 have 20 LEDs turn on, etc*).
7. Incorporate sequential turn signals.

## Circuit Diagram
A couple notes:
1. Two DC-DC Buck converters are used, one  to drive the 12V LEDs and one to drive the 5V Arduino Nano.
2. Note that, at full brightness, the WS2815s do run hot: Each LED can draw 0.3W. 66 LEDs means 20W of power. At 11V max brightness the current draw is around 0.75A. If the LEDs or Buck are getting hot, either lower the Buck output voltage, or brightness of the LEDs.

![Screenshot 2025-01-04 022818](https://github.com/user-attachments/assets/97b3e572-dd8e-40e1-9ff4-1bbe8d284469)


## Other Media
### Hazards
![VID20241231170116](https://github.com/user-attachments/assets/03d63844-a77f-4636-bbd6-6810d4615508)
