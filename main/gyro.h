#pragma once
#include <Wire.h>

// divide all these values by 17050 to get the amount in g force
#define MIN_GYRO_BREAKING 2000 // buffer area where gyro wont do anything
#define MAX_GYRO_BREAKING 10000

#define MIN_GYRO_ACCELERATING 1500 // buffer area where gyro wont do anything
#define MAX_GYRO_ACCELERATING 5000 

#define EXPECTED_ACC_MAGNITUDE 16650.0

#define SMOOTHING_FACTOR 0.1 // lower value is more smoothing, less vibrations, but less reactive/fast
#define UPDATE_TIMEOUT 100 // Timeout in milliseconds to force an update
#define BUMP_THRESHOLD 500 // if the acceleration changes by this much, its probably a bump
/*
    VCC -> 3.3 V / 5 V (better) 
    GND -> GND 
    SCL -> A5 
    SDA -> A4 

    NOTE, GYRO SHOULD HAVE LIGHT TOWARDS STRAIGHT, PINS ON RIGHT
    TRY TO GET AS FLAT AT POSSIBLE
*/

class Gyro {
private:
    const int MPU = 0x68; // MPU6050 I2C address
    unsigned long lastUpdateTime = 0;

public:
    float measuredAccX; // left and right
    float measuredAccY; // forward+ backwards- (ASSUMING BOARD IS FACING UP)
    float measuredAccZ; // up and down

    bool accelerating = false;

    float prevCorrectedAcc = 0.0; // previous corrected acceleration, used to filter out bumps
    float correctedAcc = 0.0; // disregarding gravity, and sign

    float smoothedAcc = 0.0; // smoothed out acceleration for dealing with bumps and irregularities, disregards sign

    Gyro();
    void Update();
};