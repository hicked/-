#include <Wire.h>

#define MIN_GYRO_ERROR 1500
#define MIN_GYRO 2000
#define MAX_GYRO 16000
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
    float initialAccX = 0.0;
    float initialAccY = 0.0;
    float initialAccZ = 0.0;

public:
    float accX; // left and right
    float accY; // forward+ backwards- (ASSUMING BOARD IS FACING UP
    float accZ; // up and down

    Gyro();
    void Update();
};