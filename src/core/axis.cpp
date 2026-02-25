#include "axis.h"

axis::axis(motor &motorType, Limits &sw, long maxTravel, bool inverted) : motorRef(motorType), homeSwitch(sw), maxTravelSteps(maxTravel), isInverted(inverted) {
    state = HOMING_IDLE;
    measuredMaxSteps = 0;
    isInverted = inverted;
    targetPosition = 0;
    isMoving = false;
}

void axis::startHoming() {
    motorRef.enable(true);
    bool searchDir = isInverted;
    motorRef.setDirection(searchDir);
    motorRef.setSpeed(dynamicSpeed(50)); // Start with 50% speed
    state = HOMING_FAST_SEARCH;
}

void axis::updateHoming(){
    bool toSensor = isInverted;
    bool awayFromSensor = !isInverted;
    switch (state)
    {
    case HOMING_FAST_SEARCH:
        if(!homeSwitch.isTriggered()){
            motorRef.update();
        } else{
            state = HOMING_BACKOFF;
            motorRef.setDirection(awayFromSensor);
            motorRef.setSpeed(dynamicSpeed(20)); // Back off at 20% speed
        }
        break;

    case HOMING_BACKOFF:
        if(homeSwitch.isTriggered()){
            motorRef.update();
        }else{
            state = HOMING_SLOW_SEARCH;
            motorRef.setDirection(toSensor);
            motorRef.setSpeed(dynamicSpeed(20)); // Slow search at 20% speed
        }
        break;
    
    case HOMING_SLOW_SEARCH:
        if(!homeSwitch.isTriggered()){
            motorRef.update();
        }else{
            // Home really found, set position to zero
            motorRef.resetPosition(0); 
            // Move away from the sensor to measure the max travel
            motorRef.setDirection(awayFromSensor);
            motorRef.setSpeed(dynamicSpeed(60));
            state = HOMING_SET_ZERO;
        }
        break;
    
    case HOMING_SET_ZERO:
        motorRef.resetPosition(0);
        motorRef.setSpeed(dynamicSpeed(70)); // Set max travel at 70% speed
        state = HOMING_MEASURE_TRAVEL;
        break;
    
    case HOMING_MEASURE_TRAVEL:
        if(motorRef.getPosition() < maxTravelSteps){
            motorRef.update();
        }else{
            measuredMaxSteps = motorRef.getPosition();
            //motorRef.enable(false);
            motorRef.setDirection(toSensor);
            state = HOMING_RETURN_TO_ZERO;
        }
        break;
    case HOMING_RETURN_TO_ZERO:
        if(motorRef.getPosition() > 0){
            motorRef.update();
        }else{
            motorRef.enable(false);
            state = HOMING_DONE;
        }
        break;
    case HOMING_DONE:
        // Homing complete, do nothing or maintain state
        motorRef.enable(false);
        break;

    case HOMING_ERROR:
        // Handle error state
        motorRef.enable(false);
        break;

    default:
        break;
    }
}

bool axis::isHomed() const{
    return state == HOMING_DONE;
}

bool axis::isMoved() const
{
    return isMoving;
}

long axis::getMeasuredMax() const{
    return measuredMaxSteps;
}

long axis::dynamicSpeed(int percentage){
    // This function can be implemented to adjust the speed based on the distance to the target position
    // For example, you can reduce the speed as it gets closer to the target to avoid overshooting
    // This is just a placeholder implementation and can be adjusted based on the specific requirements of your application
    return (((motorRef.getDefaultRPM() + motorRef.getMaxRPM())/2) * percentage) / 100;
}

bool axis::moveTo(long target) {

    // Software Limits
    if(target < 0) target = 0;
    if(target > measuredMaxSteps) target = measuredMaxSteps;

    long current = motorRef.getPosition();
    if(target == current) return true;

    motorRef.setDirection(target > current);

    targetPosition = target;
    isMoving = true;
    return true;
}

bool axis::moveRelative(long delta)
{
    return moveTo(motorRef.getPosition() + delta);
}

void axis::update()
{
    if(!isHomed())
    {
        updateHoming();
        return;
    }

    if(isMoving)
    {
        motorRef.enable(true);

        long pos = motorRef.getPosition();

        if(pos == targetPosition)
        {
            isMoving = false;
            motorRef.enable(false);
            return;
        }

        motorRef.update();
    }
}