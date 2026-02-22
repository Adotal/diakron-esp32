#pragma once
#include "../hal/IPinDriver.h"

/*
    This class represents a physical limit, such as a switch or sensor that indicates the end of a travel.
    It can be used to perform homing routines or to prevent a motor from moving beyond a safe point.
    In this case, a `Limits` class is implemented that uses an `IPinDriver` to read the state of a specific pin.
    The class constructor takes a reference to an `IPinDriver`, the pin number to be used for the limit, and an 
    optional boolean to indicate whether the pin reading is inverted (i.e., whether the limit is triggered by a 
    LOW instead of a HIGH).
    The class has a `begin()` method to configure the pin as an input, and a `isTriggered()` method that returns a 
    boolean indicating whether the limit is triggered or not, taking into account the possible signal inversion.
*/

class Limits
{
private:
    IPinDriver &driver;
    uint8_t pin;
    bool inverted;
public:
    Limits(IPinDriver &drv, uint8_t p, bool inv = false);
    void begin();
    bool isTriggered();
};
