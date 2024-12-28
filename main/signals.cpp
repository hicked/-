#include "signals.h"

Signals::Signals(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Signals::Update() {
    this->UpdateStates();
    if (millis() - lastSignalUpdate > SIGNAL_DELAY) {
        lastSignalUpdate = millis();
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

void Signals::UpdateStates() {
    // Left turn signal
    if (digitalRead(LEFT_SIGNAL_PIN) == HIGH) {
        this->left = true;
        leftLastHighTime = millis();
    } else if (leftSignalState && (millis() - leftSignalLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->left = false;
    }

    // Right turn signal
    if (digitalRead(RIGHT_SIGNAL_PIN) == HIGH) {
        this->right = true;
        rightLastHighTime = millis();
    } else if (rightSignalState && (millis() - rightSignalLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->right = false;
    }
}

void Signals::RightSignal() {
    int startIndex = numLEDs / 2 - (numLEDs / 2 - SIGNAL_LENGTH);
    this->prevNumActiveLEDs++;
    if (this->prevNumActiveLEDs >= startIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
        return;
    }
    for (int i = startIndex; i >= startIndex - this->prevNumActiveLEDs; i--) {
        LEDStrip[i] = SIGNAL_COLOUR;
    }
}

void Signals::LeftSignal() {
    int startIndex = numLEDs / 2 - (numLEDs / 2 - SIGNAL_LENGTH);
    this->prevNumActiveLEDs++;

    // signal is full, reset
    if (this->prevNumActiveLEDs >= startIndex) {
        SetSolid(BACKLIGHT_COLOR);
        this->prevNumActiveLEDs = 0;
        return;
    }
    for (int i = startIndex; i <= startIndex + this->prevNumActiveLEDs; i++) {
        LEDStrip[i] = SIGNAL_COLOUR;
    }
}

void Signals::SetSolid(CRGB color) {
    for (int i = 0; i < numLEDs; i++) {
        LEDStrip[i] = color;
    }
}