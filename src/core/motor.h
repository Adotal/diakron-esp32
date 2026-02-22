#pragma once
#include "../hal/IPinDriver.h"

/*
    Class to define what is common to all engines
    A base class that represents any engine.
    Unifies common behavior.
    - All motors:
    - Need to initialize pins
    - Need to take steps
    - Use a pin driver
*/

/* 
    - Example usage mcp_driver:
        Adafruit_MCP23X17 mcp(0x20);
        mcp_driver i2cExpander(mcp);
        nema17 motor1(&i2cExpander, 2, 5, 10);
        stteper_28byj motor2(&i2cExpander, 5,6,7,8);

    - Example usage direct pin control:
        gpio_driver gpioDrv;
        nema17 motor1(&gpioDrv, 2, 5, 10);
        stepper_28byj motor2(&gpioDrv, 5,6,7,8);
*/

/*
    To manipulate motors, a base class `motor` is defined, which is abstract. 
    Each motor type (nema17, 28byj) then inherits from this class and implements its specific methods.
    However, for more advanced control, an `axis` class is defined, which combines 
    a motor with a limit switch to perform homing routines and relative or absolute movements.
    Finally, there is a `MotorManager` that can handle multiple axes, allowing movement 
    commands to be sent to all of them in a coordinated manner.
    MORE INFORMATION IN axis.h and motor_manager.h

*/

class motor{
protected:
    IPinDriver* driver;
public:
    motor(IPinDriver* drv) : driver(drv) {}
    virtual void begin() = 0;
    virtual void enable(bool en) = 0;
    virtual void setDirection(bool dir) = 0;
    virtual void update() = 0;
    virtual void setSpeed(long rpm) = 0;
    virtual void step() = 0;
    virtual long getPosition() = 0;
    virtual void resetPosition(long pos = 0) = 0;
    virtual long getMaxRPM() = 0;
    virtual long getDefaultRPM() = 0;
};

