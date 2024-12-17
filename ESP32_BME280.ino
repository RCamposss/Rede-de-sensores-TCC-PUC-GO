//Programa ESP32 - Sensor BME280 - AsyncMQTT
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Credenciais Wi-Fi
#define WIFI_SSID "nome_da_rede"
#define WIFI_PASSWORD "senha_da_rede"

// Endereço do broker MQTT
#define MQTT_HOST IPAddress(xxx, xxx, xxx, xxx) // exemplo de ip separado por virgulas: 192, 168, 1, 18
#define MQTT_PORT 1883
#define BROKER_USER "user" //usuario do broker/usuario
#define BROKER_PASS "user01" //senha do usuario/obroker 

// Tópicos MQTT
#define MQTT_PUB_TEMP "esp/bme280/temperature"
#define MQTT_PUB_HUM "esp/bme280/humidity"
#define MQTT_PUB_PRES "esp/bme280/pressure"
#define MQTT_PUB_RSSI "esp/wifi/rssi" 

//Inicializa o sensor
Adafruit_BME280 bme;

float temp;
float hum;
float pres;

long rssi;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// Intervalos para publicações
unsigned long previousMillis = 0;  //temp e hum
unsigned long previousMillisRSSI = 0;  //rssi
const long interval = 10000;       //temp e hum
const long intervalRSSI = 11000;  //rssi

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0);
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) {
}

void publishRSSI() {
  rssi = WiFi.RSSI();

  uint16_t packetIdPubRSSI = mqttClient.publish(MQTT_PUB_RSSI, 1, true, String(rssi).c_str());
  Serial.printf("Publish acknowledged. | Publishing RSSI on topic %s at QoS 1, packetId: %i | Message: %ld \n", MQTT_PUB_RSSI, packetIdPubRSSI, rssi);

  if (rssi >= -50) {
    Serial.println("Sinal Wi-Fi: Ótimo");
  } else if (rssi < -50 && rssi >= -60) {
    Serial.println("Sinal Wi-Fi: Bom");
  } else if (rssi < -60 && rssi >= -70) {
    Serial.println("Sinal Wi-Fi: Médio");
  } else {
    Serial.println("Sinal Wi-Fi: Ruim");
  }

  Serial.print("RSSI: ");
  Serial.println(rssi);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialize BME280 sensor 
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(BROKER_USER, BROKER_PASS);

  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();

  // Publicação de temperatura, umidade e pressão a cada 10 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  
    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure() / 100.0F;

    // Publica temperatura via MQTT
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
    Serial.printf("Publish acknowledged. | Publishing on topic %s at QoS 1, packetId: %i | Message: %.2f \n", MQTT_PUB_TEMP, packetIdPub1, temp);

    // Publica umidade via MQTT
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());
    Serial.printf("Publish acknowledged. | Publishing on topic %s at QoS 1, packetId: %i | Message: %.2f \n", MQTT_PUB_HUM, packetIdPub2, hum);

    // Publica pressão via MQTT
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_PRES, 1, true, String(pres).c_str());
    Serial.printf("Publish acknowledged. | Publishing on topic %s at QoS 1, packetId: %i | Message: %.2f \n", MQTT_PUB_PRES, packetIdPub3, pres);
  }

  // Publica RSSI via MQTT
  if (currentMillis - previousMillisRSSI >= intervalRSSI) {
    previousMillisRSSI = currentMillis;
    publishRSSI();
  }
}

