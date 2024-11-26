#include "Brake.h"

Brake::Brake(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Brake::Update() {
    this->SetSolid(BACKLIGHT_COLOR);
    if (!this->accelerating) {
        if (this->prevNumActiveLEDs <= INITIAL_BRAKE_THRESHHOLD && this->numActiveLEDs > INITIAL_BRAKE_THRESHHOLD) {
            flashCount = INITIALIZE_BRAKING_FLASH_LENGTH;
        }
        else if (this->prevNumActiveLEDs <= EMERGENCY_BRAKING_THRESHOLD && this->numActiveLEDs > EMERGENCY_BRAKING_THRESHOLD) {
            flashCount = EMERGENCY_BRAKING_FLASH_LENGTH;
        }
        this->prevNumActiveLEDs = this->numActiveLEDs;
    }

    if (this->flashCount > 0) {
        this->FlashRedLEDs();
    } else {
        this->UpdateBrakeLEDs();
    }

    FastLED.show();
}

void Brake::FlashRedLEDs() {
    unsigned long currentMillis = millis();

    if (currentMillis - this->lastFlashTime >= FLASH_DELAY) {
        this->lastFlashTime = currentMillis;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->SetSolid(CRGB(0, 255, 0)); // Flash red
        } else {
            this->SetSolid(BACKLIGHT_COLOR); // Reset to background
        }
        this->flashCount--;
    }
}

void Brake::UpdateBrakeLEDs() {
    int middleIndex = this->numLEDs / 2;

    if (!this->accelerating) {
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            if (index1 >= 0 && index1 < this->numLEDs) {
                this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // red brake color
            }
            if (index2 >= 0 && index2 < this->numLEDs) {
                this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
            }
        }
    } else if (this->accelerating) {
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            if (index1 >= 0 && index1 < this->numLEDs) {
                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // Green brake color
            }
            if (index2 >= 0 && index2 < this->numLEDs) {
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }
        }
    }
}

void Brake::SetSolid(CRGB color) {
    fill_solid(this->LEDStrip, this->numLEDs, color);
}