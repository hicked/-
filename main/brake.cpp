#include "Brake.h"

Brake::Brake(Signals *signal, CRGB *leds, Gyro *gyro, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
    this->signals = signal;
    this->gyro = gyro;
}

void Brake::Update() {
    this->SetSolid(BACKLIGHT_COLOR);
     if (!gyro->accelerating) {
        if (this->prevNumActiveLEDs <= INITIAL_BRAKE_THRESHOLD && this->numActiveLEDs > INITIAL_BRAKE_THRESHOLD && millis() - this->timeSinceLastIniBraking > TIME_BETWEEN_INI_BRAKE) {
            timeSinceLastIniBraking = millis();
            flashCount = INITIALIZE_BRAKING_FLASH_LENGTH;
        }
        else if (this->numActiveLEDs > EMERGENCY_BRAKING_THRESHOLD && flashCount == 0) {
            flashCount++; // continuously adds one or flashes forever
        }

        this->prevNumActiveLEDs = this->numActiveLEDs;
    }
}

void Brake::FlashRedLEDs() {
    unsigned long currentMillis = millis();

    if (currentMillis - this->lastFlashTime >= FLASH_DELAY) {
        this->lastFlashTime = currentMillis;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->SetSolid(CRGB(MAX_BRAKE_BRIGHTNESS, 0, 0)); // Flash red
            this->flashCount--;
        } else {
            this->SetSolid(BACKLIGHT_COLOR); // Reset to background
        }
    }
}

void Brake::UpdateBrakeLEDs() {
    int middleIndex = this->numLEDs / 2;

    if (!gyro->accelerating) {
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1; 

            if (index1 >= 0 && index1 < this->numLEDs && !signals->left) {
                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
            }
            if (index2 >= 0 && index2 < this->numLEDs && !signals->right) {
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }
        }
    } else if (gyro->accelerating && SHOW_ACCEL) {
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            
            if (index1 >= 0 && index1 < this->numLEDs && !signals->left) {
                this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // Green brake color
            }
            if (index2 >= 0 && index2 < this->numLEDs && !signals->right) {
                this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
            }
        }
    }
}

void Brake::SetSolid(CRGB color) {
    fill_solid(this->LEDStrip, this->numLEDs, color);
}

void Brake::MarioStar() {
    static int position = 0; // Tracks the current position of the marquee
    unsigned long currentMillis = millis();

    if (currentMillis - this->lastRainbowTime >= 10) { 
        this->lastRainbowTime = currentMillis;

        // Clear all LEDs first (optional but ensures clean transitions)
        fill_solid(this->LEDStrip, this->numLEDs, CRGB::Black);
        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position of the lit LED in the marquee
            int ledPosition = (position + i) % this->numLEDs;
            this->LEDStrip[ledPosition] = CHSV((i * 256 / this->numLEDs + currentMillis / 10) % 256, 255, MAX_BRAKE_BRIGHTNESS);
        }
        position = (position + 1) % this->numLEDs;
    }
}
