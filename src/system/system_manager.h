#pragma once
#include "../manager/motor_manager.h"
#include "../communication/command_router.h"
#include "system_controller.h"
#include "../core/interfaceUI.h"
#include "../services/camera_service.h"
#include "../manager/fill_level_manager.h"
#include "../services/websocket_service.h"
class SystemManager
{
private:
    MotorManager& motorManager;
    CommandRouter& router;
    SystemController &controller;
    InterfaceUI &display;
    CameraService& camera;
    FillLevelManager& fillLevelManager;
    WebSocketService& wsService;

public:
    SystemManager(MotorManager& mm, CommandRouter& r, SystemController& sc, InterfaceUI& ui, CameraService& cam, FillLevelManager& fm, WebSocketService& ws);

    void init();
    void update();
    void processCommand(char* cmd);
    void handleExternalCommand(const String& cmd);
    SystemController& getController();
};