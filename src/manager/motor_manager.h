#pragma once
#include "../core/axis.h"
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

    WARNING 2
    If you try to move a NEMA and a 28BYJ at the same time, the NEMA will shudder and lose steps along its path. 
    My theory is that the 28BYJ is extremely slow at sending pulses, and this affects the NEMA's timing. 
    So, when it's time for its update, the time has already passed, and it's lost.
    DO NOT USE NEMA and 28BYJ at the same time.
*/

class MotorManager
{
private:
    struct AxisEntry
    {
        char id;
        axis* ax;
    };

    AxisEntry axes[MAX_MOTORS_PER_MANAGER];
    uint8_t count;

public:
    MotorManager();

    bool addAxis(char id, axis* ax);
    axis* getAxis(char id);

    void update();
    void homeAll();

    bool isAnyAxisMoving();
    bool allAxesHomed();
};