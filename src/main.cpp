#include <Arduino.h>
#include <WiFi.h>

#include "./services/UserService.hpp"
#include "./services/LedService.hpp"

const char* ssid = "Tix Maria";
const char* password = "88226842";

UserService userService;
LedService ledService2;

void setup() {
  pinMode(27, INPUT);

  Serial.begin(9600);
  Serial.print("Starting ESP32");

  WiFi.begin(ssid, password);
  Serial.print("\nConnection to WiFi");

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected");
}

void loop() {
  if (digitalRead(27) == HIGH){
    Serial.println("Verificando usuário...\n");

    if((WiFi.status() == WL_CONNECTED)) {
      userService.verifyUserAccess("CARD1");
    }else{
      Serial.println("Connection Lost...");
    }
  }

  delay(500);
}
