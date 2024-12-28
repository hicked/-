#include "Brake.h"

Brake::Brake(Signals *signal, CRGB *leds, Gyro *gyro, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
    this->signals = signal;
    this->gyro = gyro;
    this->middleIndex = this->numLEDs / 2;
}

void Brake::Update() {
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

                // dont need these if statements...?
                //
                //
                //
                //
                //
                //
                //
                //
                //
                //
                //
                //
                // check
                if (index1 >= 0 && index1 < this->numLEDs && !signals->left) {
                    this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
                }
                if (index2 >= 0 && index2 < this->numLEDs && !signals->right) {
                    this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
                }
            }
        }
    } 
    
    else if (gyro->smoothedAcc > MIN_GYRO_ACCELERATING && SHOW_ACCEL) { // Accelerating
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

void Brake::FlashCenterLEDs() {
    // If the time since the last flash is greater than the delay betweem flashes
    if (millis() - this->lastCenterFlashTime >= FLASH_DELAY) {
        int index1 = middleIndex + CENTER_FLASH_WIDTH - 1;
        int index2 = middleIndex - CENTER_FLASH_WIDTH - 2;
        
        this->lastCenterFlashTime = currentMillis;
        this->centerFlashON = !this->centerFlashON;

        for(int i = 0; i < CENTER_FLASH_WIDTH/2; i++) {
            int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
            int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1;

            if (this->centerFlashON) {
                this->LEDStrip[index1] = CRGB(MAX_BRAKE_BRIGHTNESS, 0, 0); // Flash red
                this->LEDStrip[index2] = CRGB(MAX_BRAKE_BRIGHTNESS, 0, 0);
            } else {
                this->LEDStrip[index1] = BACKLIGHT_COLOR; // Reset to background
                this->LEDStrip[index2] = BACKLIGHT_COLOR;
            }
        }
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
