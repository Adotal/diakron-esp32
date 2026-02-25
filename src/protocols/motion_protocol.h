#pragma once
#include "../manager/motor_manager.h"
#include "../system/system_controller.h"
#include "../communication/logger.h"
/*
    This class interprets all commands related to motor_manager.
    This class receives the command thanks to command_router.
*/

class MotionProtocol
{
private:
    MotorManager& manager;
    SystemController& controller;

public:
    MotionProtocol(MotorManager& mm, SystemController& sc);

    bool handle(char* command);
};