#pragma once
#include <FastLED.h>

#define BACKLIGHT_COLOR CRGB(1, 1, 0) // Dim green for the background
#define GLOBAL_BRIGHTNESS 255
#define MIN_BRAKE_BRIGHTNESS 50
#define MAX_BRAKE_BRIGHTNESS 150
#define FLASH_SPEED 50
#define BACKLIGHT_BRIGHTNESS 1
#define EMERGENCY_BRAKING_FLASH_LENGTH 25
#define INITIALIZE_BRAKING_FLASH_LENGTH 7
#define EMERGENCY_BRAKING_THRESHOLD 95
#define MIN_GYRO_BRAKE_THRESHHOLD 5

class Brake {
private:
    CRGB *LEDStrip; // Pointer to the LED strip
    bool flashON = false; // status while flashing
    unsigned long lastFlashTime = 0; // time of the last flash
    int numLEDs;

public:
    bool initializedBraking = false; // status of the braking
    bool initializedEmergencyBraking = false; // status of the emergency 
    byte active_brightness;
    bool accelerating = false;
    int flashCount = 0; // amount of times that the leds need to flash will be reduced by one each time by FlashRedLEDs
    int num_brake_leds = 0;

    byte initializedBrakingNumFlashes = INITIALIZE_BRAKING_FLASH_LENGTH;
    byte emergencyBrakingNumFlashes = EMERGENCY_BRAKING_FLASH_LENGTH;

    byte emergencyBrakingThreshold = EMERGENCY_BRAKING_THRESHOLD;
    byte minGyroBrakeThreshold = MIN_GYRO_BRAKE_THRESHHOLD;
    
    byte minBrakeBrightness = MIN_BRAKE_BRIGHTNESS;
    byte maxBrakeBrightness = MAX_BRAKE_BRIGHTNESS;

    Brake(CRGB *leds, int num_leds);
    void Update();
    void FlashRedLEDs();
    void UpdateBrakeLEDs();
    void SetSolid(CRGB color);
};
