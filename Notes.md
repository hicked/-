- 12V LED strips WS2815 
- Arduino Nano (controls the system logic)
- MOSFET (IRLZ44N) controlled by nano (to switch the ground side of the LED strips)
- 12V motorcycle battery (power source)
- DC-DC Buck LM2596 Voltage regulator (either onboard or external for stepping down 12V to 5V for the Arduino)
- Manual switch from battery -> voltage regulator -> nano to turn it off manually if needed



https://www.aliexpress.com/item/32401358878.html?spm=a2g0o.productlist.main.1.407aDvdYDvdYH8&algo_pvid=610a9ae5-1849-4d93-974d-e91b7275c591&algo_exp_id=610a9ae5-1849-4d93-974d-e91b7275c591-0&pdp_npi=4%40dis%21CAD%213.70%211.41%21%21%212.61%211.00%21%402101ec1f17310481281222967ecfc8%2154122577111%21sea%21CA%212752790534%21ABX&curPageLogUid=8wycufZBX8Q9&utparam-url=scene%3Asearch%7Cquery_from%3A
https://www.aliexpress.com/item/1005005278857364.html?spm=a2g0o.productlist.main.25.417c23f8EWB9LY&algo_pvid=647a59cd-5d05-4a24-a06e-03bb535ee857&algo_exp_id=647a59cd-5d05-4a24-a06e-03bb535ee857-42&pdp_npi=4%40dis%21CAD%2113.11%216.52%21%21%2166.10%2132.86%21%402101efeb17310415636764666e140a%2112000034526921699%21sea%21CA%210%21ABX&curPageLogUid=nmxusgjmBu4I&utparam-url=scene%3Asearch%7Cquery_from%3A
https://www.aliexpress.com/item/1005007066680464.html?spm=a2g0o.productlist.main.1.44c36f37DeGJAg&algo_pvid=00683b3e-1d59-419c-9e03-1a662ee84996&algo_exp_id=00683b3e-1d59-419c-9e03-1a662ee84996-0&pdp_npi=4%40dis%21CAD%213.99%211.41%21%21%212.81%210.99%21%402101c72a17310418272837817ec396%2112000039294978565%21sea%21CA%210%21ABX&curPageLogUid=X5533ijurx4u&utparam-url=scene%3Asearch%7Cquery_from%3A
https://www.aliexpress.com/item/32714396199.html?spm=a2g0o.productlist.main.1.58b34165LAhKRI&algo_pvid=7733b2ee-3fd3-47f9-84a3-a4a8180c54a4&algo_exp_id=7733b2ee-3fd3-47f9-84a3-a4a8180c54a4-0&pdp_npi=4%40dis%21CAD%213.95%211.41%21%21%212.78%210.99%21%402103244417310419633348309ebce6%2112000026980721547%21sea%21CA%210%21ABX&curPageLogUid=ZfqwHwLFidYV&utparam-url=scene%3Asearch%7Cquery_from%3A
https://www.aliexpress.com/item/1005004904872120.html?spm=a2g0o.productlist.main.1.1f393fc3yZRkPm&algo_pvid=e56115e8-5cfe-4a61-91d6-de21ae4a69df&algo_exp_id=e56115e8-5cfe-4a61-91d6-de21ae4a69df-0&pdp_npi=4%40dis%21CAD%215.14%211.41%21%21%213.62%210.99%21%402103146c17310420363692422eb8a0%2112000030968453858%21sea%21CA%210%21ABX&curPageLogUid=xDQTxVD9rYZy&utparam-url=scene%3Asearch%7Cquery_from%3A


```c
#include <Adafruit_NeoPixel.h>

// Pin definitions
const int ledPin = 6;            // Pin for NeoPixel LED strip
const int gyroPin = A0;          // Pin for accelerometer or gyro (analog input for braking force)

// NeoPixel setup (adjust the number of LEDs)
const int numPixels = 144;       // Number of LEDs in the strip (adjust for your setup)
Adafruit_NeoPixel strip(numPixels, ledPin, NEO_GRB + NEO_KHZ800);

// Timing variables
unsigned long previousMillis = 0;  
unsigned long flashInterval = 100; // Interval between flash toggles (in milliseconds)
bool isFlashing = false;          // State variable for flashing effect

// Gyro and brake force variables
int gyroValue = 0;                // The current value from the gyro/accelerometer
int maxGyroValue = 1023;          // Maximum gyro value (this could be adjusted for your sensor's range)
int gyroThreshold = 100;          // Threshold for braking detection, adjust based on the sensor's behavior

// Function to set the brake light intensity (red for braking, green for acceleration)
void setLightIntensity(int force, bool isBraking) {
  int numLitPixels = map(abs(force), 0, maxGyroValue, 0, numPixels);
  
  // Gradually light up the LEDs based on the force (deceleration or acceleration)
  for (int i = 0; i < numPixels; i++) {
    if (i < numLitPixels) {
      if (isBraking) {
        // Braking (negative acceleration) = Red color
        strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red color
      } else {
        // Acceleration (positive acceleration) = Green color
        strip.setPixelColor(i, strip.Color(0, 255, 0));  // Green color
      }
    } else {
      // Set remaining LEDs to off
      strip.setPixelColor(i, strip.Color(0, 0, 0));    // Off
    }
  }
  strip.show(); // Update the strip
}

void setup() {
  // Initialize NeoPixel strip
  strip.begin();
  strip.show(); // Initialize all pixels to off
  
  // Set up input pins
  pinMode(gyroPin, INPUT);
}

void loop() {
  // Read gyro value (simulate with analogRead)
  gyroValue = analogRead(gyroPin);  // Replace with actual gyro input for real-world use

  // Check if the gyro value exceeds the threshold for braking or acceleration
  if (abs(gyroValue) > gyroThreshold) {  // We check absolute value to detect both braking and acceleration
    if (!isFlashing) {
      startFlashing(); // Start flashing the LED strip
    }

    // Determine if it's braking or acceleration
    bool isBraking = gyroValue < 0; // Negative values indicate braking, positive values indicate acceleration

    // Adjust the lighting based on braking or acceleration
    setLightIntensity(gyroValue, isBraking);
  } else {
    if (isFlashing) {
      stopFlashing();  // Stop flashing if gyro value falls below threshold
    }

    // If gyro value is below threshold, keep the LEDs off (idle state)
    for (int i = 0; i < numPixels; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));  // Off
    }
    strip.show();
  }

  // Allow the system to process other tasks (non-blocking code)
  yield();
}

// Function to start flashing all LEDs
void startFlashing() {
  isFlashing = true;
  previousMillis = millis(); // Reset the flashing timer
}

// Function to stop flashing all LEDs
void stopFlashing() {
  isFlashing = false;
  // Once flashing is over, reset the LED strip
  for (int i = 0; i < numPixels; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));  // Off
  }
  strip.show();
}

// Function to handle the flashing effect without blocking
void flashStrip() {
  // Track the time elapsed since the last toggle
  unsigned long currentMillis = millis();
  
  // Toggle the LED state every flashInterval
  if (currentMillis - previousMillis >= flashInterval) {
    previousMillis = currentMillis; // Reset the timer
    
    // Toggle the state of the LEDs (on/off)
    static bool ledState = false;  // Track the current LED state (on or off)

    if (ledState) {
      // Turn on all LEDs (flash)
      for (int i = 0; i < numPixels; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));  // Red color for braking
      }
    } else {
      // Turn off all LEDs (flash off)
      for (int i = 0; i < numPixels; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));    // Off
      }
    }

    strip.show(); // Update the strip
    ledState = !ledState; // Toggle the LED state for the next cycle
  }
}
```