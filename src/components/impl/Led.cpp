#include "../Led.hpp"

Led::Led(const int pin) {
    _pin = pin;

    pinMode(_pin, OUTPUT);
}

void Led::blink(void) {
    digitalWrite(_pin, HIGH);
    delay(500);

    digitalWrite(_pin, LOW);
    delay(500);
}