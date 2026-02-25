#include "motion_protocol.h"
#include <Arduino.h>
#include <string.h>
#include <stdlib.h>

MotionProtocol::MotionProtocol(MotorManager& mm, SystemController& sc)
    : manager(mm), controller(sc)
{
}

bool MotionProtocol::handle(char* command)
{
    if(strncmp(command, "HOME ALL", 8) == 0)
    {
        Logger::info("HOME ALL START");
        manager.homeAll();
        controller.setState(SystemState::HOMING);
        return true;
    }

    if(strncmp(command, "HOME ", 5) == 0)
    {
        char axisID = command[5];
        axis* ax = manager.getAxis(axisID);
        Logger::info("STARTING HOME");

        if(ax){
            ax->startHoming();
            controller.setState(SystemState::HOMING);
        }

        return true;
    }

    if(strncmp(command, "MOVE ", 5) == 0)
    {   
        

        if (!controller.canMove()){
            Logger::info("THE AXIS COULD NOT BE MOVED IN THIS MOMENT");
            return true;
        }
        
        const char* ptr = command + 5;

        while(*ptr != '\0')
        {
            char axisID = *ptr++;
            long value = strtol(ptr, (char**)&ptr, 10);

            axis* ax = manager.getAxis(axisID);
            if(ax){
                if(!ax->isHomed())
                {
                    Logger::error("AXIS NOT HOMED");
                    return true;
                }
                ax->moveRelative(value);
                controller.setState(SystemState::RUNNING);
            }
        }

        return true;
    }

    if(strncmp(command, "MOVETO ", 7) == 0)
    {
        if (!controller.canMove()){
            Logger::info("THE AXIS COULD NOT BE MOVED IN THIS MOMENT");
            return true;
        }

        const char* ptr = command + 7;

        while(*ptr != '\0')
        {
            char axisID = *ptr++;
            long value = strtol(ptr, (char**)&ptr, 10);

            axis* ax = manager.getAxis(axisID);
            if(ax){
                if(!ax->isHomed())
                {
                    Logger::error("AXIS NOT HOMED");
                    return true;
                }
                ax->moveTo(value);
                controller.setState(SystemState::RUNNING);
            }
        }
        return true;
    }

    return false;
}