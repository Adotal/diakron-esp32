#include "system_manager.h"

SystemManager::SystemManager(MotorManager& mm, CommandRouter& r, SystemController& sc, InterfaceUI& ui, CameraService& cam, FillLevelManager& fm, WebSocketService& ws)
    : motorManager(mm), router(r), controller(sc), display(ui), camera(cam), fillLevelManager(fm), wsService(ws)
{
}

void SystemManager::init()
{
    controller.setState(SystemState::IDLE);
    
}

void SystemManager::update()
{
    motorManager.update();

    SystemState state = controller.getState();

    if(state == SystemState::IDLE)
    {
        display.update();
    }

    // Camera
    if(state == SystemState::CAPTURING)
    {
        if(camera.hasNewResult())
        {
            String result = camera.getPrediction();
            Logger::info(("Prediction: " + result).c_str());

            // lógica de clasificación (luego la movemos)
            // selectFinalM();
            // Aquí decides siguiente estado

            controller.setState(SystemState::IDLE);
        }
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
    if(controller.isEstopped())
        return;

    if(!router.route(cmd))
    {
        Logger::error("INVALID COMMAND!");
    }
}

SystemController& SystemManager::getController()
{
    return controller;
}

void SystemManager::handleExternalCommand(const String& cmd)
{
    if(cmd == "CAPT")
    {
        controller.setState(SystemState::CAPTURING);
        // aquí después llamaremos camera
    }
    else if(cmd == "FL")
    {
        fillLevelManager.sendLevels(wsService);
    }
}