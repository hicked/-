#pragma once
#include <FastLED.h>
#include "brake.h"

#define BACKLIGHT_COLOR CRGB(0, 1, 0) // Dim green for the background
#define SIGNAL_COLOUR CRGB(255, 255, 0) // Dim yellow for the signal
#define SIGNAL_BRIGHTNESS 255
#define SIGNAL_DELAY 20


class Signals {
private:
    Brake *brake;
    CRGB *LEDStrip; // Pointer to the LED strip
    unsigned long lastSignalUpdate = 0; // time of the last flash
    int numLEDs;
    int prevNumActiveLEDs = 0;

    void LeftSignal();
    void RightSignal();
    void SetSolid(CRGB color);

public:
    bool left = false;
    bool right = false;

    Signals(Brake *brake, CRGB *leds, int num_leds);
    void Update();
};
