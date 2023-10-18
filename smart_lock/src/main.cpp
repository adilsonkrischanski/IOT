#include <WiFi.h>
#include <PubSubClient.h>

// Informações de conexão Wi-Fi
const char* ssid = "Krischanski 2.4G";
const char* password = "krischanski@1503";

// Informações de conexão HiveMQ
const char* mqttServer = "ccba0f6a0c6564551bf8999b8921ff3c5.s1.eu.hivemq.cloud:8883/mqtt";
const int mqttPort = 8883;
const char* mqttUser = "AdilsonTeste";
const char* mqttPassword = "Udesc@123";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // Manipule as mensagens recebidas, se necessário
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }

  Serial.println("Conectado ao Wi-Fi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Conectando ao HiveMQ...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Conectado ao HiveMQ");
    } else {
      Serial.print("Falha na conexão. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }

  // Publica a palavra "alface" no tópico desejado
  client.publish("seutopico", "alface");
}

void loop() {
  client.loop();
}
