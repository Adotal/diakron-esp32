#pragma once
#include "../system/system_controller.h"
#include "../communication/logger.h"
/*
    This class is related to returning the current system status to the user
    by interpreting the STATUS command sent by command_router.
*/

class StatusProtocol
{
private:
    SystemController& controller;

public:
    StatusProtocol(SystemController& sc);

    bool handle(char* command);
};