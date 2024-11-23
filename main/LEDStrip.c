#include "ledStrip.h"

LEDStrip::LEDStrip(byte data_pin, byte num_leds, byte brightness) {
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, GRB>(this.leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(GLOBAL_BRIGHTNESS);
    FastLED.show();
}

void LEDStrip::SetBrightness(byte brightness) {
    FastLED.setBrightness(brightness);
}

void LEDStrip::SetSolid(CRGB color) {
    fill_solid(leds, NUM_LEDS, color);
}