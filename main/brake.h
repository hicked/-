#pragma once
#include <FastLED.h>
#include "signals.h"
#include "gyro.h"

#define BACKLIGHT_COLOR CRGB(0, 10, 0) // Dim red for the background
#define GLOBAL_BRIGHTNESS 255 // 0-255, this might be broken

#define MIN_BRAKE_BRIGHTNESS 100 // 0-255
#define MAX_BRAKE_BRIGHTNESS 255 //0-255

#define CENTER_FLASH_DELAY 50 // flashrate of the center part
#define CENTER_FLASH_BRIGHTNESS 200

#define FLASH_DELAY 25 // delay between flashes for emergency and initialization of brakes
#define INITIALIZE_BRAKING_FLASH_LENGTH 10 // number of flashes

#define TIME_BETWEEN_INI_BRAKE 3000 // cant continuously initialize braking if gyro is acting up
#define CENTER_FLASH_WIDTH 4 // Width of the flashing center part of the brake. should be even

#define SHOW_ACCEL true
#define SHOW_MARIO true

class Signals;

class Brake {
private:
    CRGB *LEDStrip; // Pointer to the LED strip
    Signals *signals;
    Gyro *gyro;

    bool flashON = false; // status while flashing
    bool centerFlashON = false; // status while center flashing

    unsigned long lastFlashTime = 0; // time of the last flash
    unsigned long lastCenterFlashTime = 0; // time of the last center flash
    unsigned long lastRainbowTime = 0; // time of the last rainbow
    unsigned long timeSinceLastIniBraking = 0; // time since last initialized braking

    int numLEDs;
    int middleIndex;
    
    void SetSolid(CRGB color);

public:
    bool initializedBraking = false; // status of the braking
    byte active_brightness;
    int flashCount = 0; // amount of times that the leds need to flash will be reduced by one each time by FlashRedLEDs
    int numActiveLEDs = 0;

    Brake(Signals *signal, CRGB *leds, Gyro *gyro, int num_leds);
    void Update();
    void FlashRedLEDs();
    void MarioStar();
    void FlashCenterLEDs();
};
