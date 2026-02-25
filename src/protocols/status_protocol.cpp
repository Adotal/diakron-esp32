#include "status_protocol.h"
#include <Arduino.h>
#include <string.h>

StatusProtocol::StatusProtocol(SystemController& sc)
    : controller(sc)
{
}

bool StatusProtocol::handle(char* command)
{
    // ===== STATE =====
    if(strncmp(command, "STATE", 5) == 0)
    {
        switch(controller.getState())
        {
            case SystemState::BOOT:    Logger::state("BOOT"); break;
            case SystemState::IDLE:    Logger::state("IDLE"); break;
            case SystemState::HOMING:  Logger::state("HOMING"); break;
            case SystemState::RUNNING: Logger::state("RUNNING"); break;
            case SystemState::ERROR:   Logger::state("ERROR"); break;
            case SystemState::ESTOP:   Logger::state("ESTOP"); break;
        }
        return true;
    }

    return false;
}