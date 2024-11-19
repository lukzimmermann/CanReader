#include "CanBus.h"

void CanBus::begin() {
    messageLength = 0;
    CAN.setPins(4, 5);
    if (!CAN.begin(1000E3)) {
        Serial.println("Starting CAN failed!");
        while (1);
    }
}

CanMessage* CanBus::update() {
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
    return messages;
}

int CanBus::getMessageLength() { return messageLength; }

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
