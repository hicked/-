#pragma once
#include "LEDStrip.h"

#define BACKLIGHT_COLOR CRGB(0, 1, 0) // Dim green for the background
#define MIN_BRAKE_BRIGHTNESS  50
#define MAX_BRAKE_BRIGHTNESS  150
#define FLASH_SPEED 50
#define EMERGENCY_BRAKING_FLASH_LENGTH 25
#define INITIALIZE_BRAKING_FLASH_LENGTH 7

class Brake {
  private:
    unsigned long lastFlashTime = 0;
    bool initializedBraking = false; // Flag to track the first braking event
    bool flashON = false;
    int flashCount = 0;

  public:
    Brake(byte data_pin, byte num_leds, byte brightness);
    void FlashRedLEDs(byte flash_speed, byte flash_length);
    void SetBrakeLEDs(int num_leds, int brightness);
    void update() override;
};
