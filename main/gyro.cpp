#include "gyro.h"
#include <Arduino.h>

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

    if (this->initialAccX/16300.0 > MIN_GYRO_ERROR) {
        Serial.println("WARNING, X AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACURATE");
    }
    if (this->initialAccY/16300.0 > MIN_GYRO_ERROR) {
        Serial.println("WARNING, Y AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACURATE");
    }
    if ((this->initialAccZ/16300.0)-1.0 > MIN_GYRO_ERROR) {
        Serial.println("WARNING, Z AXIS OF GYRO NOT SETUP PROPERLY, RESULTS MAY NOT BE ACURATE");
    }
    Serial.println("GYRO SETUP");
    Serial.print("Initial Accel X: ");
    Serial.println(this->initialAccX/16300.0);
    Serial.print("Initial Accel Y: ");
    Serial.println(this->initialAccY/16300.0);
    Serial.print("Initial Accel Z: ");
    Serial.println(this->initialAccZ/16300.0);
}

void Gyro::Update() {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);  
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);  // 6 pieces of data cause we dont care about rotational

    if (Wire.available() == 6) {
        this->accX = ((Wire.read() << 8 | Wire.read()) - this->initialAccX)/16300.0;    
        this->accY = ((Wire.read() << 8 | Wire.read()) - this->initialAccY)/16300.0;  
        this->accZ = ((Wire.read() << 8 | Wire.read()) - this->initialAccZ)/16300.0;
    } else {
        Serial.println("Failed to read from MPU");
    }
}