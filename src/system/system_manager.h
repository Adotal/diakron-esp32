#pragma once
#include "../motion/motor_manager.h"
#include "../communication/command_router.h"
#include "system_controller.h"

class SystemManager
{
private:
    MotorManager& motorManager;
    CommandRouter& router;
    SystemController controller;

public:
    SystemManager(MotorManager& mm, CommandRouter& r);

    void init();
    void update();
    void processCommand(char* cmd);

    SystemController& getController();
};