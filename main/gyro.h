#include <Wire.h>

#define MIN_GYRO_ERROR 1000

#define MIN_GYRO_BREAKING 2000 // buffer area where gyro wont do anything
#define MIN_GYRO_ACCELERATING 1000 // buffer area where gyro wont do anything

#define MAX_GYRO_BREAKING 13000 // divide by around 1600 to get the acceleration in gs
#define MAX_GYRO_ACCELERATING 7000

#define SMOOTHING_FACTOR 0.04 // lower value is more smoothing, less vibrations, but less reactive/fast
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
    const int MPU = 0x68;

    int numReadings = 0;
    float sumReadings = 0;

    float plannedAccX = 0.0;
    float plannedAccY = 0.0;
    float plannedAccZ = 1600.0;

    // gonna have to do sm weird 3d vector algebra to be able to figure out if were going uphill, aka
    // disregard the gravitational component of the acceleration that is measured. same can be done for lean angle

public:
    float accX; // left and right
    float accY; // forward+ backwards- (ASSUMING BOARD IS FACING UP
    float accZ; // up and down

    float smoothedX;
    float smoothedY;
    float smoothedZ;

    Gyro();
    void Update();
};