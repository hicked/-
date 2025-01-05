#pragma once
#include <FastLED.h>
#include "signals.h"
#include "gyro.h"

// Note, all CRGB values are in GRB format for now
#define BRAKE_PIN 11 // Pin for the brake lights

#define BACKLIGHT_COLOR CRGB(10, 0, 0) // Dim red for the background
#define BLIND_COLOR CRGB(255, 255, 255) // white

#define GLOBAL_BRIGHTNESS 200 // 0-255, this might be broken since it gets overriden
#define MIN_BRAKE_BRIGHTNESS 75 // 0-255 minimum brightness of the brake when braking
#define MAX_BRAKE_BRIGHTNESS 200 //0-255 maximum brightness of the brake when braking

#define CENTER_FLASH_SPEED 50 // flashrate of the center part lower is faster
#define CENTER_FLASH_BRIGHTNESS 200 // brightness of the flashing part of the brake
#define CENTER_FLASH_WIDTH 4 // Width of the flashing center part of the brake. should be even

#define FLASH_SPEED 25 // delay between flashes for emergency braking and initialization of brakes
#define INITIALIZE_BRAKING_FLASH_LENGTH 10 // Number of brake flahses when initializing braking

#define TIME_BETWEEN_INI_BRAKE 3000 // initialization of braking detection can only happen every x milliseconds

#define SHOW_ACCEL true // show acceleration (green)
#define SHOW_MARIO true // show mario star (rainbow)

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

    int numLEDs; // number of LEDs, from header in main.ino
    int middleIndex; // middle index of the LED strip (+1 since even)  xxxoxx
    
    void SetSolid(CRGB color);
    void FlashCenterLEDs();

public:
    bool initializedBraking = false; // status of the braking
    int active_brightness; // brightness of the brake, based on how hard you brake
    int flashCount = 0; // amount of times that the leds need to flash will be reduced by one each time by FlashRedLEDs
    int numActiveLEDs = 0; // number of LEDs from the center that are ON, based on how hard you brake

    bool blind = false;

    bool brakeON = false; // status of the brake based on wire input

    Brake(Signals *signal, CRGB *leds, Gyro *gyro, int num_leds);
    void Update(); // update the brake lights. numActiveLEDs and active_brightness must be changed externally (main)
    void FlashRedLEDs(); // flash led strip red based on flashCountS
    void MarioStar(); // show mario star on the brake lights

};
