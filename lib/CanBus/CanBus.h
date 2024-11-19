#ifndef CAN_BUS_H
#define CAN_BUS_H

#include <CAN.h>

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

class CanBus {
   public:
    void begin();
    void update();
    String getCanDataJson();
    void printCanMessage(long canId, CanMessage canMessage);

   private:
    int messageLength;
    CanMessage messages[MAX_MESSAGES];
    CanMessage createCanMessage(long id, int length, bool isRtr,
                                bool isExtended);
    int getIndex(long canId);
    void canMessageToString(CanMessage message);
};

#endif