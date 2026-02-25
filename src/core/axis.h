#pragma once
#include "motor.h"
#include "../drivers/limits.h"

/*
    This class represents a shaft, which is a combination of a motor and a limit switch for homing.
    The shaft is responsible for performing the homing routine, or relative or absolute movement, taking into account the physical limits of the system.
    Honing is performed in several steps to ensure that the true zero point is found, and not a false positive due to noise or a switch error.
    -------------- HOW IT WORKS --------------
    1. Start homing in the default direction (can be inverted with isInverted)
    2. When the switch is activated, back up until you release the switch.
    3. Approach slowly until the switch is triggered again, this is the home position, set it to zero
    4. Move in the opposite direction until the switch is triggered again, this is the max travel, save this value
    5. Move back to home position and stop, homing is done

    After homing, the axis can receive moveTo() commands with absolute positions, or moveRelative() commands with relative movements.
    The axis will take care of enabling the motor, setting the direction and speed, and disabling
*/

class axis
{
private:
    motor &motorRef;
    Limits &homeSwitch;
    enum HomingState {
        HOMING_IDLE,
        HOMING_FAST_SEARCH,
        HOMING_BACKOFF,
        HOMING_SLOW_SEARCH,
        HOMING_SET_ZERO,
        HOMING_MEASURE_TRAVEL,
        HOMING_RETURN_TO_ZERO,
        HOMING_DONE,
        HOMING_ERROR
    };
    HomingState state; 
    long maxTravelSteps;            // Theoretical safety limit
    long measuredMaxSteps;          // Actual measured distance
    bool isInverted;                // If the direction is inverted, so the homing will be in the opposite direction
    long targetPosition;
    bool isMoving;
    long dynamicSpeed(int percentage);    
public:
    axis(motor &motorType, Limits &sw, long maxTravel, bool inverted = false);
    void startHoming();
    void updateHoming();
    bool isHomed() const;
    bool isMoved() const;
    long getMeasuredMax() const;
    bool moveTo(long target);
    bool moveRelative(long delta);
    void update();
};
