// #include <FastLED.h>
// #include "brake.h"
// #include "signals.h"

// #define LED_DATA_PIN 7
// #define NUM_LEDS 66
// #define LED_TYPE WS2812B
// #define GLOBAL_BRIGHTNESS 255

// // Brightness configuration
// #define MIN_GYRO 0
// #define MAX_GYRO 100

// // Encoder configuration
// #define ENCODER_PIN_A   3
// #define ENCODER_PIN_B   2
// #define ENCODER_CLICK_PIN 8

// int encoderPosition = 0; // Tracks the encoder position
// bool prevA = 1;
// bool prevB = 1;
// bool signal = false;
// bool prevSignal = false;

// CRGB leds[NUM_LEDS];
// Brake brake(leds, NUM_LEDS);
// Signals signals(leds, NUM_LEDS);

// void setup() {
//     FastLED.addLeds<LED_TYPE, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
//     FastLED.clear();
//     FastLED.setBrightness(GLOBAL_BRIGHTNESS);
//     FastLED.show();

//     // Initialize Serial for debugging
//     Serial.begin(115200);
//     Serial.println("Starting...");

//     // Setup encoder pins
//     pinMode(ENCODER_PIN_A, INPUT_PULLUP);
//     pinMode(ENCODER_PIN_B, INPUT_PULLUP);
//     pinMode(ENCODER_CLICK_PIN, INPUT_PULLUP);

//     // Attach interrupts to encoder pins
//     attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderA, CHANGE);
//     attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoderB, CHANGE);
// }

// void loop() {
//     // Prevent encoder values from going beyond the limits
//     if (encoderPosition < -100) {
//         encoderPosition = -100;
//     } else if (encoderPosition > 100) {
//         encoderPosition = 100; // Limit to max value of 100
//     }

//     // Determine if accelerating based on encoder position
//     if (encoderPosition < 0) {
//         brake.accelerating = true;
//         brake.numActiveLEDs = map(-encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS / 2);
//         brake.active_brightness = map(-encoderPosition, MIN_GYRO, MAX_GYRO, brake.minBrakeBrightness, brake.maxBrakeBrightness);
//     } else {
//         brake.accelerating = false;
//         brake.numActiveLEDs = map(encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS / 2);
//         brake.active_brightness = map(encoderPosition, MIN_GYRO, MAX_GYRO, brake.minBrakeBrightness, brake.maxBrakeBrightness);
//     }

//     // Handle flashing if braking is initiated
//     brake.Update();
//     signals.Update();

//     // Debug output
//     // Serial.print("encoderPosition: ");
//     // Serial.print(encoderPosition);
//     // Serial.print(" | Active Brightness: ");
//     // Serial.print(brake.active_brightness);
//     // Serial.print(" | Number of LEDs On: ");
//     // Serial.println(brake.numActiveLEDs * 2);
//     Serial.println();
// }

// // Interrupt Service Routine for ENCODER_PIN_A
// void handleEncoderA() {
//     updateEncoder();
// }

// // Interrupt Service Routine for ENCODER_PIN_B
// void handleEncoderB() {
//     updateEncoder();
// }

// // Function to update encoder position
// void updateEncoder() {
//     signal = digitalRead(ENCODER_CLICK_PIN) == LOW;
//     if (prevSignal == false && signal == true) {
//         Serial.print("trying");
//         signals.left = !signals.left;
//     } 
//     prevSignal = signal;
    
//     bool A = digitalRead(ENCODER_PIN_A);
//     bool B = digitalRead(ENCODER_PIN_B);

//     if (B != prevB) {
//         encoderPosition += (B - prevB) * (A ? +1 : -1);
//     } else if (A != prevA) {
//         encoderPosition += (A - prevA) * (B ? -1 : +1);
//     }

//     prevA = A;
//     prevB = B;
// }
#include <FastLED.h>
#include "brake.h"
#include "signals.h"

#define LED_DATA_PIN 7
#define NUM_LEDS 66
#define LED_TYPE WS2812B
#define GLOBAL_BRIGHTNESS 255

// Brightness configuration
#define MIN_GYRO 0
#define MAX_GYRO 100

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
Brake brake(leds, NUM_LEDS);
Signals signals(&brake, leds, NUM_LEDS);

void setup() {
    FastLED.addLeds<LED_TYPE, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.clear();
    FastLED.setBrightness(GLOBAL_BRIGHTNESS);
    FastLED.show();

    // Initialize Serial for debugging
    Serial.begin(115200);
    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Starting...");

    // Setup encoder pins
    pinMode(ENCODER_PIN_A, INPUT_PULLUP);
    pinMode(ENCODER_PIN_B, INPUT_PULLUP);
    pinMode(ENCODER_CLICK_PIN, INPUT_PULLUP);

    // Attach interrupts to encoder pins
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), handleEncoderA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), handleEncoderB, CHANGE);

    Serial.println("Setup complete");
}

void loop() {
    // Prevent encoder values from going beyond the limits
    if (encoderPosition < -100) {
        encoderPosition = -100;
    } else if (encoderPosition > 100) {
        encoderPosition = 100; // Limit to max value of 100
    }

    // Determine if accelerating based on encoder position
    if (encoderPosition < 0) {
        brake.accelerating = true;
        brake.numActiveLEDs = map(-encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS / 2);
        brake.active_brightness = map(-encoderPosition, MIN_GYRO, MAX_GYRO, brake.minBrakeBrightness, brake.maxBrakeBrightness);
    } else {
        brake.accelerating = false;
        brake.numActiveLEDs = map(encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS / 2);
        brake.active_brightness = map(encoderPosition, MIN_GYRO, MAX_GYRO, brake.minBrakeBrightness, brake.maxBrakeBrightness);
    }

    // Debounce the encoder click
    bool currentClickState = digitalRead(ENCODER_CLICK_PIN);

    if (currentClickState == LOW && prevClickState == HIGH) {
        signals.left = !signals.left;
    }
    
    prevClickState = currentClickState;

    // Handle flashing if braking is initiated
    brake.Update();
    signals.Update();

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