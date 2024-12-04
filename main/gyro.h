#include <Wire.h>

#define MIN_GYRO_BREAKING 2000 // buffer area where gyro wont do anything
#define MAX_GYRO_BREAKING 13000 // divide by around 1600 to get the acceleration in gs

#define MIN_GYRO_ACCELERATING 1000 // buffer area where gyro wont do anything
#define MAX_GYRO_ACCELERATING 7000 

#define EXPECTED_ACC_X 0.0
#define EXPECTED_ACC_Y 0.0
#define EXPECTED_ACC_Z 16384.0


#define SMOOTHING_FACTOR 0.1 // lower value is more smoothing, less vibrations, but less reactive/fast
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

public:
    float measuredAccX; // left and right
    float measuredAccY; // forward+ backwards- (ASSUMING BOARD IS FACING UP)
    float measuredAccZ; // up and down

    float correctedAcc; // disregarding gravity

    float smoothedAcc = 0.0; // smoothed out acceleration for dealing with bumps and irregularities

    Gyro();
    void Update();
};