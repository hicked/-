#pragma once
#include <FastLED.h>
#include "brake.h"

#define LEFTS_SIGNAL_PIN 9
#define RIGHT_SIGNAL_PIN 10

#define BACKLIGHT_COLOR CRGB(20, 0, 0)
#define SIGNAL_COLOUR CRGB(255, 100, 0)
#define SIGNAL_BRIGHTNESS 255

#define SIGNAL_OFF_DELAY 1500 // if the signal pin is off for this long, turn off the signal
#define SIGNAL_DELAY 15 // 60 to 120 flashes per minute check this legal requirement
#define SIGNAL_LENGTH 5 // length of the signal in LEDs

class Signals {
private:
    CRGB *LEDStrip; // Pointer to the LED strip
    unsigned long lastSignalUpdate = 0; // time of the last flash

    unsigned long leftLastHighTime = 0;
    unsigned long rightLastHighTime = 0;

    int numLEDs;
    int prevNumActiveLEDs = 0;

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
