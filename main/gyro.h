#include <Wire.h>

#define MIN_GYRO_ERROR 1500
#define MIN_GYRO 2000 // buffer area where gyro wont do anything
#define MAX_GYRO_BREAKING 15000
#define MAX_GYRO_ACCELERATING 10000
#define NUM_READINGS_SMOOTHING 25
#define SMOOTHING_FACTOR 0.05 //lower value is more smoothing, less vibrations, less reactive/fast
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

    float initialAccX = 0.0;
    float initialAccY = 0.0;
    float initialAccZ = 0.0;

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