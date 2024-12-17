//códigos
#include <math.h>

// Configuração dos Sensores
const int rainSensorPin = A0;     // Pino analógico do sensor de chuva HW-028
const int ldrPin = A1;            // Pino analógico do LDR
const int ntcPin = A2;            // Pino analógico do NTC 10k

// Configuração do NTC
const int resistorValue = 4700;  // Resistor fixo de 4.7kΩ no divisor de tensão
const float BETA = 3950;          // Coeficiente Beta do NTC
const float RREF = 10000.0;       // Resistência do NTC a 25 °C (10kΩ)

void setup() {
  Serial.begin(115200);   // Comunicação Serial com o PC para depuração
}

void loop() {
  // Leitura do Sensor de Chuva
  int rainValue = analogRead(rainSensorPin);

  // Leitura do LDR
  int ldrValue = analogRead(ldrPin);

  // Leitura do NTC
  int ntcValue = analogRead(ntcPin);
  float temperature = 0;
  if (ntcValue > 0) {
    // Calcular a resistência do NTC
    float resistance = (1023.0 / (float)ntcValue - 1.0) * resistorValue;

    // Equação de Steinhart-Hart para calcular a temperatura
    temperature = 1.0 / (log(resistance / RREF) / BETA + 1.0 / (25.0 + 273.15));
    temperature -= 273.15; // Converter de Kelvin para Celsius
  } else {
    Serial.println("Erro na leitura do NTC. Verifique as conexões.");
  }

  // Construir a mensagem para enviar ao ESP8266
  String message = String("LDR: ") + ldrValue + 
                   ", Rain: " + rainValue + 
                   ", Temp: " + temperature;

  //M anda mensagem para o ESP8266
  Serial.println(message);

  // 2 segundos para proxima leitura
  delay(2000);
}


//SENSOR DE CHUVA:
// 0: Indica que há muita água ou umidade na placa (alta condutividade elétrica).
// 1023: Indica que a placa está completamente seca (baixa condutividade elétrica).


//LDR
//800 e 1023: Há luz incidente no sensor
//0 e 300: há pouca ou nenhuma luz incidente no sensor


