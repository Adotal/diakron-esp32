#pragma once
#include "../drivers/hcsr04_sensor.h"
#include "../services/websocket_service.h"
class FillLevelManager{
private:
    static const uint8_t MAX_BINS = 4;
    HCSR04Sensor* sensors[MAX_BINS];
    uint8_t count = 0;
    
public:
    void addSensor(HCSR04Sensor* sensor);
    uint8_t getFillPercentage(uint8_t index);
    void buildPayload(uint8_t* buffer);
    void sendLevels(WebSocketService& ws);
};