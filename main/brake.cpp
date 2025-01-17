#include "Brake.h"

Brake::Brake(Signals *signal, CRGB *leds, Gyro *gyro, Button *button, int numLEDs) {
    this->LEDStrip = leds;
    this->numLEDs = numLEDs;
    this->signals = signal;
    this->gyro = gyro;
    this->button = button;
    this->middleIndex = this->numLEDs / 2;

    fill_solid(this->LEDStrip, this->numLEDs, CRGB::Black);
}

void Brake::update() {
    unsigned long currentTime = millis();
    this->brakeWireInput = !digitalRead(BRAKE_PIN);

    // Emergency braking detected
    if (gyro->smoothedAcc < -MAX_GYRO_BREAKING && flashCount == 0) {
        flashCount++; // continuously adds one or flashes forever
    }

    // Initialization of braking detected
    else if (gyro->smoothedAcc < -MIN_GYRO_BREAKING && gyro->prevAcc >= -MIN_GYRO_BREAKING && millis() - this->timeSinceLastIniBraking > TIME_BETWEEN_INI_BRAKE) {
        timeSinceLastIniBraking = millis();
        flashCount = INITIALIZE_BRAKING_FLASH_LENGTH;
    }

    switch (this->button->mode) {
        case BRAKE_MODE_MARIO_STAR:
            this->marioStarMode();
            break;
        case BRAKE_MODE_CHRISTMAS:
            this->christmasMode();
            break;
        case BRAKE_MODE_HALLOWEEN:
            this->halloweenMode();
            break;
        case BRAKE_MODE_FLASHLIGHT:
            this->flashlightMode();
            break;
        case BRAKE_MODE_STATIC:
            this->staticBrakeMode();
            break;
        default:
            // Default to dynamic brake mode
            this->dynamicBrakeMode();
            break;
    }
}


void Brake::dynamicBrakeMode() {
    this->setSolid(BACKLIGHT_COLOUR); // Reset to background
    if (gyro->smoothedAcc < -MIN_GYRO_BREAKING) { // breaking
        // Flash the LEDs in the center
        this->flashCenterLEDs();

        // If signals on, use static braking (not progressive)
        if (signals->left || signals->right) {
            for (int i = 0; i < numLEDs/2 - CENTER_FLASH_WIDTH/2 - SIGNAL_LENGTH; i++) {
                int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
                int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1; 

                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }         
        } 
        // Otherwise, use progressive brake lighting
        else {
            for (int i = 0; i < this->numActiveLEDs; i++) {
                int index1 = middleIndex + CENTER_FLASH_WIDTH/2 + i;
                int index2 = middleIndex - CENTER_FLASH_WIDTH/2 - i - 1; 

                this->LEDStrip[index1] = CRGB(this->active_brightness, 0, 0); // red brake color
                this->LEDStrip[index2] = CRGB(this->active_brightness, 0, 0);
            }
        }
    } 
    
    else if (gyro->smoothedAcc > MIN_GYRO_ACCELERATING && SHOW_ACCEL) { // Accelerating
        for (int i = 0; i < this->numActiveLEDs; i++) {
            int index1 = middleIndex + i;
            int index2 = middleIndex - i - 1;
            
            this->LEDStrip[index1] = CRGB(0, this->active_brightness, 0); // Green brake color
            this->LEDStrip[index2] = CRGB(0, this->active_brightness, 0);
        }
    }
}

void Brake::staticBrakeMode(){
    this->setSolid(BACKLIGHT_COLOUR); // Reset to background
    if (gyro->smoothedAcc < -MIN_GYRO_BREAKING || this->brakeWireInput) {
        if (gyro->smoothedAcc < -MIN_GYRO_BREAKING) {
            this->setSolid(CRGB(this->active_brightness, 0, 0)); // Red brake color
            this->flashCenterLEDs();
        }
        else if (gyro->smoothedAcc > MIN_GYRO_ACCELERATING && SHOW_ACCEL) {
            this->setSolid(CRGB(0, this->active_brightness, 0)); // Green brake color
        }
    } 
}

void Brake::flashCenterLEDs() { // Flashing of the center LEDs
    unsigned long currentTime = millis();
    // If the time since the last flash is greater than the delay betweem flashes
    if (currentTime - this->lastCenterFlashTime >= CENTER_FLASH_SPEED) {
        this->lastCenterFlashTime = currentTime;
        this->centerFlashON = !this->centerFlashON;
    }

    for(int i = 0; i < CENTER_FLASH_WIDTH/2; i++) {
        int index1 = middleIndex + i;
        int index2 = middleIndex - i - 1;

        if (this->centerFlashON) {
            this->LEDStrip[index1] = CENTER_FLASH_COLOUR; // Flash red
            this->LEDStrip[index2] = CENTER_FLASH_COLOUR;
        }
    }
}

void Brake::flashRedLEDs() { // Flashing of the entire LED strip
    unsigned long currentTime = millis();

    if (currentTime - this->lastFlashTime >= FLASH_SPEED) {
        this->lastFlashTime = currentTime;
        this->flashON = !this->flashON;

        if (this->flashON) {
            this->setSolid(FLASH_COLOUR); // Flash red
            this->flashCount--;
        } else {
            this->setSolid(BACKLIGHT_COLOUR); // Reset to background
        }
    }
}

void Brake::setSolid(CRGB colour) {
    for (int i=0; i<this->numLEDs; i++) {
        this->LEDStrip[i] = colour;
    }
}

void Brake::shiftPatternMode(CRGB colors[], int numColors, unsigned long speed, int size) {
    unsigned long currentTime = millis();

    // Check if enough time has passed to update the pattern
    if (currentTime - this->lastPatternTime >= speed) {
        this->lastPatternTime = currentTime;

        static int shift = 0; // Tracks the current shift of the pattern

        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the effective position within the pattern
            int effectiveIndex = (i + shift) % (numColors * size);

            // Determine the color for the current LED based on the effective position
            int segment = effectiveIndex / size; // Which segment (color) this LED belongs to

            // Set the color for the current LED
            this->LEDStrip[i] = colors[segment];
        }

        // Increment the shift, ensuring it wraps around properly after one full cycle
        shift = (shift + 1) % (numColors * size);
    }
}


void Brake::marqueeEffect(CRGB* colors, int numColors, int speed, float blend) {
    unsigned long currentTime = millis();

    // Check if enough time has passed to update the marquee
    if (currentTime - this->lastMarqueeTime >= speed) {
        this->lastMarqueeTime = currentTime;

        static float shift = 0; // Tracks the current shift of the marquee as a floating-point number

        for (int i = 0; i < this->numLEDs; i++) {
            // Calculate the position within the virtual pattern
            float effectiveIndex = (i + shift) / this->numLEDs * numColors;

            // Determine the two colors to blend between
            int color1Index = static_cast<int>(effectiveIndex) % numColors;
            int color2Index = (color1Index + 1) % numColors;

            // Calculate the blend factor
            float blendFactor = effectiveIndex - static_cast<int>(effectiveIndex);

            // Blend the two colors
            this->LEDStrip[i] = blendColors(colors[color1Index], colors[color2Index], blendFactor);
        }

        // Increment the shift for the marquee effect
        shift += (1.0f / this->numLEDs); // Smoothly shifts by a fraction of the LED count
        if (shift >= this->numLEDs) {
            shift -= this->numLEDs; // Wrap around to prevent overflow
        }
    }
}

void Brake::marioStarMode() {
    CRGB colors[] = {CRGB::Yellow, CRGB::Red, CRGB::Blue, CRGB::Green}; // List of colors
    marqueeEffect(colors, 4, 50, 10); // Speed of 50ms and brightness of 200()
}

void Brake::christmasMode() {
    CRGB colors[] = { CRGB::Blue, CRGB::Red, CRGB::Green };
    shiftPatternMode(colors, 3, 100, 10); // 100ms update, 5 LEDs per segment
}


// Halloween mode: Alternating orange and purple
void Brake::halloweenMode() {
    CRGB colors[] = { CRGB::Purple, CRGB::Orange, CRGB::Black };
    shiftPatternMode(colors, 3, 100, 10); // 100ms update, 5 LEDs per segment
}


void Brake::flashlightMode() {
    fill_solid(this->LEDStrip, this->numLEDs, FLASHLIGHT_COLOUR);
}

CRGB Brake::blendColors(CRGB color1, CRGB color2, float factor) {
    return CRGB(
        static_cast<uint8_t>(color1.r + factor * (color2.r - color1.r)),
        static_cast<uint8_t>(color1.g + factor * (color2.g - color1.g)),
        static_cast<uint8_t>(color1.b + factor * (color2.b - color1.b))
    );
}

