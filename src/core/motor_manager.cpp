#include "motor_manager.h"
#include <Arduino.h>


bool MotorManager::addAxis(char id, axis* motorAx) {
    if(count >= MAX_MOTORS_PER_MANAGER)
        return false;

    axes[count++] = { id, motorAx };
    return true;
}

axis* MotorManager::getAxis(char id) {
    for(uint8_t i = 0; i < count; i++) {
        if(axes[i].id == id)
            return axes[i].ax;
    }
    return nullptr;
}

bool MotorManager::exeCommand(char* command)
{
    if(strncmp(command, "HOME ALL", 8) == 0)
    {
        for(uint8_t i=0;i<count;i++)
            axes[i].ax->startHoming();

        return true;
    }

    if(strncmp(command, "HOME ", 5) == 0)
    {
        char axisID = command[5];
        axis* ax = getAxis(axisID);

        if(ax)
            ax->startHoming();

        return true;
    }

    if(strncmp(command, "MOVE ", 5) == 0)
    {
        const char* ptr = command + 5;

        while(*ptr != '\0')
        {
            char axisID = *ptr++;
            long value = strtol(ptr, (char**)&ptr, 10);

            axis* ax = getAxis(axisID);
            if(ax)
                ax->moveRelative(value);
        }

        return true;
    }

    if(strncmp(command, "MOVETO ", 7) == 0)
    {
        const char* ptr = command + 7;

        while(*ptr != '\0')
        {
            char axisID = *ptr++;
            long value = strtol(ptr, (char**)&ptr, 10);

            axis* ax = getAxis(axisID);
            if(ax)
                ax->moveTo(value);
        }

        return true;
    }

    return false;
}

void MotorManager::update()
{
    for(uint8_t i=0;i<count;i++)
        axes[i].ax->update();
}