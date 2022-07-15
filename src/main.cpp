#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#include "./configs/PinConfig.cpp"
#include "./configs/WifiConfig.cpp"

#include "./services/UserService.hpp"

#define ID_MQTT  "BCI01"
#define TOPIC_PUBLISH "valid_access"
#define TOPIC_CONSUMER "valid_access"

#define PASSWORD "88226842"
#define SSID "Tix Maria"

const char* MQTT_HOST = "162.243.165.170"; 
const char* MQTT_USER = "esp32";
const char* MQTT_PASSWORD= "esp32";
const int MQTT_PORT = 1883;

// Internal Services
UserService userService;

// Lib Instances
MFRC522 rfid(PinConfigs::dataPin, PinConfigs::resetPin);
WiFiClient wifiClient;

PubSubClient MQTT(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

void wifiConnect() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(2, HIGH);

    return;
  }

  digitalWrite(2, LOW);

  Serial.print("Connecting: ");
  Serial.print(SSID);
  Serial.println("  Waiting!\n");

  WiFi.begin(SSID, PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Wifi Connected: ");
  Serial.print(SSID);  
  Serial.print("  IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!MQTT.connected()) {
      Serial.print("Conectando ao Broker MQTT: ");
      Serial.println(MQTT_HOST);

      String clientId = "esp32-adri";

      if (MQTT.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
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

  if (!MQTT.connected()) {
    connectMQTT(); 
  }
 
  wifiConnect();
}

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();
  Serial.print("Starting ESP32");

  wifiConnect();

  MQTT.setServer(MQTT_HOST, MQTT_PORT);
  MQTT.setCallback(callback);

  String clientId = "esp32-adri";

  if (MQTT.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
    Serial.println("connected");
    MQTT.subscribe(TOPIC_CONSUMER);
  }
}

void loop() {
  verifyConnect();

  // MQTT.publish(TOPIC_PUBLISH, "0");

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

  Serial.println("Verificando usuário...\n");

  if((WiFi.status() == WL_CONNECTED)) {
    cardCode.toUpperCase();

    char *cardCodeFinal = new char[cardCode.length() + 1];
    strcpy(cardCodeFinal, cardCode.c_str());

    Serial.println("Card Code: " + String(cardCodeFinal) + "\n");

    userService.verifyUserAccess(cardCodeFinal);
  }else{
    Serial.println("Connection Lost...");
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  MQTT.loop();
}
