#pragma once
#include <FastLED.h>

#define LED_DATA_PIN 7
#define NUM_LEDS 66
#define LED_TYPE WS2812B
#define GLOBAL_BRIGHTNESS 255


class LEDStrip {
    protected:
        CRGB leds[NUM_LEDS];
        int num_braking_leds = 0;
        int active_brake_brightness = 0;

    public:
        LEDStrip(byte data_pin, byte num_leds, byte brightness);
        void SetBrightness(byte brightness);
        void SetSolid(CRGB color);
        virtual Update();
};