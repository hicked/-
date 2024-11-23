#include "brake.h"

Brake::Brake(byte data_pin, byte num_leds, byte brightness)
    : LEDStrip(data_pin, num_leds, brightness) { // replaced ledstrip constructor
    // Initialize to background color
    fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);
    FastLED.show();
}

void Brake::FlashRedLEDs(byte flash_speed, byte flash_length) {
    unsigned long currentMillis = millis();

    // Handle flashing logic
    if (flashCount > 0) {
        if (currentMillis - lastFlashTime >= flash_speed) {
            lastFlashTime = currentMillis;
            flashON = !flashON;

            if (flashON) {
                fill_solid(leds, NUM_LEDS, CRGB::Red);  // Flash red
            } else {
                fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);  // Reset to background color
            }
            FastLED.show();
            flashCount--;
        }
    } else {
        flashCount = 0;
        fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);  // Reset to background color
        flashON = false;
    }
}

void Brake::SetBrakeLEDs(int num_leds, int brightness) {
    // Light up LEDs symmetrically around the center
    for (int i = 0; i < num_leds; i++) {
        leds[NUM_LEDS / 2 + i] = CRGB(0, brightness, 0); // Green brake color
        leds[NUM_LEDS / 2 - i - 1] = CRGB(0, brightness, 0);
    }
}

void Brake::update() {
    SetBrakeLEDs(num_braking_leds, active_brake_brightness);
    FlashRedLEDs(FLASH_SPEED, flashCount);
    FastLED.show();
}
