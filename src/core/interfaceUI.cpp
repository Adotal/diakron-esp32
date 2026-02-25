#include "interfaceUI.h"

const InterfaceUI::MenuItem InterfaceUI::menuItems[] = {
    {"Sensores", UI_VIEW_SENSORS},
    {"Niveles", UI_VIEW_LEVELS},
    {"Test Camara", UI_CAMERA_TEST},
    {"WiFi Status", UI_WIFI_STATUS},
    {"Homing Motores", UI_HOME_MOTORS}
};

InterfaceUI::InterfaceUI(Adafruit_SSD1306& oled, ButtonUI& btn) : button(btn), display(oled){
    menuLength = sizeof(menuItems) / sizeof(menuItems[0]);
    
    currentState = UI_MENU;
    selectedIndex = 0;
    needsRedraw = true;
}

void InterfaceUI::begin(){

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
        Logger::error("OLED BEGIN FAILED");
    }else{
        display.clearDisplay();
        display.display();
        display.drawBitmap( (display.width() - LOGO_WIDTH ) / 2,((display.height()- LOGO_HEIGHT) / 2 ), diakronIc, LOGO_WIDTH, LOGO_HEIGHT, WHITE);
        display.display();
    }
}

void InterfaceUI::update()
{
    ButtonEvent evt = button.handleButton();

    if (currentState == UI_MENU)
    {
        if (evt == ButtonEvent::SHORT_PRESS)
        {
            ui_nextItem();
            needsRedraw = true;
        }
        else if (evt == ButtonEvent::LONG_PRESS)
        {
            ui_select();
            needsRedraw = true;
        }
    }
    else
    {
        if (evt == ButtonEvent::LONG_PRESS)
        {
            ui_select();  // volver al menú
            needsRedraw = true;
        }
    }

    if (needsRedraw)
    {
        drawCurrentScreen();
        needsRedraw = false;
    }
}


void InterfaceUI::drawCurrentScreen()
{
    display.clearDisplay();

    if (currentState == UI_MENU)
    {
        for (uint8_t i = 0; i < menuLength; i++)
        {
            int y = i * 12;
            if (i == selectedIndex)
            {
                display.fillRect(0, y, 128, 12, SSD1306_WHITE);
                display.setTextColor(SSD1306_BLACK);
            }
            else
            {
                display.setTextColor(SSD1306_WHITE);
            }
            display.setCursor(5, y + 2);
            display.print(menuItems[i].label);
        }
        display.display();
    }
    else
    {
        switch (currentState)
        {
            case UI_VIEW_SENSORS:
                display.println("Vista Sensores");
                break;

            case UI_VIEW_LEVELS:
                display.println("Vista Niveles");
                break;

            case UI_CAMERA_TEST:
                display.println("Test Camara");
                break;

            case UI_WIFI_STATUS:
                display.println("WiFi Status");
                break;
            case UI_HOME_MOTORS:
                display.println("HOMING");
                break;
            default:
                break;
        }
    }

    display.display();
}


void InterfaceUI::ui_nextItem()
{
    if (currentState == UI_MENU)
    {
        selectedIndex++;
        if (selectedIndex >= menuLength)
            selectedIndex = 0;
    }
}

void InterfaceUI::ui_select()
{
    if (currentState == UI_MENU)
        currentState = menuItems[selectedIndex].targetState;
    else
        currentState = UI_MENU;
}

UIState InterfaceUI::ui_getState()
{
    return currentState;
}