#include "../LedService.hpp"

#include "../../components/Led.hpp"
#include "../../configs/PinConfigs.cpp"

Led defaultLed(PinConfigs::getDefaultLed());

Led yellowLed(14);
Led redLed(13);
Led greenLed(12);


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
