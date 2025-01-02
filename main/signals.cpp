#include "signals.h"

Signals::Signals(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Signals::Update() {
    unsigned long currentTime = millis();

    this->UpdateStates();
    if (this->numActiveLEDs < SIGNAL_LENGTH && currentTime - lastSignalUpdate > SIGNAL_SPEED && (this->left || this->right)) {
        lastSignalUpdate = currentTime;
        this->numActiveLEDs++;
    }
    // pauses when bar is full momentarily
    else if (this->numActiveLEDs >= SIGNAL_LENGTH && currentTime - lastSignalUpdate > PAUSE_TIME) {
        lastSignalUpdate = currentTime;
        this->numActiveLEDs = 0;
    }

    if (left) {
        LeftSignal();
    } 
    if (right) {
        RightSignal();
    }
    if (!right && !left) {
        this->numActiveLEDs = 0;
    }
}

void Signals::UpdateStates() {
    // Left turn signal
    if (digitalRead(LEFT_SIGNAL_PIN) == HIGH) {
        this->left = true;
        leftLastHighTime = millis();
    } else if (this->left && (millis() - leftLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->left = false;
    }

    // Right turn signal
    if (digitalRead(RIGHT_SIGNAL_PIN) == HIGH) {
        this->right = true;
        rightLastHighTime = millis();
    } else if (this->right && (millis() - rightLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->right = false;
    }
}

void Signals::RightSignal() {
    int startIndex = numLEDs / 2 + (numLEDs / 2 - SIGNAL_LENGTH);

    for (int i = 0; i < numActiveLEDs; i++) {
        FastLED.setBrightness(255);
        LEDStrip[startIndex+i] = SIGNAL_COLOUR; 
    }
}

void Signals::LeftSignal() {
    int startIndex = numLEDs / 2 - (numLEDs / 2 - SIGNAL_LENGTH) - 1;

    for (int i = 0; i < numActiveLEDs; i++) {
        FastLED.setBrightness(255);
        LEDStrip[startIndex-i] = SIGNAL_COLOUR; 
    }
}

void Signals::SetSolid(CRGB color) {
    fill_solid(this->LEDStrip, this->numLEDs, color);
}