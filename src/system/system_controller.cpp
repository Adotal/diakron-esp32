#include "system_controller.h"

SystemController::SystemController()
{
    state = SystemState::BOOT;
}

void SystemController::setState(SystemState s)
{
    state = s;
}

SystemState SystemController::getState() const
{
    return state;
}

bool SystemController::canMove() const
{
    return state == SystemState::IDLE ||
           state == SystemState::RUNNING;
}

bool SystemController::isInError() const
{
    return state == SystemState::ERROR;
}

bool SystemController::isEstopped() const
{
    return state == SystemState::ESTOP;
}