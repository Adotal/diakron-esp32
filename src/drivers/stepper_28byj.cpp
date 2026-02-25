#include "stepper_28byj.h"
#include <Arduino.h>

stepper_28byj::stepper_28byj(IPinDriver &driver, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) : motor(&driver) {
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    stepIndex = 0;
    direction = true;
    position = 0;
}

void stepper_28byj::begin() {
    for (int i = 0; i < 4; i++) {
        driver->pinMode(pins[i], OUTPUT);
        driver->digitalWrite(pins[i], LOW);
    }
}

void stepper_28byj::step() {
    for (int i = 0; i < 4; i++) {
        driver->digitalWrite(pins[i], stepSequence[stepIndex][i]);
    }
    if(direction){
        stepIndex = (stepIndex + 1) % 4; // Move to the next step in the sequence
        position++;
    }
    else{
        stepIndex = (stepIndex + 3) % 4; // Move to the previous step in the sequence
        position--;
    }
}

void stepper_28byj::update() {
    unsigned long currentTime = micros();
    
    // Timer
    if (currentTime - lastStepTime >= stepInterval) {
        lastStepTime = currentTime;
        step(); // Execute a step
        if(direction)
            position++;
        else
            position--;
    }
}

void stepper_28byj::setSpeed(long rpm) {
    if (rpm <= 0) return;
    // (60s * 1,000,000us) / (steps_per_revolution * rpm)
    this->stepInterval = 60000000L / (STEPS_PER_REVOLUTION * rpm);
}



void stepper_28byj::setDirection(bool dir) {
    direction = dir;
    direction = dir ? 1 : 0;
}

long stepper_28byj::getPosition() {
    return position;
}

void stepper_28byj::resetPosition(long pos) {
    position = pos;
}

long stepper_28byj::getMaxRPM() {
    return MAX_RPM_28BYJ;
}

long stepper_28byj::getDefaultRPM() {
    return DEFAULT_RPM_28BYJ;
}