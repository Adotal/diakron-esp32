#pragma once
#include <Arduino.h>

class Logger
{
public:
    static void info(const char* msg);
    static void error(const char* msg);
    static void state(const char* msg);
};