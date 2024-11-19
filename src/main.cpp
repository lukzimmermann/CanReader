#include <Arduino.h>
#include <CanBus.h>
#include <CanWebServer.h>
#include <WiFi.h>

#include "config.h"

CanBus canBus;
CanWebServer webServer(80);

void setup() {
    Serial.begin(921600);
    canBus.begin();

    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Serial.print("IP:  ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());

    webServer.begin();
}

void loop() {
    CanMessage* messages = canBus.update();
    int length = canBus.getMessageLength();
    webServer.updateData(messages, length);
}
