#include "Brake.h"

Brake::Brake(Signals *signal, CRGB *leds, Gyro *gyro, int num_leds) {
    this->LEDStrip = leds;
    this->numLEDs = num_leds;
    this->signals = signal;
    this->gyro = gyro;
    this->middleIndex = this->numLEDs / 2;
}

void Brake::Update() {
    unsigned long currentTime = millis();

    if (this->blind) {
        this->SetSolid(BLIND_COLOR);
        return;
    }
    

    this->SetSolid(BACKLIGHT_COLOR);

    this->brakeON = digitalRead(BRAKE_PIN);

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

                this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // red brake color
                this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
            }         
        } 
        // Otherwise, use progressive brake lighting
        else {
            for (int i = 0; i < this->numActiveLEDs; i++) {
                int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
                int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1; 

                this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // red brake color
                this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
            }
        }
    } 
    
    else if (gyro->smoothedAcc > MIN_GYRO_ACCELERATING && SHOW_ACCEL) { // Accelerating
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            
            this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // Green brake color
            this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
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
            this->LEDStrip[index1] = CRGB(0, CENTER_FLASH_BRIGHTNESS, 0); // Flash red
            this->LEDStrip[index2] = CRGB(0, CENTER_FLASH_BRIGHTNESS, 0);
        }
    }
}

void Brake::FlashRedLEDs() { // Flashing of the entire LED strip
    unsigned long currentTime = millis();

    if (currentTime - this->lastFlashTime >= FLASH_SPEED) {
        this->lastFlashTime = currentTime;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->SetSolid(CRGB(0, MAX_BRAKE_BRIGHTNESS, 0)); // Flash red
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
    unsigned long currentTime = millis();

    if (currentTime - this->lastRainbowTime >= 10) { 
        this->lastRainbowTime = currentTime;

        // Clear all LEDs first (optional but ensures clean transitions)
        fill_solid(this->LEDStrip, this->numLEDs, CRGB::Black);
        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position of the lit LED in the marquee
            int ledPosition = (position + i) % this->numLEDs;
            this->LEDStrip[ledPosition] = CHSV((i * 256 / this->numLEDs + currentTime / 10) % 256, 255, MAX_BRAKE_BRIGHTNESS);
        }
        position = (position + 1) % this->numLEDs;
    }
}
