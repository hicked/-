#pragma once
#include <FastLED.h>
#include "signals.h"

#define BACKLIGHT_COLOR CRGB(0, 10, 0) // Dim green for the background
#define GLOBAL_BRIGHTNESS 255
#define MIN_BRAKE_BRIGHTNESS 200
#define MAX_BRAKE_BRIGHTNESS 255
#define FLASH_DELAY 25
#define INITIALIZE_BRAKING_FLASH_LENGTH 7 // number of flashes
#define EMERGENCY_BRAKING_THRESHOLD 32 // note this is number of LEDs FROM THE CENTER
#define MARIO_STAR_THRESHOLD 32
#define INITIAL_BRAKE_THRESHOLD 0 // note this is number of LEDs FROM THE CENTER
#define TIME_BETWEEN_INI_BRAKE 5000 // cant continuously initialize braking if gyro is acting up
#define SHOW_ACCEL true
#define SHOW_MARIO true

class Signals;

class Brake {
private:
    CRGB *LEDStrip; // Pointer to the LED strip
    Signals *signals;
    bool flashON = false; // status while flashing
    unsigned long lastFlashTime = 0; // time of the last flash
    unsigned long lastRainbowTime = 0; // time of the last rainbow
    int numLEDs;
    int prevNumActiveLEDs = 0;
    void SetSolid(CRGB color);
    unsigned long timeSinceLastIniBraking = 0;
public:
    
    bool initializedBraking = false; // status of the braking
    byte active_brightness;
    bool accelerating = false;
    int flashCount = 0; // amount of times that the leds need to flash will be reduced by one each time by FlashRedLEDs
    int numActiveLEDs = 0;

    Brake(Signals* signal, CRGB *leds, int num_leds);
    void Update();
    void FlashRedLEDs();
    void UpdateBrakeLEDs();
    void MarioStar();
};
