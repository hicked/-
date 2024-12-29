#include <FastLED.h>
#include "brake.h"
#include "signals.h"
#include "gyro.h"

#define MOSFET_PIN 6
#define LED_DATA_PIN_1 4
#define LED_DATA_PIN_2 5

#define NUM_LEDS 66
#define LED_TYPE WS2815

CRGB leds[NUM_LEDS];

Gyro *gyro;
Brake *brake;
Signals signals(leds, NUM_LEDS);

void setup() {
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN_1, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN_2, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(GLOBAL_BRIGHTNESS);

    // Initialize Serial for debugging
    Serial.begin(115200);
    Serial.println("Serial initialized");

    // Setup pins
    pinMode(LEFT_SIGNAL_PIN, INPUT_PULLUP);
    pinMode(RIGHT_SIGNAL_PIN, INPUT_PULLUP);
    pinMode(MOSFET_PIN, OUTPUT);

    // Initialize objects
    gyro = new Gyro();
    brake = new Brake(&signals, leds, gyro, NUM_LEDS);
    Serial.println("Setup complete");
}


void loop() {
    gyro->Update(); // Updates the value of smoothedAcc

    // Update the number of active LEDs and brightness of the brake lights
    if (gyro->smoothedAcc > 0) { // accelerating
        brake->numActiveLEDs = constrain(map(gyro->smoothedAcc, MIN_GYRO_ACCELERATING, MAX_GYRO_ACCELERATING, 0, NUM_LEDS/2), 0, NUM_LEDS/2);
        brake->active_brightness = constrain(map(gyro->smoothedAcc, MIN_GYRO_ACCELERATING, MAX_GYRO_ACCELERATING, MIN_BRAKE_BRIGHTNESS, MAX_BRAKE_BRIGHTNESS), 0, MAX_BRAKE_BRIGHTNESS);
    } else { // breaking
        brake->numActiveLEDs = constrain(map(abs(gyro->smoothedAcc), MIN_GYRO_BREAKING, MAX_GYRO_BREAKING, 0,  (NUM_LEDS - CENTER_FLASH_WIDTH)/2), 0, (NUM_LEDS - CENTER_FLASH_WIDTH)/2);
        brake->active_brightness = constrain(map(-(gyro->smoothedAcc), MIN_GYRO_BREAKING, MAX_GYRO_BREAKING, MIN_BRAKE_BRIGHTNESS, MAX_BRAKE_BRIGHTNESS), 0, MAX_BRAKE_BRIGHTNESS);
    }

    if (SHOW_MARIO && gyro->smoothedAcc > MAX_GYRO_ACCELERATING) {
        brake->MarioStar();
        signals.Update(); // signals on top of mario star
    }
    else if (brake->flashCount == 0) {
        brake->Update();
        signals.Update(); // signals on top of brake lighting
    }  
    else { // handled outside brake.update() since we want it to go OVER the turn signals, while normal brake lights go UNDER signals
        brake->FlashRedLEDs();
    }
    
    FastLED.show();
}