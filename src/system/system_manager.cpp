#include "system_manager.h"

SystemManager::SystemManager(MotorManager& mm, CommandRouter& r)
    : motorManager(mm), router(r)
{
}

void SystemManager::init()
{
    controller.setState(SystemState::IDLE);
}

void SystemManager::update()
{
    // Allways update motors
    motorManager.update();
    SystemState state = controller.getState();

    if(state == SystemState::HOMING)
    {
        if(motorManager.allAxesHomed())
        {
            Logger::info("HOMING COMPLETE");
            controller.setState(SystemState::IDLE);
        }
    }

    if(state == SystemState::RUNNING)
    {
        if(!motorManager.isAnyAxisMoving())
        {
            Logger::info("MOTION COMPLETE");
            controller.setState(SystemState::IDLE);
        }
    }
}

void SystemManager::processCommand(char* cmd)
{
    // Si está en ESTOP no aceptar comandos
    if(controller.isEstopped())
        return;

    if(!router.route(cmd))
    {
        controller.setState(SystemState::ERROR);
    }
}

SystemController& SystemManager::getController()
{
    return controller;
}