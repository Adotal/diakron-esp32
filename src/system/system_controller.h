#pragma once
#include "system_state.h"
#include "../communication/logger.h"
/*
    This class is responsible for determining if an action can be performed
    depending on the machine's state; it also helps to change the state
    with `setState` or obtain the state with `getState`.
    The other methods are for verifying the possibility of performing different
    actions.
*/

class SystemController
{
private:
    SystemState state;

public:
    SystemController();

    void setState(SystemState s);
    SystemState getState() const;

    bool canMove() const;
    bool isInError() const;
    bool isEstopped() const;
};