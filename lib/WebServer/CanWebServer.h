#ifndef CAN_WEB_SERVER_H
#define CAN_WEB_SERVER_H

#include <ArduinoJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "CanBus.h"
#include "SPIFFS.h"

class CanWebServer {
   public:
    CanWebServer(uint16_t port = 80);
    void begin();
    void updateData(CanMessage* messages, int length);

   private:
    CanMessage* messages;
    int messageLength;

    AsyncWebServer server;

    void setUpWebServer();
    void setUpBackendServer();

    String canDataToJson();
};

#endif