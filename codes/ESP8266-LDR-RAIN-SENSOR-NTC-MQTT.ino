#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configurações WiFi e MQTT
const char* ssid = "nome_da_rede";
const char* password = "senha_da_rede";
const char* mqtt_server = "xxx.xxx.xxx.xxx"; //IP EXEMPLO: 192.168.1.1

const int mqtt_port = 1883;

const char* mqtt_user = "usuario";
const char* mqtt_pass = "usuario01";

const char* topicLDR = "mega/ldr";   // Tópico para o LDR
const char* topicRain = "mega/sensor/rain"; // Tópico para o sensor de chuva
const char* topicTemp = "mega/ntc/temp"; // Tópico para o NTC (temperatura)

WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi() {
  delay(10);
  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi conectado.");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao broker MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("Conectado.");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);        // Comunicação Serial com o Arduino Mega
  setupWiFi();               // Configura a conexão Wi-Fi
  client.setServer(mqtt_server, mqtt_port); // Configura o broker MQTT
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {
    String message = Serial.readStringUntil('\n'); // Lê a mensagem enviada pelo Arduino Mega

    // Separar os valores individuais da mensagem
    int ldrIndex = message.indexOf("LDR: ");
    int rainIndex = message.indexOf(", Rain: ");
    int tempIndex = message.indexOf(", Temp: ");

    if (ldrIndex != -1 && rainIndex != -1 && tempIndex != -1) {
      // Extração das substrings
      String ldrValue = message.substring(ldrIndex + 5, rainIndex);
      String rainValue = message.substring(rainIndex + 7, tempIndex);
      String tempValue = message.substring(tempIndex + 7);

      // Ajustar espaços desnecessários (trim manual)
      ldrValue.trim();
      rainValue.trim();
      tempValue.trim();

      // Publicar os valores nos tópicos MQTT
      client.publish(topicLDR, ldrValue.c_str());
      client.publish(topicRain, rainValue.c_str());
      client.publish(topicTemp, tempValue.c_str());

      // Exibir no Monitor Serial
      Serial.println("Mensagem publicada no broker MQTT:");
      Serial.println("LDR: " + ldrValue);
      Serial.println("Rain: " + rainValue);
      Serial.println("Temp: " + tempValue);
    }
  }
}
