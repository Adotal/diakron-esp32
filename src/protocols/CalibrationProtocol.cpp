#include "CalibrationProtocol.h"
#include <string.h>
#include <Arduino.h>

CalibrationProtocol::CalibrationProtocol(HX711Sensor &s) : scale(s) {}

bool CalibrationProtocol::handle(char *command)
{
    if (strncmp(command, "CAL:TARE", 8) == 0)
    {
        scale.tare();
        Logger::info("Tare done");
        return true;
    }

    if (strncmp(command, "CAL:SET:", 8) == 0)
    {
        float known = atof(command + 8);

        scale.calibrate(known);

        String msg = "Calibration updated: " + String(scale.getCalibrationFactor());
        Logger::info(msg.c_str());

        return true;
    }

    if (strncmp(command, "CAL:GET", 8) == 0)
    {
        String msg = "Current calibration factor: " + String(scale.getCalibrationFactor());
        Logger::info(msg.c_str());
        return true;
    }
    

    return false;
}