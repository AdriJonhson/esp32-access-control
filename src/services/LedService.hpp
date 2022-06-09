#ifndef LED_SERVICE
#define LED_SERVICE

#include <Arduino.h>

class LedService {
    public:
        void blinkDefault(void);
        void blinkGreen(void);
        void blinkRed(void);
        void onLedYellow(void);
        void offLedYellow(void);

};

#endif
