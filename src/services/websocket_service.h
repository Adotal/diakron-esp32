#pragma once
#include <ESPAsyncWebServer.h>

class WebSocketService
{
private:
    AsyncWebServer& server;
    AsyncWebSocket ws;

    // external callback
    std::function<void(const String&)> onMessageCallback;

public:
    WebSocketService(AsyncWebServer& srv);

    void init();
    void update();
    void onMessage(std::function<void(const String&)> cb);

    void sendText(const String& msg);
    void sendBinary(uint8_t* data, size_t len);

private:
    static void handleEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                            AwsEventType type, void *arg, uint8_t *data, size_t len);

    void processMessage(void *arg, uint8_t *data, size_t len);
};