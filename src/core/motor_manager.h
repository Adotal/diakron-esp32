#pragma once
#include "axis.h"
#include "../config/defaults.h"
/*
    Multi-engine coordinator.
    1. Manage multiple axes
    2. Call `moveTo()` on all axes
    3. Command format can be defined as needed, for example:
        - "MOVE A100 B50 C-20" to move A axis 100 steps forward, B axis 50 steps forward and C axis 20 steps backward
        - "MOVETO A300 B500 C0" to move A axis to position 300, B axis to position 500 and C axis to position 0
        - "HOME A B C" to home all axes
    4. A letter is assigned for each axis added from A to Z

    WARNING
    When starting the microcontroller for the first time, always begin with calibration protocols (homing routine) 
    so that the absolute and relative movement routines execute correctly.
    This is because it will always measure the actual space using the boundary sensors.
*/


class MotorManager
{
private:
    struct AxisEntry {
        char id;
        axis* ax;
    };

    AxisEntry axes[MAX_MOTORS_PER_MANAGER];
    uint8_t count = 0;

public:
    bool addAxis(char id, axis* motorAx);
    axis* getAxis(char id);
    bool exeCommand(char* command);
    void update();
};