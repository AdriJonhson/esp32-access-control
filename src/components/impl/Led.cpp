#include "../Led.hpp"

Led::Led(const int pin) {
    _pin = pin;

    pinMode(_pin, OUTPUT);
}

void Led::blink(void) {
    digitalWrite(_pin, HIGH);
    delay(1000);
    digitalWrite(_pin, LOW);
}

void Led::on(void) {
    digitalWrite(_pin, HIGH);
}

void Led::off(void) {
    digitalWrite(_pin, LOW);
}