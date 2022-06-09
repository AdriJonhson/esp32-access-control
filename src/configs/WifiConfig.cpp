#include <Arduino.h>

#define SSID "Tix Maria"
#define PASSWORD "88226842"

class WifiConfig {

    public:
        static String getSsid() {
            return SSID;
        }

        static String getPassword() {
            return PASSWORD;
        }
};