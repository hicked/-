#include <FastLED.h>

// LED strip configuration
#define LED_PIN     7
#define NUM_LEDS    66
#define LED_TYPE WS2812B

#define BACKLIGHT_COLOR CRGB(0, 1, 0) // Dim red for the background

// Brightness configuration
#define GLOBAL_BRIGHTNESS 255
#define MIN_BRAKE_BRIGHTNESS  50
#define MAX_BRAKE_BRIGHTNESS  150

#define FLASH_SPEED 50
#define EMERGENCY_BRAKING_FLASH_LENGTH 25
#define INITIALIZE_BRAKING_FLASH_LENGTH 7

#define MIN_GYRO 0
#define MAX_GYRO 100
#define EMERGENCY_BRAKING_THRESHOLD 95
#define MIN_GYRO_BRAKE_THRESHHOLD 5

// Encoder configuration
#define ENCODER_PIN_A   3
#define ENCODER_PIN_B   2

int encoderPosition = 0; // Tracks the encoder position
bool prevA = 1;
bool prevB = 1;

unsigned long lastFlashTime = 0;
bool flashON = false;
bool initializedBraking = false; // Flag to track the first braking event
bool initializedEmergencyBraking = false;
int flashCount = 0; // Counter to track how many flashes need to occur

CRGB leds[NUM_LEDS];

void setup() {
  // Initialize LED strip
  FastLED.addLeds<LED_TYPE, LED_PIN, RGB>(leds, NUM_LEDS);
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
    initializedBraking = false;
  }

  else if (encoderPosition > MIN_GYRO_BRAKE_THRESHHOLD && !initializedBraking) {
    initializedBraking = true;
    flashCount = INITIALIZE_BRAKING_FLASH_LENGTH;  // Flash 5 times when braking starts
  }
  else if (encoderPosition < EMERGENCY_BRAKING_THRESHOLD && initializedEmergencyBraking) {
    initializedEmergencyBraking = false;
  }
  // Handle emergency braking event when encoder reaches 100
  else if (!initializedEmergencyBraking && encoderPosition > EMERGENCY_BRAKING_THRESHOLD) {
    initializedEmergencyBraking = true;
    flashCount = EMERGENCY_BRAKING_FLASH_LENGTH;  // Flash 10 times for emergency braking
  }
  

  fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);

  // Set the number of LEDs and brightness based on encoder position
  int num_braking_leds = map(encoderPosition, MIN_GYRO, MAX_GYRO, 0, NUM_LEDS/2);
  int active_brake_brightness = map(encoderPosition, MIN_GYRO, MAX_GYRO, MIN_BRAKE_BRIGHTNESS, MAX_BRAKE_BRIGHTNESS);
  
  // Handle flashing if braking is initiated
  if (flashCount > 0) {
    FlashRedLEDs(); // Flashing logic
  }
  else {
    for (int i = 0; i < num_braking_leds; i++) {
      leds[NUM_LEDS/2 + i] = CRGB(0, active_brake_brightness, 0); 
      leds[NUM_LEDS/2 - i - 1] = CRGB(0, active_brake_brightness, 0); 
    }
  }

  FastLED.show();
  
  // Debug output
  Serial.print("encoderPosition: ");
  Serial.print(encoderPosition);
  Serial.print(" | Active Brightness: ");
  Serial.print(active_brake_brightness);
  Serial.print(" | Number of LEDs On: ");
  Serial.println(num_braking_leds * 2);
}

void FlashRedLEDs() {
  unsigned long currentMillis = millis();

  // Check if it's time to toggle the flashing state
  if (currentMillis - lastFlashTime >= FLASH_SPEED) {
    lastFlashTime = currentMillis;
    flashON = !flashON; // Toggle the flash state
    if (flashON) {
      fill_solid(leds, NUM_LEDS, CRGB(0, MAX_BRAKE_BRIGHTNESS, 0));  // Flash red at MAX_BRIGHTNESS
    } else {
      fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);  // Reset to background color
    }
    flashCount--;  // Decrease the flash count once per toggle
  }

  // If flashCount is 0, stop flashing and reset the state
  if (flashCount <= 0) {
    flashCount = 0;  // Ensure it doesn't go negative
    fill_solid(leds, NUM_LEDS, BACKLIGHT_COLOR);  // Reset LEDs to background color
    flashON = false;  // Reset flashON to ensure it starts fresh if flashCount is reset again
  }
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
