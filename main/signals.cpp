#include "signals.h"

Signals::Signals(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Signals::Update() {
}

void Signals::UpdateSignals() {
    unsigned long currentTime = millis();
    if (currentTime - lastSignalUpdate > SIGNAL_DELAY) {
        lastSignalUpdate = currentTime;
        if (left) {
            LeftSignal();
        } else if (right) {
            RightSignal();
        }
        else {
            this->prevNumActiveLEDs = 0;
        }
    }
}

void Signals::RightSignal() {
    int middleIndex = numLEDs / 2;
    this->prevNumActiveLEDs++;
    if (this->prevNumActiveLEDs >= middleIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
    }
    // for (int i = middleIndex; i >= middleIndex - this->prevNumActiveLEDs; i--) {
    //     LEDStrip[i] = SIGNAL_COLOUR;
    // }
    this->prevNumActiveLEDs++;
    LEDStrip[this->prevNumActiveLEDs] = SIGNAL_COLOUR;
}

void Signals::LeftSignal() {
    int middleIndex = numLEDs / 2;
    this->prevNumActiveLEDs++;
    if (this->prevNumActiveLEDs >= middleIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
    }
    for (int i = middleIndex; i <= middleIndex + this->prevNumActiveLEDs; i++) {
        LEDStrip[i] = SIGNAL_COLOUR;
    }
}

void Signals::SetSolid(CRGB color) {
    for (int i = 0; i < numLEDs; i++) {
        LEDStrip[i] = color;
    }
}