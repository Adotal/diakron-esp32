#pragma once
#include <Arduino.h>
#include <HX711.h>
#include <Preferences.h>
// #include "../hal/IPinDriver.h"

class HX711Sensor
{
private:
    uint8_t dataPin;
    uint8_t clockPin;
    HX711 scale;
    Preferences prefs;

    float calibrationFactor = 1.0;
    long offset = 0;

    //IPinDriver &driver; // Don't supported in this sensor, it needs to be refactored to work with the driver instead of the library
public:
    HX711Sensor( uint8_t dataP, uint8_t clockP);
    void begin();
    long read();
    float getWeight();
    void tare();
    void calibrate(float knownWeight);
    float getCalibrationFactor();
};