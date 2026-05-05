#pragma once
#include "../hal/IPinDriver.h"

class HCSR04Sensor
{
private:
    IPinDriver& driver;
    uint8_t echoPin;
    uint8_t trigPin;
    uint16_t maxDistanceCm;

public:
    HCSR04Sensor(IPinDriver& driver, uint8_t trig, uint8_t echo, uint16_t maxDist);

    void begin();

    uint16_t readDistance();   // cm
    uint8_t getFillPercentage(); // %
};