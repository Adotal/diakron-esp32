#include "motor_manager.h"

MotorManager::MotorManager()
{
    count = 0;
}

bool MotorManager::addAxis(char id, axis* ax)
{
    if(count >= MAX_MOTORS_PER_MANAGER)
        return false;

    axes[count++] = { id, ax };
    return true;
}

axis* MotorManager::getAxis(char id)
{
    for(uint8_t i = 0; i < count; i++)
    {
        if(axes[i].id == id)
            return axes[i].ax;
    }
    return nullptr;
}

void MotorManager::update()
{
    for(uint8_t i = 0; i < count; i++)
        axes[i].ax->update();
}

void MotorManager::homeAll()
{
    for(uint8_t i = 0; i < count; i++)
        axes[i].ax->startHoming();
}

bool MotorManager::isAnyAxisMoving()
{
    for(uint8_t i = 0; i < count; i++)
    {
        if(axes[i].ax->isMoved())
            return true;
    }
    return false;
}

bool MotorManager::allAxesHomed()
{
    for(uint8_t i = 0; i < count; i++)
    {
        if(!axes[i].ax->isHomed())
            return false;
    }
    return true;
}