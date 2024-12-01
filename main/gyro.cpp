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
        this->initialAccX = Wire.read() << 8 | Wire.read();    
        this->initialAccY = Wire.read() << 8 | Wire.read();  
        this->initialAccZ = Wire.read() << 8 | Wire.read();
    } else {
        Serial.println("Failed to read from MPU");
    }

    if (abs(this->initialAccX - plannedAccX) > MIN_GYRO_ERROR) {
        Serial.println("WARNING, X AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACCURATE");
    }
    if (abs(this->initialAccY) > MIN_GYRO_ERROR) {
        Serial.println("WARNING, Y AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACCURATE");
    }
    if (abs(this->initialAccZ - plannedAccZ) > MIN_GYRO_ERROR) {
        Serial.println("WARNING, Z AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACCURATE");
    }
    Serial.println("GYRO SETUP");
    Serial.print("Initial Accel X: ");
    Serial.println(this->initialAccX);
    Serial.print("Initial Accel Y: ");
    Serial.println(this->initialAccY);
    Serial.print("Initial Accel Z: ");
    Serial.println(this->initialAccZ);
}

void Gyro::Update() {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we dont care about rotational

    if (Wire.available() == 6) {
        this->accX = Wire.read() << 8 | Wire.read();    
        this->accY = Wire.read() << 8 | Wire.read();  
        this->accZ = Wire.read() << 8 | Wire.read();
    } else {
        Serial.println("Failed to read from MPU");
        return;
    }

    this->smoothedX = SMOOTHING_FACTOR * accX + (1 - SMOOTHING_FACTOR) * smoothedX;
    this->smoothedY = SMOOTHING_FACTOR * accY + (1 - SMOOTHING_FACTOR) * smoothedY;
    this->smoothedZ = SMOOTHING_FACTOR * accZ + (1 - SMOOTHING_FACTOR) * smoothedZ;
}