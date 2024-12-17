//Programa ESP8266 - NTC 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <math.h>

// Configurações WiFi e MQTT
const char* ssid = " NOME DA REDE "; //exemplo: wifi_one
const char* password = " PASSWORD "; //exemplo: wifione002
const char* mqtt_server = " BROKER IP ";  // IP do broker MQTT, exemplo: 192.168.1.1
const int mqtt_port = 1883;                // Porta MQTT

const char* mqtt_user = "user_mosquitto";          // Usuário do broker MQTT
const char* mqtt_pass = "password1";      // Senha do broker MQTT

// Configuração do pino do NTC e do resistor
const int analogPin = A0;
const int resistorValue = 10000;
const float BETA = 3950;

// Tópicos MQTT
const char* MQTT_PUB_TEMP = "esp/ntc/temperatura";
const char* MQTT_PUB_RSSI = "esp/wifi/rssi";  

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  delay(10);
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi!");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura e publicação da temperatura do NTC
  float temperature = readNTCTemperature();
  publishTemperature(temperature);

  // Publicação do RSSI
  publishRSSI();

  delay(2000); 
}

float readNTCTemperature() {
  int sensorValue = analogRead(analogPin);
  float temperature = 0;

  if (sensorValue > 0) {
    float resistance = resistorValue * ((1023.0 / (float)sensorValue) - 1.0);

    temperature = 1.0 / (log(resistance / 10000.0) / BETA + 1.0 / (25.0 + 273.15));
    temperature -= 273.15;

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
  } else {
    Serial.println("Sensor reading is too low. Check connections.");
  }

  return temperature;
}

void publishTemperature(float temperature) {
  String temperatureStr = String(temperature, 2);
  client.publish(MQTT_PUB_TEMP, temperatureStr.c_str(), true);
}

void publishRSSI() {
  long rssi = WiFi.RSSI();
  String rssiStr = String(rssi);

  client.publish(MQTT_PUB_RSSI, rssiStr.c_str(), true);

  Serial.print("RSSI: ");
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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println(" Conectado ao broker!");
    } else {
      Serial.print(" Falha na conexão. Código de erro: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 3 segundos...");
      delay(3000);
    }
  }
}
