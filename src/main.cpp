#include <Arduino.h>
#include <WiFi.h>

#include "./services/UserService.hpp"

const char* ssid = "Tix Maria";
const char* password = "88226842";

UserService userService;

void setup() {
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
  if((WiFi.status() == WL_CONNECTED)) {
    userService.getAllUsers();
  }else{
    Serial.println("Connection Lost...");
  }

  delay(10000);
}
