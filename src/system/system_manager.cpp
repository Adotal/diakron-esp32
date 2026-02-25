#include "system_manager.h"

SystemManager::SystemManager(MotorManager& mm, CommandRouter& r, SystemController& sc, InterfaceUI& ui)
    : motorManager(mm), router(r), controller(sc), display(ui)
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

    // ONLY UPDATE OLED IF THE SYSTEM IS NOT WORKING. THIS IS BECAUSE IT HAS PROBLEMS WITH THE MOTOR MOVEMENT.
    if(state == SystemState::IDLE){
        display.update();
    }

    if(state == SystemState::HOMING)
    {
        if(motorManager.allAxesHomed())
        {
            Logger::info("ALL HOMING COMPLETED");
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