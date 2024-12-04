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
    this->correctedAcc = this->correctAcc(this->measuredAccX, this->measuredAccY, this->measuredAccZ);
    this->smoothedAcc = this->smoothedAcc * (1 - SMOOTHING_FACTOR) + this->correctedAcc * SMOOTHING_FACTOR;
}


float Gyro::correctAcc(float accX, float accY, float accZ) {
    // Normalize the deviations to determine the lean angle effect
    float magnitude = sqrt(accX * accX + accY * accY + accZ * accZ);
    
    // Assume that the lean angle correction affects primarily the X-axis
    // Adjust as needed based on motorcycle dynamics
    float lean_correction = acos(accZ / magnitude);  // Angle between Z-axis and gravity vector
    
    // Adjust acceleration based on lean
    float corrected_acc = accY / cos(lean_correction);  // Y-axis is most likely to reflect true acceleration
    return corrected_acc;
}