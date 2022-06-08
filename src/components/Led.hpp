#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>

class Led {
    public:
        Led(const int pin);

        void blink(void);

    private:
        int _pin;
};

#endif
