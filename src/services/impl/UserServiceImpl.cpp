#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "../UserService.hpp"

void UserService::getAllUsers(void) {
    long rnd = random(1, 10);
    HTTPClient client;

    client.begin("https://jsonplaceholder.typicode.com/todos/" + String(rnd));
    int httpCode = client.GET();

    if(httpCode > 0) {
      String payload = client.getString();

      Serial.println("Status Code: " + String(httpCode));
      Serial.println(payload);
    }
}
