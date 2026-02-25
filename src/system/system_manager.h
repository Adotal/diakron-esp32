#pragma once
#include "../manager/motor_manager.h"
#include "../communication/command_router.h"
#include "system_controller.h"
#include "../core/interfaceUI.h"
class SystemManager
{
private:
    MotorManager& motorManager;
    CommandRouter& router;
    SystemController &controller;
    InterfaceUI &display;

public:
    SystemManager(MotorManager& mm, CommandRouter& r, SystemController& sc, InterfaceUI& ui);

    void init();
    void update();
    void processCommand(char* cmd);

    SystemController& getController();
};