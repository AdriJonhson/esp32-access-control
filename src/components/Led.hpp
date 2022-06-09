#ifndef LED_HPP
#define LED_HPP

#include <Arduino.h>

class Led {
    public:
        Led(const int pin);

        void blink(void);
        void on(void);
        void off(void);

    private:
        int _pin;
};

#endif
