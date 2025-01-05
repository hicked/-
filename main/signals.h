#pragma once
#include <FastLED.h>
#include "brake.h"

#define LEFT_SIGNAL_PIN 9
#define RIGHT_SIGNAL_PIN 10

#define SIGNAL_COLOUR CRGB(255, 65, 0) // orange

#define SIGNAL_OFF_DELAY 1000 // if the signal pin is off for this long, turn off the signal
#define SIGNAL_SPEED 20 // 60 to 120 flashes per minute check this legal requirement
#define SIGNAL_LENGTH 26 // length of the signal in LEDs
#define PAUSE_TIME 250

class Signals {
private:
    CRGB *LEDStrip; // Pointer to the LED strip
    unsigned long lastSignalUpdate = 0; // time of the last flash

    unsigned long leftLastHighTime = 0;
    unsigned long rightLastHighTime = 0;

    int numLEDs;
    int numActiveLEDs = 0;

    void LeftSignal();
    void RightSignal();
    void SetSolid(CRGB color);

public:
    bool left = false;
    bool right = false;

    Signals(CRGB *leds, int num_leds);
    void Update();
    void UpdateStates();
};
