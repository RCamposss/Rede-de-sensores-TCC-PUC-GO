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

**Imagem do Fluxo** 
[FluxogramaARQUITETURA-DO-SISTEMA](https://github.com/user-attachments/assets/352e5bbc-8622-484d-b9ad-52ec4ad3e492)

---

## **Hardware Utilizado**

### **PC**
- Raspberry Pi 4 Model B

### **Microcontroladores**
- ESP8266 e ESP32
- Arduino UNO e MEGA 2560

### **Sensores**
- DHT11 e DHT22
- BME280
- LDR
- Sensor de Chuva (RAIN SENSOR)
- NTC

---

## **Topologia Estrela**
A topologia **estrela** foi escolhida devido a:
- Escalabilidade, permitindo a adção ou remoção de nós com facilidade;
- Ampliação do alcance de monitoramento;
- Praticidade na manutenção e redução de latência.

**Imagem da Topologia** *(Adicionar aqui)*

---

## **Diagramas de Sequência**
Os diagramas de sequência mostram a comunicação entre os dispositivos conectados aos **Arduinos** (funcionando como escravos) e outros microcontroladores independentes.

**Imagem dos Diagramas** *(Adicionar aqui)*

---

## **Exemplo de Nó no Node-RED**
A imagem abaixo ilustra o fluxo do **ESP32** com o sensor **BME280** enviando dados para o **InfluxDB**.

**Imagem do Nó** *(Adicionar aqui)*

---

## **Dashboard no Banco de Dados**
A imagem a seguir mostra o relatório gerado no **InfluxDB** com base nos dados recebidos do Node-RED.

**Imagem do Dashboard** *(Adicionar aqui)*

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

## **Vídeo - Apresentação do TCC**
A execução do projeto pode ser acessada no link a seguir:
[Apresentação do Projeto](https://drive.google.com/file/d/1AOXGchitQ8P0aQ9WHTfZ-_uaugCDxYjI/view?usp=sharing)
