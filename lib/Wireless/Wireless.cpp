#include <Wireless.h>

#include "config.h"

void Wireless::begin() {
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

String Wireless::getIpAddress() { return WiFi.localIP().toString(); }
String Wireless::getMacAddress() { return WiFi.macAddress(); }