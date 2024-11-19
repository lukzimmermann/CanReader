#include "CanBus.h"

void CanBus::begin() {
    messageLength = 0;
    CAN.setPins(4, 5);
    if (!CAN.begin(1000E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }
}

void CanBus::update() {
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

String CanBus::getCanDataJson() {
    const unsigned long startTime = micros();

    const int bufferSize = 8024;
    char json[bufferSize];
    int pos = 0;

    pos += snprintf(json + pos, bufferSize - pos, "[");
    for (int i = 0; i < messageLength; i++) {
        pos += snprintf(json + pos, bufferSize - pos, "{");
        pos += snprintf(json + pos, bufferSize - pos, "\"id\":%u,",
                        messages[i].id);
        pos += snprintf(json + pos, bufferSize - pos, "\"length\":%u,",
                        messages[i].length);

        pos += snprintf(json + pos, bufferSize - pos, "\"data\":[");
        for (int j = 0; j < messages[i].length; j++) {
            pos += snprintf(json + pos, bufferSize - pos, "%u,",
                            messages[i].data[j]);
        }
        if (messages[i].length > 0) {
            pos--;  // Remove trailing comma
        }
        pos += snprintf(json + pos, bufferSize - pos, "],");

        pos += snprintf(json + pos, bufferSize - pos, "\"isRTR\":%s,",
                        messages[i].isRTR ? "true" : "false");
        pos += snprintf(json + pos, bufferSize - pos, "\"isExtended\":%s,",
                        messages[i].isExtended ? "true" : "false");
        pos += snprintf(json + pos, bufferSize - pos, "\"frequency\":%u",
                        messages[i].frequency);
        pos += snprintf(json + pos, bufferSize - pos, "},");
    }
    if (messageLength > 0) {
        pos--;  // Remove trailing comma
    }
    pos += snprintf(json + pos, bufferSize - pos, "]");

    Serial.print("CreateJson: ");
    Serial.println(micros() - startTime);

    return String(json);  // Convert to String if necessary
}

int CanBus::getIndex(long canId) {
    for (int i = 0; i < MAX_MESSAGES; i++) {
        if (messages[i].id == -1) return -1;
        if (messages[i].id == canId) return i;
    }
    return -1;
}

CanMessage CanBus::createCanMessage(long id, int length, bool isRtr,
                                    bool isExtended) {
    CanMessage canMessage;
    canMessage.id = id;
    canMessage.length = length;
    canMessage.isRTR = isRtr;
    canMessage.isExtended = isExtended;
    canMessage.time = micros();
    return canMessage;
}

void CanBus::printCanMessage(long canId, CanMessage canMessage) {
    if (canMessage.id == canId) {
        canMessageToString(canMessage);
    }
}

void CanBus::canMessageToString(CanMessage message) {
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