#include "fill_level_manager.h"


void FillLevelManager::addSensor(HCSR04Sensor* sensor)
{
    if(count < MAX_BINS)
    {
        sensors[count++] = sensor;
    }
}

uint8_t FillLevelManager::getFillPercentage(uint8_t index)
{
    if(index >= count) return 0;

    uint32_t sum = 0;

    for(int i = 0; i < 3; i++)
    {
        sum += sensors[index]->readDistance();
    }

    uint16_t avg = sum / 3;

    // convertir a porcentaje
    uint16_t depth = 100;

    uint8_t percent = 100 - ((avg * 100) / depth);

    return percent;
}

void FillLevelManager::buildPayload(uint8_t* buffer)
{
    buffer[0] = 'F';
    buffer[1] = 'L';

    for(uint8_t i = 0; i < count; i++)
    {
        buffer[i+2] = getFillPercentage(i);
    }
}

void FillLevelManager::sendLevels(WebSocketService& ws)
{
    uint8_t fillLevels[6];
    fillLevels[0] = 'F';
    fillLevels[1] = 'L';

    for (int i = 0; i < count; i++)
    {
        fillLevels[i+2] = sensors[i]->getFillPercentage();
    }

    ws.sendBinary(fillLevels, sizeof(fillLevels));
}