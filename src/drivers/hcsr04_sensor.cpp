#include "hcsr04_sensor.h"
#include <Arduino.h>
HCSR04Sensor::HCSR04Sensor(IPinDriver& driver, uint8_t trig, uint8_t echo, uint16_t maxDist)
    : driver(driver), trigPin(trig), echoPin(echo), maxDistanceCm(maxDist)
{}

void HCSR04Sensor::begin()
{
    driver.pinMode(trigPin, OUTPUT);
    driver.pinMode(echoPin, INPUT);
    driver.digitalWrite(trigPin, LOW);
}

uint16_t HCSR04Sensor::readDistance()
{
    // Trigger pulse
    driver.digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    driver.digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);

    driver.digitalWrite(trigPin, LOW);

    // Read echo
    unsigned long duration = driver.pulseIn(echoPin, HIGH, 30000UL);

    uint16_t distance = duration / 29 / 2;

    if(distance > maxDistanceCm)
        distance = maxDistanceCm;

    return distance;
}

uint8_t HCSR04Sensor::getFillPercentage()
{
    uint16_t distance = readDistance();

    if (distance > maxDistanceCm)
        distance = maxDistanceCm;

    return 100 - (distance * 100 / maxDistanceCm);
}