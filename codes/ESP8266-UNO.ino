#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "nome_da_rede";
const char* password = "senha_da_rede";
const char* mqtt_server = "xxx.xxx.xxx.xxx";  // IP do broker MQTT
const int mqtt_port = 1883;                // Porta MQTT

const char* mqtt_user = "user";          // Usuário do broker MQTT
const char* mqtt_pass = "user01";      // Senha do broker MQTT

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);                     // Inicializa a serial
  setup_wifi();                           // Conecta ao WiFi
  client.setServer(mqtt_server, mqtt_port); // Configura o servidor MQTT
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
    reconnect();                          // Reconecta ao broker MQTT se necessário
  }
  client.loop();                          // Mantém a conexão com o broker ativa

  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');  // Lê até o final da linha
    parseAndPublish(data);                       // Processa e publica os dados
  }
}

void parseAndPublish(String data) {
  int tempIndex = data.indexOf("T:");            // Localiza "T:" para temperatura
  int humIndex = data.indexOf(",H:");            // Localiza ",H:" para umidade

  if (tempIndex != -1 && humIndex != -1) {
    String temperature = data.substring(tempIndex + 2, humIndex);
    String humidity = data.substring(humIndex + 3);

    // Imprime os dados recebidos no monitor serial para debug
    Serial.print("Temperatura recebida: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Umidade recebida: ");
    Serial.print(humidity);
    Serial.println("%");

    client.publish("arduino/esp/temp", (const uint8_t*)temperature.c_str(), temperature.length(), true); // QoS 1
    client.publish("arduino/esp/hum", (const uint8_t*)humidity.c_str(), humidity.length(), true);       // QoS 1
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println(" Conectado ao broker!");
      client.subscribe("arduino/#"); // Opcional: Inscrição em tópicos MQTT
    } else {
      Serial.print(" Falha na conexão. Código de erro: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 3 segundos...");
      delay(3000);
    }
  }
}

/*

client.state()

-1: Disconnected
0: Connection accepted
1: Connection refused, unacceptable protocol version
2: Connection refused, identifier rejected
3: Connection refused, broker unavailable
4: Connection refused, bad username or password
5: Connection refused, not authorized

*/