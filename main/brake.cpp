#include "Brake.h"

Brake::Brake(CRGB *leds, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
}

void Brake::Update() {
    this->SetSolid(BACKLIGHT_COLOR);

    if (this->flashCount > 0) {
        this->FlashRedLEDs();
    } else {
        this->UpdateBrakeLEDs();
    }

    FastLED.show();
}

void Brake::FlashRedLEDs() {
    unsigned long currentMillis = millis();

    if (currentMillis - this->lastFlashTime >= FLASH_SPEED) {
        this->lastFlashTime = currentMillis;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->SetSolid(CRGB::Red); // Flash red
        } else {
            this->SetSolid(BACKLIGHT_COLOR); // Reset to background
        }
        this->flashCount--;
    }
}

void Brake::UpdateBrakeLEDs() {
    if (!this->accelerating) {
        for (int i = 0; i < this->num_brake_leds; i++) {
            this->LEDStrip[this->num_brake_leds / 2 + i] = CRGB(0, this->active_brightness, 0); // red brake color
            this->LEDStrip[this->num_brake_leds / 2 - i - 1] = CRGB(0, this->active_brightness, 0);
        }
    } else if (this->accelerating) {
        for (int i = 0; i < this->numLEDs; i++) {
            this->LEDStrip[this->num_brake_leds / 2 + i] = CRGB(this->active_brightness, 0, 0); // Green brake color
            this->LEDStrip[this->num_brake_leds / 2 - i - 1] = CRGB(this->active_brightness, 0, 0);
        }
    }
}

void Brake::SetSolid(CRGB color) {
    fill_solid(this->LEDStrip, this->numLEDs, color);
}