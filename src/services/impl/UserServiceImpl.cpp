#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "../UserService.hpp"
#include "../LedService.hpp"

LedService ledService;

// const String HOST = "https://esp32-access-control-api.herokuapp.com";
const String HOST = "http://147.182.191.7:8080";

StaticJsonDocument<200> parseJson(String payload)
{
  char json[500];
  payload.replace(" ", "");
  payload.replace("\n", "");
  payload.trim();
  payload.toCharArray(json, 500);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, json);

  return doc;
}

String requestGetUser(char *rfidCode)
{
  HTTPClient client;

  client.begin(HOST + "/api/users/verify-access?rfidCode=" + rfidCode);

  int httpCode = client.GET();

  String payload;

  if (httpCode != 200)
  {
    client.end();

    Serial.println("User not found: " + String(rfidCode));

    return "";
  }

  payload = client.getString();

  Serial.println("Status Code: " + String(httpCode));
  Serial.println("Payload: " + String(payload));

  client.end();

  return payload;
}

void searchUserByRfidCode(char *rfidCode)
{
  String response = requestGetUser(rfidCode);

  ledService.offLedYellow();

  if(response == "") {
    ledService.blinkRed();

    return;
  }

  StaticJsonDocument<200> doc = parseJson(response);

  int id = doc["id"];
  const char *name = doc["name"];

  Serial.println(String(id) + " - " + String(name) + "\n");

  ledService.blinkGreen();
}

void UserService::verifyUserAccess(char *rfidCode)
{
  ledService.onLedYellow();

  searchUserByRfidCode(rfidCode);
}

void UserService::sendRfidCode(char *registerIdentify, char *rfidCode)
{
  HTTPClient client;

  client.begin(HOST + "/api/users/send-rfid-code");
  client.addHeader("Content-Type", "application/json");
  
  String jsonOutput = "{ \"registerIdentify\": \""+String(registerIdentify)+"\", \"rfidCode\": \""+String(rfidCode)+"\"}";

  int httpCode = client.POST(jsonOutput);

  Serial.println("request: " + jsonOutput);

  if (httpCode != 200)
  {
    client.end();

    ledService.offLedYellow();
    ledService.blinkRed();

    return;
  }

  client.end();

  ledService.offLedYellow();
  ledService.blinkGreen();
}
