#include <Arduino.h>
#include <CanBus.h>
#include <CanWebServer.h>
#include <WiFi.h>
#include <Wireless.h>

CanBus canBus;
Wireless wireless;
CanWebServer webServer(80);

void setup() {
    Serial.begin(921600);
    canBus.begin();
    wireless.begin();

    Serial.println("IP:  " + wireless.getIpAddress());
    Serial.println("Mac: " + wireless.getMacAddress());

    webServer.begin();
}

void loop() {
    CanMessage* messages = canBus.update();
    int length = canBus.getMessageLength();
    webServer.updateData(messages, length);
}
