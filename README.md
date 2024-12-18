# **Desenvolvimento de uma Rede de Sensores para Monitoramento Ambiental**

## **Autor**: Rafael Campos Almeida  
## **Orientador**: Prof. Dr. Talles Marcelo Gonçalves de Andrade Barbosa  
## **Universidade**: Pontifícia Universidade Católica de Goiás  
## **Trabalho de Conclusão de Curso**

---

## **Descrição do Projeto**
O presente trabalho tem como desafio a implementação de uma solução tecnológica que reduza a **complexidade** e os **custos** na criação de uma rede de sensores voltada ao monitoramento ambiental. A solução possibilita:
- Escalabilidade de dispositivos heterogêneos;
- Monitoramento remoto em tempo real;  
- Adaptabilidade em diversos cenários.

---

## **Ferramentas Utilizadas**

### **Arduino IDE**
- Editor de código, compilador integrado e suporte para upload direto em microcontroladores. Dispõe de um vasto acervo de bibliotecas que facilita a integração com sensores e microcontroladores.

### **Protocolo MQTT**
- Meio de comunicação pelo qual os componentes trocam informações entre si.

### **Node-RED**
- Plataforma **open-source** que possibilita a criação de fluxos de dados através de nós que recebem, processam e transmitem informações. Sua flexibilidade permite a adção e controle de dispositivos heterogêneos de forma prática.

### **InfluxDB**
- Banco de dados **open-source** especializado em séries temporais, projetado para lidar com altos volumes de dados em tempo real. Sua integração com o Node-RED viabiliza o monitoramento e a geração de relatórios com os dados coletados.

### **Cloudflare**
- Permite o acesso remoto seguro ao projeto, protegendo e criptografando o tráfego e eliminando a necessidade de portas públicas.

---

## **Comportamento do Fluxo do Sistema**

1. **Leitura e encaminhamento dos dados**: Sensores conectados aos microcontroladores realizam a leitura do ambiente.
2. **Envio dos dados para o servidor**: Os microcontroladores encaminham os dados via protocolo MQTT.
3. **Servidor (Broker MQTT)**: Responsável por receber os dados encaminhados.
4. **Organização de nós na rede**: O Node-RED gerencia a adção, exclusão e modificação dos nós do sistema.
5. **Armazenamento e amostragem dos dados**: Os dados são armazenados no **InfluxDB**, possibilitando consultas baseadas no histórico.
 
[Imagem do comportamento do fluxo de dados](https://github.com/user-attachments/assets/352e5bbc-8622-484d-b9ad-52ec4ad3e492)

---

## **Hardware**

### **PC**
- Raspberry Pi 4 Model B.

### **Microcontroladores**
- ESP8266 e ESP32;
- Arduino UNO e MEGA 2560.

### **Sensores**
- DHT11 e DHT22;
- BME280;
- LDR;
- Sensor de Chuva;
- NTC.

---

## **Topologia Estrela**

A topologia **estrela** foi escolhida devido a:
- Escalabilidade, permitindo a adção ou remoção de nós com facilidade;
- Ampliação do alcance de monitoramento;
- Praticidade na manutenção e redução de latência entre os dispositivos.

[ilustração da Topologia Estrela no projeto](https://github.com/user-attachments/assets/995c1783-eeab-4f9d-982f-daf1972c5bf7)

---

## **Diagramas de Sequência**
Os diagramas de sequência mostram dois tipos de comunicação: os dispositivos conectados aos **Arduinos** (funcionando como escravos) e os microcontroladores independentes.
**0bservação: somente os dois diagramas, um para cada tipo de comunicação.

[Diagrama de sequencia com dispositivos dependentes](https://github.com/user-attachments/assets/30f69daa-9fe1-41c7-af6f-e77ece2f3546)

[Diagrama de sequencia com dispositivo independente](https://github.com/user-attachments/assets/13f4ac0f-50b0-4414-9d64-fc0d460b47e4)

---

## **Exemplo de Nó no Node-RED**
A imagem abaixo ilustra o fluxo do **ESP32** com o sensor **BME280** enviando dados para o **InfluxDB**.

[Node-RED-fluxo-exemplo](https://github.com/user-attachments/assets/369123d3-0cca-4714-9390-7e1767309313)

---

## **Dashboard no Banco de Dados**
A imagem a seguir mostra o relatório gerado no **InfluxDB** com base nos dados recebidos do Node-RED.

[dados-relatorio-influxdb](https://github.com/user-attachments/assets/695d3fff-3f4a-4dff-b282-aadd20909dfc)

---

## **Licenciamento**
Este projeto utiliza bibliotecas e tecnologias **open-source** listadas abaixo, aplicadas para fins educacionais:

### **Linguagem de Programação**
- C Embedded

---

## **Ferramentas**
- Node-RED
- InfluxDB
- Cloudflare
- Arduino IDE

---

## **Bibliotecas Utilizadas**
- **Bibliotecas assíncronas**:
   - [ESPAsyncTCP (ESP8266)](https://github.com/me-no-dev/ESPAsyncTCP)
   - [AsyncTCP (ESP32)](https://github.com/me-no-dev/AsyncTCP)

- **Biblioteca síncrona**:
   - [PubSubClient (ESP8266, Arduino UNO e MEGA 2560)](https://github.com/knolleary/pubsubclient)

---

## **Trabalhos futuros**

Como sugestão para trabalhos futuros, propõe-se:
- Integração à Energy Harvesting: adaptação do sistema para o aproveitamento de fontes de energia renovável, como solar, eólica ou vibrações mecânicas, com o objetivo de alimentar dispositivos IoT;
- Otimização de alcance: implementar protocolos de comunicação de baixa energia e alta eficiência, como Long-Range(LoRa);
- Integração com Inteligência Artificial: incorporar algoritmos de aprendizado de máquina para melhorar a análise dos dados coletados, possibilitando previsões mais precisas e decisões mais assertivas.

---

## **Vídeo - Apresentação do TCC**
O exemplo de execução remota do projeto pode ser acessado no link a seguir:
[Apresentação do Projeto](https://drive.google.com/file/d/1AOXGchitQ8P0aQ9WHTfZ-_uaugCDxYjI/view?usp=sharing)

---

## **Slides MS Powerpoint - Apresentação do TCC**
Os slides usados na apresentação estão nos seguintes links:
- [Apresentação TCC - 13-12-2024.pdf](https://github.com/user-attachments/files/18183626/Apresentacao.TCC.-.13-12-2024.pdf)
- [Google Drive(com vídeos da implementação)](https://docs.google.com/presentation/d/1OoiNBTO4xguILLfA39K5EdtBodRy1bvH/edit?usp=sharing&ouid=118365570367346542020&rtpof=true&sd=true)
