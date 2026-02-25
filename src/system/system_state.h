#pragma once

enum class SystemState
{
    BOOT,
    IDLE,
    HOMING,
    RUNNING,
    ERROR,
    ESTOP
};

enum class SystemError
{
    NONE,
    NOT_HOMED,
    LIMIT_HIT,
    MOTOR_FAULT,
    UNKNOWN_COMMAND
};

