#include "buttonUI.h"
#include <Arduino.h>

ButtonUI::ButtonUI(IPinDriver &drv, uint8_t p, bool inv) : driver(drv), pin(p), inverted(inv){
    pressStartTime = 0;
    lastButtonState = HIGH;
    longPressDetected = false;
}

void ButtonUI::begin(){
    driver.pinMode(pin, INPUT_PULLUP);
}

bool ButtonUI::isTriggered(){
    bool state = driver.digitalRead(pin);
    return inverted ? !state : state;
}

ButtonEvent ButtonUI::handleButton(){
    bool currentState = isTriggered();

    ButtonEvent event = ButtonEvent::NONE;

    // Detects the start of a pulse
    if (lastButtonState == HIGH && currentState == LOW)
    {
        pressStartTime = millis();
        longPressDetected = false;
    }

    // Press and hold
    if (lastButtonState == LOW && currentState == LOW)
    {
        if (!longPressDetected && (millis() - pressStartTime >= 2000))
        {
            longPressDetected = true;
            event = ButtonEvent::LONG_PRESS;
        }
    }

    // Drop
    if (lastButtonState == LOW && currentState == HIGH)
    {
        if (!longPressDetected)
        {
            event = ButtonEvent::SHORT_PRESS;
        }
    }

    lastButtonState = currentState;
    return event;
}