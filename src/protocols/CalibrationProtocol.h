
#ifndef CALIBRATIONPROTOCOL_H
#define CALIBRATIONPROTOCOL_H

#include "../drivers/hx711Sensor.h"
#include "../communication/logger.h"

/*
    This protocol is responsible for handling commands related to the calibration of the HX711 sensor. 
    It can handle commands to tare the sensor, set a new calibration factor based on a known weight, 
    and get the current calibration factor.

    Commands:
        - CAL:TARE -> Tares the sensor
        - CAL:SET:<known_weight> -> Sets a new calibration factor based on the provided known weight
        - CAL:GET -> Gets the current calibration factor
    How to calibrate the scale:
        1- Remove all weight (empty scale)
        2- Execute the command "CAL:TARE" to adjust the offset (zero)
        3- Execute the command "CAL:SET:1000" to set a known weight of 1000g

    NOTE: It is not necessary to calibrate every time the system starts. The last calibration is saved 
    in internal memory; if it does not exist, a default value is used.
*/

class CalibrationProtocol
{
private:
    HX711Sensor& scale;
public:
    CalibrationProtocol(HX711Sensor& s);
    bool handle(char* command);
};

#endif

