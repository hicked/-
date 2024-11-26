#include <FastLED.h>
#include "brake.h"

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

int encoderPosition = 0; // Tracks the encoder position
bool prevA = 1;
bool prevB = 1;

CRGB leds[NUM_LEDS];
Brake brake(leds, NUM_LEDS);

void setup() {
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  FastLED.show();

  // Initialize Serial for debugging
  Serial.begin(115200);
  Serial.println("Starting...");

  // Setup encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);
}

void loop() {
  // Update counter based on encoder state
  encoderPosition += CheckEncoderRotation();

  // Prevent negative encoder values
  if (encoderPosition < 0) {
    encoderPosition = 0;
  }
  else if (encoderPosition > 100) {
    encoderPosition = 100; // Limit to max value of 100
  }

  // Handle braking events
  if (encoderPosition < MIN_GYRO_BRAKE_THRESHHOLD) {
    brake.initializedBraking = false;
  }

  else if (encoderPosition > MIN_GYRO_BRAKE_THRESHHOLD && !brake.initializedBraking) {
    brake.initializedBraking = true;
    brake.flashCount = brake.initializedEmergencyBraking;  // Flash 5 times when braking starts
  }
  else if (encoderPosition < brake.emergencyBrakingThreshold && brake.initializedEmergencyBraking) {
    brake.initializedEmergencyBraking = false;
  }
  // Handle emergency braking event when encoder reaches 100
  else if (!brake.initializedEmergencyBraking && encoderPosition > brake.emergencyBrakingThreshold) {
    brake.initializedEmergencyBraking = true;
    brake.flashCount = brake.emergencyBrakingNumFlashes;  // Flash 10 times for emergency braking
  }

  // Set the number of LEDs and brightness based on encoder position
  brake.num_brake_leds = map(encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS/2);
  brake.active_brightness = map(encoderPosition, MIN_GYRO, MAX_GYRO, brake.minBrakeBrightness, brake.maxBrakeBrightness);
  
  // Handle flashing if braking is initiated
  brake.Update();
  
  // Debug output
  Serial.print("encoderPosition: ");
  Serial.print(encoderPosition);
  Serial.print(" | Active Brightness: ");
  Serial.print(brake.active_brightness);
  Serial.print(" | Number of LEDs On: ");
  Serial.println(brake.num_brake_leds * 2);
}


int CheckEncoderRotation() {
  int delta = 0;
  bool A = digitalRead(ENCODER_PIN_A);
  bool B = digitalRead(ENCODER_PIN_B);

  // Update the counter based on the state changes
  if (B != prevB) {
    delta = (B - prevB) * (A ? +1 : -1);
  } else if (A != prevA) {
    delta = (A - prevA) * (B ? -1 : +1);
  }

  // Update previous states
  prevA = A;
  prevB = B;

  return delta;
}
