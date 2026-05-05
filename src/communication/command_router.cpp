#include "command_router.h"

CommandRouter::CommandRouter(MotionProtocol& mp, StatusProtocol& sp, SensorProtocol& senp, CalibrationProtocol& calp) 
: motion(mp), status(sp), sensor(senp), calibration(calp)
{
}

bool CommandRouter::route(char* command)
{
    if(motion.handle(command)) return true;
    if(status.handle(command)) return true;
    if(sensor.handle(command)) return true;
    if(calibration.handle(command)) return true;
    return false;
}