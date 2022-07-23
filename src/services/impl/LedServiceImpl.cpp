#include "../LedService.hpp"

#include "../../components/Led.hpp"
#include "../../configs/PinConfig.cpp"

Led defaultLed(PinConfigs::defaultLed);

Led yellowLed(13);
Led redLed(12);
Led greenLed(14);


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
