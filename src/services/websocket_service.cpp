#include "websocket_service.h"

WebSocketService::WebSocketService(AsyncWebServer& srv)
    : server(srv), ws("/ws")
{
}

void WebSocketService::init()
{
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len)
    {
        switch (type)
        {
        case WS_EVT_CONNECT:
            Serial.printf("Client %u connected\n", client->id());
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("Client %u disconnected\n", client->id());
            break;

        case WS_EVT_DATA:
            processMessage(arg, data, len);
            break;

        default:
            break;
        }
    });

    server.addHandler(&ws);
}

void WebSocketService::onMessage(std::function<void(const String&)> cb)
{
    onMessageCallback = cb;
}

void WebSocketService::processMessage(void *arg, uint8_t *data, size_t len)
{
    AwsFrameInfo *info = (AwsFrameInfo *)arg;

    if (!(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT))
        return;

    String msg;
    for (size_t i = 0; i < len; i++)
        msg += (char)data[i];

    Serial.println("WS Received: " + msg);

    if (onMessageCallback)
        onMessageCallback(msg);
}

void WebSocketService::sendText(const String& msg)
{
    ws.textAll(msg);
}

void WebSocketService::sendBinary(uint8_t* data, size_t len)
{
    ws.binaryAll(data, len);
}

void WebSocketService::update()
{
    ws.cleanupClients();
}