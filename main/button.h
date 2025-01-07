#pragma once
#include <Arduino.h>

#define BUTTON_PIN 8 // Pin for the button
#define BUTTON_ENABLED true

enum BrakeMode {
    BRAKE_MODE_NORMAL,
    BRAKE_MODE_FLASHLIGHT,
    BRAKE_MODE_CHRISTMAS,
    BRAKE_MODE_HALLOWEEN,
    BRAKE_MODE_MARIO_STAR,

    BRAKE_MODE_NUM_MODES
};

class Button {
public:
    bool state = false;
    bool prevState = false;
    int mode = 0;
    void Update();
};