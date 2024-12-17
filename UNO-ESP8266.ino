#include <DHT.h>

#define DHTPIN 2          // Pino do sensor DHT11
#define DHTTYPE DHT11     // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);     // Inicializa a comunicação serial com o ESP8266
  dht.begin();              // Inicializa o sensor DHT
  Serial.println("Sensor DHT11 iniciado!"); // Mensagem de inicialização 
}

void loop() {
  delay(5000);              // Aumente o tempo entre as leituras para 5 segundos

  float h = dht.readHumidity();  // Lê a umidade
  float t = dht.readTemperature(); // Lê a temperatura

  // Verifica se a leitura falhou
  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }

  // Imprime as leituras no monitor serial para debug
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" *C");

  // Envia os dados para o ESP8266 no formato esperado
  Serial.print("T:");
  Serial.print(t);
  Serial.print(",H:");
  Serial.println(h);

  delay(5000);              // Delay após enviar os dados
}

// de 10 em 10 segundos está sendo feita a leitura da temperatura e umidade
