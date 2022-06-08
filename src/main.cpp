#include <Arduino.h>
#include "./services/LedService.hpp"

LedService ledService;

void setup()
{
  
}

void loop()
{
  ledService.blinkDefault();
}