#pragma once
#include "../hal/IPinDriver.h"

enum class ButtonEvent 
{
        NONE,
        SHORT_PRESS,
        LONG_PRESS
};


class ButtonUI
{
private:
    IPinDriver &driver;
    uint8_t pin;
    bool inverted;

    unsigned long pressStartTime;
    bool lastButtonState;
    bool longPressDetected;
public:
    ButtonUI(IPinDriver &drv, uint8_t p, bool in = false);
    void begin();
    bool isTriggered();
    ButtonEvent handleButton();
};

