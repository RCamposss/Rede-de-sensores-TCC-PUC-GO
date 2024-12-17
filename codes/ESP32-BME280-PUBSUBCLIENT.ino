//Programa ESP32 - Sensor BME280
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Credenciais Wi-Fi
#define WIFI_SSID "nome_da_rede"
#define WIFI_PASSWORD "senha_da_rede"

// Endereço do broker MQTT
#define MQTT_HOST IPAddress(xxx, xxx, xxx, xxx) //ip separado por virgulas -> exemplo: 192.168.0.18
#define MQTT_PORT 1883
#define BROKER_USER "user" //nome do broker/usuario
#define BROKER_PASS "user01" //senha do broker/usuario

// Tópicos MQTT
#define MQTT_PUB_TEMP "esp/bme280/temperature"
#define MQTT_PUB_HUM "esp/bme280/humidity"
#define MQTT_PUB_PRES "esp/bme280/pressure"
#define MQTT_PUB_RSSI "esp/wifi/rssi" 

// Inicializa o sensor
Adafruit_BME280 bme;

// Variáveis para armazenamento dos dados
float temp;
float hum;
float pres;
long rssi;

// Configuração do cliente Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Intervalos para publicações
unsigned long previousMillis = 0;  // Intervalo de publicação temp e hum
unsigned long previousMillisRSSI = 0;  // Intervalo de publicação rssi
const long interval = 10000;       // Intervalo para temp e hum
const long intervalRSSI = 11000;   // Intervalo para rssi

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectToMqtt() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client", BROKER_USER, BROKER_PASS)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void publishRSSI() {
  rssi = WiFi.RSSI();
  mqttClient.publish(MQTT_PUB_RSSI, String(rssi).c_str());

  Serial.print("Publishing RSSI: ");
  Serial.println(rssi);
  if (rssi >= -50) {
    Serial.println("Sinal Wi-Fi: Ótimo");
  } else if (rssi < -50 && rssi >= -60) {
    Serial.println("Sinal Wi-Fi: Bom");
  } else if (rssi < -60 && rssi >= -70) {
    Serial.println("Sinal Wi-Fi: Médio");
  } else {
    Serial.println("Sinal Wi-Fi: Ruim");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Inicializa o sensor BME280
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToMqtt();
}

void loop() {
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();

  unsigned long currentMillis = millis();

  // Publicação de temperatura, umidade e pressão a cada 10 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    temp = bme.readTemperature();
    hum = bme.readHumidity();
    pres = bme.readPressure() / 100.0F;

    mqttClient.publish(MQTT_PUB_TEMP, String(temp).c_str());
    Serial.printf("Publishing temperature: %.2f\n", temp);

    mqttClient.publish(MQTT_PUB_HUM, String(hum).c_str());
    Serial.printf("Publishing humidity: %.2f\n", hum);

    mqttClient.publish(MQTT_PUB_PRES, String(pres).c_str());
    Serial.printf("Publishing pressure: %.2f\n", pres);
  }

  // Publicação do RSSI a cada 11 segundos
  if (currentMillis - previousMillisRSSI >= intervalRSSI) {
    previousMillisRSSI = currentMillis;
    publishRSSI();
  }
}
