#include <Arduino.h>
#include <CanBus.h>
#include <CanWebServer.h>
#include <WiFi.h>
#include <Wireless.h>

CanBus canBus;
Wireless wireless;
CanWebServer webServer(80);

void setup() {
    Serial.begin(115200);
    canBus.begin();
    wireless.begin();
    webServer.begin();

    Serial.println("IP:  " + wireless.getIpAddress());
    Serial.println("Mac: " + wireless.getMacAddress());
}

void loop() {
    CanMessage* messages = canBus.update();
    int length = canBus.getMessageLength();
    webServer.updateData(messages, length);
}
