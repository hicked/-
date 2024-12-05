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
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we dont care about rotational

    if (Wire.available() == 6) {
        Serial.println("Connected to Gyro");
    } else {
        Serial.println("Failed to read from MPU");
    }
}

void Gyro::Update() {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we dont care about rotational

    if (Wire.available() == 6) {
        this->measuredAccX = Wire.read() << 8 | Wire.read();    
        this->measuredAccY = Wire.read() << 8 | Wire.read();  
        this->measuredAccZ = Wire.read() << 8 | Wire.read();
    } else {
        Serial.println("Failed to read from MPU");
        return;
    }

    // The logic here, is that any under circumstance (leaning, uphill, downhill, etc) total magnitude of acceleration should be 1g or 16384
    // So we can just subtract 16384 from the total magnitude of acceleration to get the corrected acceleration based on only the motorcycle's acceleration
    // There are still some issues like the momentary acceleration caused by bumps, but thats what the smoothing is for
    this->correctedAcc = sqrt(this->measuredAccX*this->measuredAccX + this->measuredAccY*this->measuredAccY + this->measuredAccZ*this->measuredAccZ) - EXPECTED_ACC_MAGNITUDE;
    
    if (this->measuredAccY < 0) {
        this->correctedAcc *= -1;
        this->accelerating = false;
    }
    else {
        this->accelerating = true;
    }
    
    this->smoothedAcc = this->smoothedAcc * (1 - SMOOTHING_FACTOR) + this->correctedAcc * SMOOTHING_FACTOR;
}