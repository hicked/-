#include "Brake.h"

Brake::Brake(Signals *signal, CRGB *leds, Gyro *gyro, Button *button, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
    this->signals = signal;
    this->gyro = gyro;
    this->button = button;
    this->middleIndex = this->numLEDs / 2;

    fill_solid(this->LEDStrip, this->numLEDs, CRGB::Black);
}

void Brake::Update() {
    unsigned long currentTime = millis();

    if (this->button->mode == BRAKE_MODE_MARIO_STAR) {
        this->MarioStarMode();
        return;
    } else if (this->button->mode == BRAKE_MODE_CHRISTMAS) {
        this->ChristmasMode();
        return;
    } else if (this->button->mode == BRAKE_MODE_HALLOWEEN) {
        this->HalloweenMode();
        return;
    } else if (this->button->mode == BRAKE_MODE_FLASHLIGHT) {
        this->FlashlightMode();
        return;
    }


    this->SetSolid(BACKLIGHT_COLOUR);

    this->brakeON = !digitalRead(BRAKE_PIN);

    if (gyro->smoothedAcc < -MIN_GYRO_BREAKING) { // breaking
        // Flash the LEDs in the center
        this->FlashCenterLEDs();

        // Emergency braking detected
        if (gyro->smoothedAcc < -MAX_GYRO_BREAKING && flashCount == 0) {
            flashCount++; // continuously adds one or flashes forever
        }

        // Initialization of braking detected
        else if (gyro->prevAcc >= -MIN_GYRO_BREAKING && millis() - this->timeSinceLastIniBraking > TIME_BETWEEN_INI_BRAKE) {
            timeSinceLastIniBraking = millis();
            flashCount = INITIALIZE_BRAKING_FLASH_LENGTH;
        }

        // If signals on, use static braking (not progressive)
        if (signals->left || signals->right) {
            for (int i = 0; i < numLEDs/2 - CENTER_FLASH_WIDTH/2 - SIGNAL_LENGTH; i++) {
                int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
                int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1; 

                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }         
        } 
        // Otherwise, use progressive brake lighting
        else {
            for (int i = 0; i < this->numActiveLEDs; i++) {
                int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
                int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1; 

                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }
        }
    } 
    
    else if (gyro->smoothedAcc > MIN_GYRO_ACCELERATING && SHOW_ACCEL) { // Accelerating
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            
            this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // Green brake color
            this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
        }
    }
}


void Brake::FlashCenterLEDs() { // Flashing of the center LEDs
    unsigned long currentTime = millis();
    // If the time since the last flash is greater than the delay betweem flashes
    if (currentTime - this->lastCenterFlashTime >= CENTER_FLASH_SPEED) {
        this->lastCenterFlashTime = currentTime;
        this->centerFlashON = !this->centerFlashON;
    }

    for(int i = 0; i < CENTER_FLASH_WIDTH/2; i++) {
        int index1 = middleIndex + i;
        int index2 = middleIndex - i - 1;

        if (this->centerFlashON) {
            this->LEDStrip[index1] = CENTER_FLASH_COLOUR; // Flash red
            this->LEDStrip[index2] = CENTER_FLASH_COLOUR;
        }
    }
}

void Brake::FlashRedLEDs() { // Flashing of the entire LED strip
    unsigned long currentTime = millis();

    if (currentTime - this->lastFlashTime >= FLASH_SPEED) {
        this->lastFlashTime = currentTime;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->SetSolid(FLASH_COLOUR); // Flash red
            this->flashCount--;
        } else {
            this->SetSolid(BACKLIGHT_COLOUR); // Reset to background
        }
    }
}

void Brake::SetSolid(CRGB colour) {
    fill_solid(this->LEDStrip, this->numLEDs, colour);
}


void Brake::MarioStarMode() {
    int shift = 0; // Tracks the current position of the marquee
    unsigned long currentTime = millis();

    if (currentTime - this->lastMarioTime >= 10) { 
        this->lastMarioTime = currentTime;

        // Clear all LEDs first (optional but ensures clean transitions)
        fill_solid(this->LEDStrip, this->numLEDs, CRGB::Black);
        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position of the lit LED in the marquee
            int effectiveIndex = (shift + i) % this->numLEDs;
            this->LEDStrip[effectiveIndex] = CHSV((i * 256 / this->numLEDs + currentTime / 10) % 256, 255, MAX_BRAKE_BRIGHTNESS);
        }
        shift = (shift + 1) % this->numLEDs;
    }
}

void Brake::ChristmasMode() {
    unsigned long currentTime = millis();

    if (currentTime - this->lastChristmasTime >= 100) { // Update every 100ms (adjust for speed)
        this->lastChristmasTime = currentTime;

        static int shift = 0; // Tracks the current shift of the pattern

        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position of the current LED, wrapping around the strip
            int effectiveIndex = (i + shift) % this->numLEDs;
            
            // Determine the color based on the effective index
            int segment = (effectiveIndex / 5) % 3; // Divide into groups of 5 LEDs

            if (segment == 0) {
                this->LEDStrip[i] = CRGB::Blue; // First segment is Blue
            } else if (segment == 1) {
                this->LEDStrip[i] = CRGB::Red; // Second segment is Red
            } else {
                this->LEDStrip[i] = CRGB::Green; // Third segment is Green
            }
        }

        // Shift the pattern smoothly
        shift = (shift + 1) % this->numLEDs; // Wrap every full cycle (length of one complete pattern)
    }
}


// Halloween mode: Alternating orange and purple
void Brake::HalloweenMode() {
    unsigned long currentTime = millis();

    if (currentTime - this->lastHalloweenTime >= 100) { // Update every 100ms (adjust for speed)
        this->lastHalloweenTime = currentTime;

        static int shift = 0; // Tracks the current shift of the pattern

        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position of the current LED, wrapping around the strip
            int effectiveIndex = (i + shift) % this->numLEDs;
            
            // Determine the color based on the effective index
            int segment = (effectiveIndex / 5) % 3; // Divide into groups of 5 LEDs

            if (segment == 0) {
                this->LEDStrip[i] = CRGB::Purple; // First segment is Purple
            } else if (segment == 1) {
                this->LEDStrip[i] = CRGB::Orange; // Second segment is Orange
            } else {
                this->LEDStrip[i] = CRGB::Black; // Third segment is Black
            }
        }

        // Shift the pattern smoothly
        shift = (shift + 1) % this->numLEDs; // Wrap every full cycle (length of one complete pattern)
    }
}

void Brake::FlashlightMode() {
    fill_solid(this->LEDStrip, this->numLEDs, FLASHLIGHT_COLOUR);
}
