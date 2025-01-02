# Motorcycle-Signals
As we know, rear-ends, while not the most fatal, are considered to be the most common type of collision.
Therefore, this is an Arduino project focused on increasing road safety of motorcyclists by making them more visible from behind, as well as better communicate their intentions on the roadways.

## The Problems
Motorcyclists know that there are two fundamental problems with motorcycle brake lights:
1. The first is that they are far too small, sometimes going by unnoticed. This is one of the problems that this projects aims to fix, by incorporating a center flashing element, which is sure to get drivers attention.
2. The second, and arguably more dangerous problem, is that the rear light is activated based solely on brake input. This means that if the motorcyclist chooses to downshift instead of brake, the brake lights will never turn on. This varies from bike to bike, but regardless, it would be important to communicate this information to the riders behind us, as all they care about is whether were slowing down or not, regardless of brake activation.
These two effects are hightened even more when we consider the weight of motorcycles, sometimes cutting their braking distances by half in some ideal circumstances. 

## The Solutions
Thus, this project will have a few set goals:
1. Use a gyroscope to detect braking instead of brake input (bumps will have to be filtered out).
3. Lightly flash the center of the brake light when braking.
4. Flash the entire brake light when emergency braking is detected.
5. Flash the entire brake light when braking has been initialize (when braking for the first time).
6. The brake light will light up MORE the harder the deceleration (*Ex; at -1m/s^2 have 10 LEDs turn on, when -2m/s^2 have 20 LEDs turn on, etc*).
7. Incorporate sequential turn signals.

## Circuit Diagram
![Screenshot 2025-01-02 053829](https://github.com/user-attachments/assets/b2a8437c-69d4-48ec-aa90-20a69da06fd5)

## Other Media
![VID20241231170116](https://github.com/user-attachments/assets/03d63844-a77f-4636-bbd6-6810d4615508)
