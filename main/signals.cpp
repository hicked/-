#include "signals.h"

Signals::Signals(Brake *brake, CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
    this->brake = brake;
}

void Signals::Update() {
    unsigned long currentTime = millis();
    if (currentTime - lastSignalUpdate > SIGNAL_DELAY) {
        lastSignalUpdate = currentTime;
        if (left) {
            Serial.println("left on");
            LeftSignal();
        } else if (right) {
            RightSignal();
        }
        else {
            prevNumActiveLEDs = 0;
        }
        FastLED.show(); // Ensure the changes are reflected
    }
}

void Signals::RightSignal() {
    int middleIndex = numLEDs / 2;
    this->prevNumActiveLEDs++;
    if (this->prevNumActiveLEDs >= middleIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
    }
    for (int i = middleIndex + this->brake->numActiveLEDs; i >= middleIndex - this->prevNumActiveLEDs; i--) {
        LEDStrip[i] = SIGNAL_COLOUR;
    }
}

void Signals::LeftSignal() {
    int middleIndex = numLEDs / 2;
    this->prevNumActiveLEDs++;
    if (this->prevNumActiveLEDs >= middleIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
    }
    for (int i = middleIndex + this->brake->numActiveLEDs; i <= middleIndex + this->prevNumActiveLEDs; i++) {
        LEDStrip[i] = SIGNAL_COLOUR;
    }
}

void Signals::SetSolid(CRGB color) {
    for (int i = 0; i < numLEDs; i++) {
        LEDStrip[i] = color;
    }
}