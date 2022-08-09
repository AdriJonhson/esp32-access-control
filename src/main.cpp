#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>

#include "./configs/PinConfig.cpp"
#include "./configs/WifiConfig.cpp"

#include "./services/UserService.hpp"
#include "./services/LedService.hpp"

#define CLIENT_ID  "esp32-main"
#define EXCHANGE_PUBLISH_CARD "user_card_verify"
#define QUEUE_NEW_USER_WAITING_CARD "mqtt-subscription-esp32-mainqos0"

const char* MQTT_HOST = "147.182.191.7"; 
const char* MQTT_USER = "esp32";
const char* MQTT_PASSWORD= "esp32";
const int MQTT_PORT = 1883;

// WiFi CREDENTIALS
const char *ssid = "MOB-MARIA";
const char *password = "88226842ma";

// Internal Services
UserService userService;
LedService ledServiceMain;

// Lib Instances
MFRC522 rfid(PinConfigs::dataPin, PinConfigs::resetPin);
WiFiClient wifiClient;

PubSubClient MQTT(wifiClient);

String actionType;
String registerIdentify;

StaticJsonDocument<200> parseJsonTest(String payload)
{
  char jsonMessage[500];
  payload.replace(" ", "");
  payload.replace("\n", "");
  payload.trim();
  payload.toCharArray(jsonMessage, 500);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, jsonMessage);

  return doc;
}

void requestRfidCode(String registerId)
{
  ledServiceMain.onLedYellow();

  actionType = "RCD";
  registerIdentify = registerId;
}

void callback(char *topic, byte *payload, unsigned int length) {
  String message = "";

  Serial.println("-----------------------");
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    message.concat(String((char) payload[i]));
  }

  StaticJsonDocument<200> doc = parseJsonTest(message);

  const char *msg = doc["msg"];
  Serial.println(String(msg) + "\n");

  Serial.println();
  Serial.println("-----------------------");

  const char *actionType = doc["actionType"];
  String actionTypeFinal = String(actionType);

  Serial.println("Action Type: " + actionTypeFinal + "\n");

  if(actionTypeFinal == "RCD") {
    const char *registerIdentify = doc["registerIdentify"];
    String registerIdentifyFinal = String(registerIdentify);

    requestRfidCode(registerIdentifyFinal);
  }
}

void initWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(2, HIGH);
    
    return;
  }

  digitalWrite(2, LOW);

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  while (!MQTT.connected()) {
      Serial.print("Conectando ao Broker MQTT: ");
      Serial.println(MQTT_HOST);

      if (MQTT.connect(CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
        Serial.println("Conectado ao Broker com sucesso!");
      } 
      else {
        Serial.println("Noo foi possivel se conectar ao broker.");
        Serial.println("Nova tentatica de conexao em 10s");
        delay(10000);
      }
  }
}

void verifyConnect() {
  initWifi();

  if (!MQTT.connected()) {
    initMQTT(); 
  }
}

void setup() {
  Serial.begin(115200);

  SPI.begin();
  rfid.PCD_Init();
  
  Serial.print("Starting ESP32");

  MQTT.setServer(MQTT_HOST, MQTT_PORT);
  MQTT.setCallback(callback);

  verifyConnect();

  MQTT.subscribe(QUEUE_NEW_USER_WAITING_CARD);
}

void loop() {
  verifyConnect();

  MQTT.loop();

  String cardCode = "";

  if (! rfid.PICC_IsNewCardPresent())
    return;

  if (! rfid.PICC_ReadCardSerial())
    return;

  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    cardCode.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
    cardCode.concat(String(rfid.uid.uidByte[i], HEX));
  }

  Serial.println("Verificando cartão...\n");

  if((WiFi.status() == WL_CONNECTED)) {
    cardCode.toUpperCase();

    char *cardCodeFinal = new char[cardCode.length() + 1];
    strcpy(cardCodeFinal, cardCode.c_str());

    Serial.println("Card Code: " + String(cardCodeFinal) + "\n");

    if(actionType == "RCD") {
      ledServiceMain.offLedYellow();

      actionType = "";
      registerIdentify = "";

      Serial.println("-----------------------");
    }else{
      Serial.println("Verificando usuário...\n");

      userService.verifyUserAccess(cardCodeFinal);

      Serial.println("-----------------------");
      Serial.println("Waiting for card");
      Serial.println("-----------------------");
    }
  }else{
    Serial.println("Connection Lost...");
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}