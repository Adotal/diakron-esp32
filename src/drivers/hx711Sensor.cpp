#include "hx711Sensor.h"
#include <Arduino.h>

HX711Sensor::HX711Sensor(uint8_t dataP, uint8_t clockP) : dataPin(dataP), clockPin(clockP)
{
    scale.begin(dataPin, clockPin);
}

void HX711Sensor::begin()
{
    prefs.begin("hx711", false);

    calibrationFactor = prefs.getFloat("cal", 1.0);
    offset = prefs.getLong("offset", 0);

    scale.set_scale(calibrationFactor);
    scale.set_offset(offset);
}

long HX711Sensor::read()
{
    if (scale.is_ready())
    {
        return scale.read();
    }
    return 0;
}

float HX711Sensor::getWeight()
{
    if (scale.is_ready())
    {
        return scale.get_units(10); // average of 10 readings
    }
    return 0;
}

void HX711Sensor::tare()
{
    scale.tare();
    offset = scale.get_offset();
    prefs.putLong("offset", offset);
}

void HX711Sensor::calibrate(float knownWeight)
{
    long reading = scale.get_units(10);
    calibrationFactor = reading / knownWeight;

    scale.set_scale(calibrationFactor);

    prefs.putFloat("cal", calibrationFactor);
}

float HX711Sensor::getCalibrationFactor()
{
    return calibrationFactor;
}