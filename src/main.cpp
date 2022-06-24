#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "./services/UserService.hpp"
#include "./services/LedService.hpp"

#define ID_MQTT  "BCI01"
#define TOPIC_PUBLISH "BCIBotao1"
#define TOPIC_CONSUMER "BCIBotao2"

const char* ssid = "Tix Maria";
const char* password = "88226842";
WiFiClient wifiClient;

//MQTT Server
const char* BROKER_MQTT = "broker.hivemq.com";
int BROKER_PORT = 1883;

PubSubClient MQTT(wifiClient);

UserService userService;

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
  Serial.print(ssid);
  Serial.println("  Waiting!");

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Wifi Connected: ");
  Serial.print(ssid);  
  Serial.print("  IP: ");
  Serial.println(WiFi.localIP());
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
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
       conectaMQTT(); 
    }
    
    wifiConnect();
}

void setup() {
  pinMode(27, INPUT);
  pinMode(2, OUTPUT);

  Serial.begin(9600);
  Serial.print("Starting ESP32");

  wifiConnect();

  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callback);

  String clientId = "esp32-adri";

  if (MQTT.connect(clientId.c_str())) {
      Serial.println("connected");
      MQTT.subscribe(TOPIC_PUBLISH);
  }
}


void loop() {
  verifyConnect();

  if (digitalRead(27) == HIGH){
    MQTT.publish(TOPIC_PUBLISH, "0");

    Serial.println("Verificando usuário...\n");

    if((WiFi.status() == WL_CONNECTED)) {
      userService.verifyUserAccess("CARD1");
    }else{
      Serial.println("Connection Lost...");
    }
  }

  delay(500);

  MQTT.loop();
}
