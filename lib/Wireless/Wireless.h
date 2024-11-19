#ifndef WIRELESS_H
#define WIRELESS_H

#include <WiFi.h>

class Wireless {
   public:
    void begin();
    String getIpAddress();
    String getMacAddress();

   private:
};

#endif