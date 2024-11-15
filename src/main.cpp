#include <Arduino.h>
#include <AsyncTCP.h>
#include <CAN.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

#define MAX_MESSAGES 200

struct CanMessage {
    long id;
    int length;
    uint8_t data[8];
    bool isRTR;
    bool isExtended;
    unsigned long time;
    float frequency;
};

CanMessage messages[MAX_MESSAGES];
int messageLength = 0;
unsigned int counter = 0;

const char* ssid = "SI-Public";
const char* password = "suterindustries-8488";

int getIndex(long canId) {
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (messages[i].id == -1) return -1;
        if (messages[i].id == canId) return i;
    }
    return -1;
}

void canMessageToString(CanMessage message) {
    Serial.println();
    Serial.println("-----------------");
    Serial.print("ID:         0x");
    Serial.println(message.id, HEX);
    Serial.println("-----------------");
    Serial.print("Length:     ");
    Serial.println(message.length);
    Serial.print("IsExtended: ");
    Serial.println(message.isExtended);
    Serial.print("IsRTR:      ");
    Serial.println(message.isRTR);
    Serial.print("Frequency:  ");
    Serial.println(message.frequency);
}

void printCanMessage(long canId, CanMessage canMessage) {
    if (canMessage.id == canId) {
        canMessageToString(canMessage);
    }
}

String getRawCanDataJson() {
    String json = "[";
    for (int i = 0; i < messageLength; i++) {
        json.concat("{");
        json.concat("\"id\": \"0x" + String(messages[i].id, HEX) + "\",");
        json.concat("\"length\": " + String(messages[i].length) + ",");
        for (int j = 0; j < messages[i].length; j++) {
            json.concat("\"data" + String(j) +
                        "\": " + String(messages[i].data[j]) + ",");
        }
        json.concat("\"isRTR\": " + String(messages[i].isRTR) + ",");
        json.concat("\"isExtended\": " + String(messages[i].isExtended) + ",");
        json.concat("\"frequency\": " + String(messages[i].frequency));
        json.concat("},");
    }
    json.remove(json.length() - 1);
    json.concat("]");
    return json;
}

AsyncWebServer server(3000);

void setup() {
    Serial.begin(921600);
    CAN.setPins(4, 5);
    if (!CAN.begin(1000E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    Serial.print("IP:  ");
    Serial.println(WiFi.localIP());
    Serial.print("Mac: ");
    Serial.println(WiFi.macAddress());

    server.on("/raw-can", HTTP_GET, [](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse(
            200, "application/json", getRawCanDataJson());
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
    });

    server.begin();
}

CanMessage createCanMessage(long id, int length, bool isRtr, bool isExtended) {
    CanMessage canMessage;
    canMessage.id = id;
    canMessage.length = length;
    canMessage.isRTR = isRtr;
    canMessage.isExtended = isExtended;
    canMessage.time = micros();
    return canMessage;
}

CanMessage updateCanMessage(long id, int length, bool isRtr, bool isExtended) {
    CanMessage canMessage;
    canMessage.id = id;
    canMessage.length = length;
    canMessage.isRTR = isRtr;
    canMessage.isExtended = isExtended;
    canMessage.time = micros();
    return canMessage;
}

void loop() {
    int packetSize = CAN.parsePacket();
    if (packetSize) {
        int index = getIndex(CAN.packetId());

        if (index == -1) {
            messages[messageLength] =
                createCanMessage(CAN.packetId(), packetSize, CAN.packetRtr(),
                                 CAN.packetExtended());

            messageLength++;
            Serial.print("ID: 0x");
            Serial.println(CAN.packetId(), HEX);
        } else {
            messages[index].id = CAN.packetId();
            messages[index].length = packetSize;
            messages[index].isRTR = CAN.packetRtr();
            messages[index].isExtended = CAN.packetExtended();
            messages[index].frequency =
                double(1E6) / double(micros() - messages[index].time);
            messages[index].time = micros();

            byte bytes[packetSize];
            for (int i = 0; i < packetSize; i++) {
                messages[index].data[i] = CAN.read();
            }
        }
    }
}
