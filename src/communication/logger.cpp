#include "logger.h"

void Logger::info(const char* msg)
{
    Serial.print("[INFO] ");
    Serial.println(msg);
}

void Logger::error(const char* msg)
{
    Serial.print("[ERROR] ");
    Serial.println(msg);
}

void Logger::state(const char* msg)
{
    Serial.print("[STATE] ");
    Serial.println(msg);
}