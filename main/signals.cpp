#include "signals.h"

Signals::Signals(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Signals::Update() {
    unsigned long currentTime = millis();

    this->UpdateStates();
    if (currentTime - lastSignalUpdate > SIGNAL_DELAY && (this->left || this->right)) {
        lastSignalUpdate = currentTime;
        this->numActiveLEDs++;
        if (this->numActiveLEDs > SIGNAL_LENGTH) {
            this->numActiveLEDs = 0;
        }
        Serial.println(this->numActiveLEDs);
        
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
    if (!digitalRead(LEFT_SIGNAL_PIN) == HIGH) {
        this->left = true;
        leftLastHighTime = millis();
    } else if (this->left && (millis() - leftLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->left = false;
    }

    // Right turn signal
    if (!digitalRead(RIGHT_SIGNAL_PIN) == HIGH) {
        this->right = true;
        rightLastHighTime = millis();
    } else if (this->right && (millis() - rightLastHighTime >= SIGNAL_OFF_DELAY)) {
        this->right = false;
    }
}

void Signals::RightSignal() {
    // int startIndex = numLEDs / 2 - (numLEDs / 2 - SIGNAL_LENGTH);
    // this->prevNumActiveLEDs++;
    // if (this->prevNumActiveLEDs >= startIndex) {
    //     SetSolid(BACKLIGHT_COLOR);
    //     this->prevNumActiveLEDs = 0;
    //     return;
    // }
    // for (int i = startIndex; i >= startIndex - this->prevNumActiveLEDs; i--) {
    //     LEDStrip[startIndex+i+1] = SIGNAL_COLOUR;
    // }
}

void Signals::LeftSignal() {
    int startIndex = numLEDs / 2 - (numLEDs / 2 - SIGNAL_LENGTH);

    for (int i = 0; i < numActiveLEDs; i++) {
        FastLED.setBrightness(255);
        LEDStrip[startIndex-i] = SIGNAL_COLOUR; 
    }
}

void Signals::SetSolid(CRGB color) {
    fill_solid(this->LEDStrip, this->numLEDs, color);
}