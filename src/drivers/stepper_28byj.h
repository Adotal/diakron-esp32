#pragma once
#include "../core/motor.h"
#include "../hal/IPinDriver.h"

/*
    This class represents a 28BYJ-48 stepper motor, which is a common and inexpensive stepper motor.
    It is a unipolar stepper motor that typically has 4 control wires (plus a common wire) and can 
    be driven using a ULN2003 driver board.
    The class inherits from the base `motor` class and implements the specific behavior for controlling 
    a 28BYJ-48 motor, including the step sequence and timing for generating the appropriate signals 
    on the control pins.

    ---- INFO ----
    - MAX RPM: 15 RPM
    - MAX RPM STABLE: 15 RPM
    - MIN RPM: 1 RPM

    -- NOTE --
    - The motor is slower than other types, so typically it will be used in max speed for the sensor 
      movements, which do not require high speed.
*/

class stepper_28byj : public motor
{
private:
    uint8_t pins[4];
    /*const uint8_t stepSequence[8][4] = {
        {1, 0, 0, 0},
        {1, 1, 0, 0},
        {0, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 0, 0, 1},
        {1, 0, 0, 1}
    };*/
    const uint8_t stepSequence[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    unsigned long lastStepTime;  // Last micros() where a step was taken
    unsigned long stepInterval;  // Microseconds between steps
    int stepIndex;
    bool direction; // true for forward, false for backward
    long position;

    long const DEFAULT_RPM_28BYJ = 15;
    long const MAX_RPM_28BYJ = 15;
    long const STEPS_PER_REVOLUTION = 2048; // 28BYJ-48 has 2048 steps per revolution in full-step mode
public:
    stepper_28byj(IPinDriver &driver, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    void begin();
    void step() override;
    void enable(bool en) override{} // motor does not have an enable pin, so this function can be left empty
    void update() override; 
    void setSpeed(long rpm);
    void setDirection(bool dir);
    long getPosition() override;
    void resetPosition(long pos = 0) override;
    long getMaxRPM() override;
    long getDefaultRPM() override;
};