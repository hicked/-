#include <FastLED.h>
#include "brake.h"
#include "signals.h"
#include "gyro.h"

#define MOSFET_PIN 6
#define LED_DATA_PIN_1 4
#define LED_DATA_PIN_2 5
#define NUM_LEDS 66
#define LED_TYPE WS2815
#define GLOBAL_BRIGHTNESS 255

// Encoder configuration
#define ENCODER_PIN_A   3
#define ENCODER_PIN_B   2
#define ENCODER_CLICK_PIN 8

int encoderPosition = 0; // Tracks the encoder position
bool prevA = 1;
bool prevB = 1;
bool prevClickState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

CRGB leds[NUM_LEDS];

Gyro *gyro;
Brake *brake;
Signals signals(leds, NUM_LEDS);

void setup() {
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN_1, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN_2, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(GLOBAL_BRIGHTNESS);
    FastLED.show();

    // Initialize Serial for debugging
    Serial.begin(115200);
    Serial.println("Serial initialized");

    // Setup encoder pins
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
    pinMode(ENCODER_CLICK_PIN, INPUT_PULLUP);

    // Attach interrupts to encoder pins
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoderB, CHANGE);

    pinMode(MOSFET_PIN, OUTPUT);
    delay(10);
    digitalWrite(MOSFET_PIN, HIGH);

    gyro = new Gyro();
    brake = new Brake(&signals, leds, gyro, NUM_LEDS);
    Serial.println("Setup complete");
}


void loop() {
    gyro->Update();

    if (gyro->smoothedAcc > 0) { // accelerating
        brake->numActiveLEDs = map(gyro->smoothedAcc, MIN_GYRO_ACCELERATING, MAX_GYRO_ACCELERATING, 0, NUM_LEDS / 2);
        brake->active_brightness = map(gyro->smoothedAcc, MIN_GYRO_ACCELERATING, MAX_GYRO_ACCELERATING, MIN_BRAKE_BRIGHTNESS, MAX_BRAKE_BRIGHTNESS);
    } else { // breaking
        brake->numActiveLEDs = map(abs(gyro->smoothedAcc), MIN_GYRO_BREAKING, MAX_GYRO_BREAKING, 0, NUM_LEDS / 2);
        brake->active_brightness = map(-(gyro->smoothedAcc), MIN_GYRO_BREAKING, MAX_GYRO_BREAKING, MIN_BRAKE_BRIGHTNESS, MAX_BRAKE_BRIGHTNESS);
    }

    // Debounce the encoder click
    bool currentClickState = digitalRead(ENCODER_CLICK_PIN); // signals, to be implemented later this is for testing

    if (currentClickState == LOW && prevClickState == HIGH) {
        signals.left = !signals.left;
    }
    prevClickState = currentClickState;
    
    brake->Update();
    signals.Update();
    if (SHOW_MARIO && gyro->smoothedAcc > 0 && brake->numActiveLEDs > MARIO_STAR_THRESHOLD) {
        brake->MarioStar();
    }
    else if (brake->flashCount == 0) {
        brake->UpdateBrakeLEDs();
        signals.UpdateSignals();
    }  
    else { // handled outside brake.update() since we want it to go OVER the turn signals
        brake->FlashRedLEDs();
    }
    FastLED.show();

    // Debug output
    // Serial.print("encoderPosition: ");
    // Serial.print(encoderPosition);
    // Serial.print(" | Active Brightness: ");
    // Serial.print(brake.active_brightness);
    // Serial.print(" | Number of LEDs On: ");
    // Serial.println(brake.numActiveLEDs * 2);

    // Serial.println("Loop end");
}

// Interrupt Service Routine for ENCODER_PIN_A
void handleEncoderA() {
    updateEncoder();
}

// Interrupt Service Routine for ENCODER_PIN_B
void handleEncoderB() {
    updateEncoder();
}
// Function to update encoder position
void updateEncoder() {
    bool A = digitalRead(ENCODER_PIN_A);
    bool B = digitalRead(ENCODER_PIN_B);

    if (B != prevB) {
        encoderPosition += (B - prevB) * (A ? +1 : -1);
    } else if (A != prevA) {
        encoderPosition += (A - prevA) * (B ? -1 : +1);
    }

    prevA = A;
    prevB = B;
}