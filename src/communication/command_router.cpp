#include "command_router.h"

CommandRouter::CommandRouter(MotionProtocol& mp, StatusProtocol& sp)
    : motion(mp), status(sp)
{
}

bool CommandRouter::route(char* command)
{
    if(motion.handle(command)) return true;
    if(status.handle(command)) return true;
    
    return false;
}