#include "gyro.h"
#include <Arduino.h>
#include <math.h>

// SDA = A4
// SCL = A5

Gyro::Gyro() {
    Wire.begin();
    Wire.beginTransmission(MPU);
    Wire.write(0x6B);  
    Wire.write(0);
    Wire.endTransmission(true); 

    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we don't care about rotational

    if (Wire.available() == 6) {
        Serial.println("Connected to Gyro");
    } else {
        Serial.println("Failed to read from MPU");
    }

    this->lastUpdateTime = millis(); // Initialize the last update time
}

void Gyro::Update() {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we don't care about rotational

    if (Wire.available() == 6) {
        this->measuredAccX = Wire.read() << 8 | Wire.read();    
        this->measuredAccY = Wire.read() << 8 | Wire.read();  
        this->measuredAccZ = Wire.read() << 8 | Wire.read();
    } else {
        Serial.println("Failed to read from MPU");
        return;
    }

    // Calculate the corrected acceleration
    this->correctedAcc = sqrt(this->measuredAccX * this->measuredAccX +
                              this->measuredAccY * this->measuredAccY +
                              this->measuredAccZ * this->measuredAccZ) - EXPECTED_ACC_MAGNITUDE;

    this->correctedAcc = this->correctedAcc < 0 ? this->correctedAcc * -1 : this->correctedAcc;

    // Check for sudden bumps
    if (abs(this->correctedAcc - this->prevCorrectedAcc) > BUMP_THRESHOLD) {
        unsigned long currentTime = millis();

        // Check if the update timeout has elapsed
        if (currentTime - this->lastUpdateTime > UPDATE_TIMEOUT) {
            Serial.println("Acceleration update overdue; applying fallback."); // forcing changes
            // Force an update with the current corrected value
            this->smoothedAcc = this->smoothedAcc * (1 - SMOOTHING_FACTOR) + this->correctedAcc * SMOOTHING_FACTOR;
            this->lastUpdateTime = currentTime; // Reset the last update time
        } else {
            // Skip the update since its a bump
            
            return;
        }
    }

    this->accelerating = this->measuredAccY >= 0;

    // Smooth the acceleration
    this->smoothedAcc = this->smoothedAcc * (1 - SMOOTHING_FACTOR) + this->correctedAcc * SMOOTHING_FACTOR;

    // Update previous corrected acceleration and timestamp
    this->prevCorrectedAcc = this->correctedAcc;
    lastUpdateTime = millis();
}
