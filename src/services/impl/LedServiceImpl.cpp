#include "../LedService.hpp"

#include "../../components/Led.hpp"
#include "../../configs/PinConfigs.cpp"

Led defaultLed(PinConfigs::getDefaultLed());

void LedService::blinkDefault(void) {
    defaultLed.blink();
}
