#pragma once
#include <Wire.h>
/*
    VCC -> 3.3 V / 5 V (better) 
    GND -> GND 
    SCL -> A5 
    SDA -> A4 

    NOTE, GYRO SHOULD HAVE LIGHT TOWARDS STRAIGHT, PINS ON RIGHT
    TRY TO GET AS FLAT AT POSSIBLE
*/

// divide all these values by 17000 to get the amount in g force
#define MIN_GYRO_BREAKING 500 // buffer area where gyro wont do anything
#define MAX_GYRO_BREAKING 17000

#define MIN_GYRO_ACCELERATING 1000 // buffer area where gyro wont do anything
#define MAX_GYRO_ACCELERATING 17000

#define EXPECTED_ACC_MAGNITUDE 17000.0
#define CALIBRATION_SAMPLE_SIZE 50 // sample size for calibration phase
#define CALIBRATION_ACC_DELTA 500 // ensures that the acc found during calibration is within this of EXPECTED_ACC_MAGNITUDE


// Filtering and smoothing

// First, it will check the raw value of the acceleration. If this value is too dissimilar from the previous value,
// the data will be disregarded. It automatically detect errors, overriding bumps if it detects continuous bumps for x amount of time.
// side note, there is a bug where it gets in an infinite loop if multiple overrides stack. For this reason there is a minimum time between overrides
#define FILTER_DELTA false

#define BUMP_THRESHOLD 1000 // if the acceleration changes by this much, its probably a bump
#define SAMPLE_SIZE_BUMPS 20 // if there are this many bumps in a row that are within threshold of each other,
// its not a bump, so override

#define BUMP_OVERRIDE_TIME 5000 // force an update if bumps are detected for this many millis
#define MIN_TIME_BETWEEN_OVERRIDES 6000


// Secondly, the program takes a sample size of gyro values
// It will take the average of n data, and calculate the average
#define FILTER_AVG true
#define AVG_SAMPLE_SIZE 5


// Finaly, it smoothed the previous and new values together instead of just setting them
#define FILTER_SMOOTHING true
#define SMOOTHING_FACTOR 0.3 // lower value is more smoothing, less vibrations, but less reactive/fast



class Gyro {
private:
    const int MPU = 0x68; // MPU6050 I2C address

    unsigned long lastUpdateTime = 0;
    unsigned long lastForceUpdate = 0;
    float idleAcc = 0.0;

    float measuredAccX; // left and right
    float measuredAccY; // forward+ backwards- (ASSUMING BOARD IS FACING UP)
    float measuredAccZ; // up and down

    float minBump = 100000.0;
    float maxBump = -100000.0;
    int numBumps = 0;

    int numSamples = 0;
    float sumSamples = 0.0;
    float avgAcc = 0.0;

    float correctedAcc = 0.0; // disregarding idle gravity

public:
    float prevAcc = 0.0; // previous corrected acceleration, used to filter out bumps
    float smoothedAcc = 0.0; // smoothed out acceleration for dealing with bumps and irregularities, signed
    Gyro();
    void Update();
};