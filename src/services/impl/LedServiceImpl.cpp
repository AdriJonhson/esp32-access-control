#include "../LedService.hpp"

#include "../../components/Led.hpp"
#include "../../configs/PinConfig.cpp"

Led defaultLed(PinConfigs::defaultLed);

Led yellowLed(2);
Led redLed(2);
Led greenLed(2);


void LedService::blinkDefault(void) {
    defaultLed.blink();
}

void LedService::blinkGreen(void) {
    greenLed.blink();
}

void LedService::blinkRed(void) {
    redLed.blink();
}

void LedService::onLedYellow(void) {
    yellowLed.on();
}

void LedService::offLedYellow(void) {
    yellowLed.off();
}
