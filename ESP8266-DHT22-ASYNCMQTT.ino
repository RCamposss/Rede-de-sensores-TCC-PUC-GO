#include "DHT.h"
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

// Credenciais Wi-Fi
#define WIFI_SSID "nome_da_rede"
#define WIFI_PASSWORD "senha_da_rede"

// Endereço do broker MQTT
#define MQTT_HOST IPAddress(xxx, xxx, xxx, xxx) //192, 168, 1, 1 como exemplo
#define MQTT_PORT 1883
#define BROKER_USER "user"
#define BROKER_PASS "user01"

// Tópicos MQTT
#define MQTT_PUB_TEMP "esp/dht/temperature"
#define MQTT_PUB_HUM "esp/dht/humidity"
#define MQTT_PUB_RSSI "esp/wifi/rssi" 

// Pino conectado ao sensor DHT
#define DHTPIN 13  
#define DHTTYPE DHT22  

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Variáveis para leitura de sensor
float temp;
float hum;

// Variável para força do sinal Wi-Fi (RSSI)
long rssi;

// Intervalos para publicações
unsigned long previousMillis = 0;  // Tempo da última publicação de temperatura/umidade
unsigned long previousMillisRSSI = 0;  // Tempo da última publicação de força do sinal Wi-Fi
const long interval = 10000;       // Intervalo para publicações de temperatura/umidade
const long intervalRSSI = 11000;        // Intervalo para publicações de força do sinal Wi-Fi

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

// Função para conectar ao Wi-Fi
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Função chamada ao conectar ao Wi-Fi
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi.");
  connectToMqtt();
}

// Função chamada ao desconectar do Wi-Fi
void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); 
  wifiReconnectTimer.once(2, connectToWifi);
}

// Função para conectar ao MQTT
void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

// Função chamada ao conectar ao MQTT
void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

// Função chamada ao desconectar do MQTT
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

//Função chamada quando ocorre uma ou mais publicações no broker
void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

// Função para publicar valores de RSSI via MQTT
void publishRSSI() {
  rssi = WiFi.RSSI();  // Obtém o valor do RSSI

  // Publica o valor de RSSI via MQTT
  uint16_t packetIdPubRSSI = mqttClient.publish(MQTT_PUB_RSSI, 1, true, String(rssi).c_str());
  Serial.printf("Publishing RSSI on topic %s at QoS 1, packetId: %i | Message: %ld \n", MQTT_PUB_RSSI, packetIdPubRSSI, rssi);

  // Exibe a classificação da força do sinal
  if (rssi >= -50) {
    Serial.println("Sinal Wi-Fi: Ótimo");
  } else if (rssi < -50 && rssi >= -60) {
    Serial.println("Sinal Wi-Fi: Bom");
  } else if (rssi < -60 && rssi >= -70) {
    Serial.println("Sinal Wi-Fi: Médio");
  } else {
    Serial.println("Sinal Wi-Fi: Ruim");
  }

  // Exibe o valor do RSSI
  Serial.print("RSSI: ");
  Serial.println(rssi);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Configura os eventos de conexão Wi-Fi
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  // Configura as funções de conexão MQTT
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCredentials(BROKER_USER, BROKER_PASS);
  
  connectToWifi();  // Inicia a conexão Wi-Fi
}

void loop() {
  unsigned long currentMillis = millis();

  // Publicação de temperatura e umidade a cada 10 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    hum = dht.readHumidity();  // Leitura de umidade
    temp = dht.readTemperature();  // Leitura de temperatura;
  
    // Publica temperatura via MQTT
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf(" | Message: %.2f \n", temp);

    // Publica umidade via MQTT
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", MQTT_PUB_HUM, packetIdPub2);
    Serial.printf(" | Message: %.2f \n", hum);

  }

  // Publicação de RSSI a cada 11 segundos
  if (currentMillis - previousMillisRSSI >= intervalRSSI) {
    previousMillisRSSI = currentMillis;
    publishRSSI();
  }
}
