#pragma once
#include "../drivers/buttonUI.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../config/icon.h"
#include "../config/defaults.h"
#include "../communication/logger.h"
/*
    This class is responsible for relating an OLED screen to its respective button.
*/

enum UIState {
    UI_MENU,
    UI_VIEW_SENSORS,
    UI_VIEW_LEVELS,
    UI_CAMERA_TEST,
    UI_WIFI_STATUS,
    UI_HOME_MOTORS
};

class InterfaceUI
{
    
private:
    ButtonUI& button;
    Adafruit_SSD1306& display;
    UIState currentState;
    uint8_t selectedIndex;
    uint8_t menuLength;

    struct MenuItem
    {
        const char* label;
        UIState targetState;
    };
    static const MenuItem menuItems[];

    bool needsRedraw;

    void drawCurrentScreen();
public:
    InterfaceUI(Adafruit_SSD1306& oled, ButtonUI& btn);
    void begin();
    void update();
    void ui_nextItem();
    void ui_select();
    UIState ui_getState();
};

