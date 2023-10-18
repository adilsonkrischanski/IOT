#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define LED_BUILTIN 2

class WebServerHandler {

    private:
        const char *ssid;
        const char *password;
        WebServer server{80};


    public:
        WebServerHandler(const char *ssid, const char *password) : ssid(ssid), password(password) {}

        void begin() {
            pinMode(LED_BUILTIN, OUTPUT);

            Serial.begin(115200);
            WiFi.softAP(ssid, password);
            IPAddress myIP = WiFi.softAPIP();
            Serial.print("SSID: ");
            Serial.println(ssid);
            Serial.print("AP IP address: ");
            Serial.println(myIP);

            server.begin();

            Serial.println("Servidor Iniciado");

            if (MDNS.begin("esp32")) {
                Serial.println("MDNS responder started");
            }

            server.on("/", std::bind(&WebServerHandler::handleRoot, this));
            server.on("/on", std::bind(&WebServerHandler::handleOn, this));
            server.on("/off", std::bind(&WebServerHandler::handleOff, this));
            server.onNotFound(std::bind(&WebServerHandler::handleNotFound, this));

            server.begin();
            Serial.println("HTTP server started");
        }

        void handleRoot() {
            String html = "";
            html += "<style>a {font-size:40px; background-color:#CCCCCC;}</style>";
            html += "<meta charset='UTF-8'>";
            html += "<h1>Exemplo SoftAP IFRJ campus Niterói</h1>";
            html += "Clique <a href=\"/on\">aqui</a> para ligar o LED.<br><br><br>";
            html += "Clique <a href=\"/off\">aqui</a></h2> para desligar o LED.";
            html += "<h3>Autor: Luiz Oliveira</h3>";
            server.send(200, "text/html", html);
        }

        void handleOn() {
            digitalWrite(LED_BUILTIN, HIGH);
            handleRoot();
        }

        void handleOff() {
            digitalWrite(LED_BUILTIN, LOW);
            handleRoot();
        }

        void handleNotFound() {
            digitalWrite(LED_BUILTIN, HIGH);
            String message = "File Not Found\n\n";
            message += "URI: ";
            message += server.uri();
            message += "\nMethod: ";
            message += (server.method() == HTTP_GET) ? "GET" : "POST";
            message += "\nArguments: ";
            message += server.args();
            message += "\n";
            for (uint8_t i = 0; i < server.args(); i++) {
                message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
            }
            server.send(404, "text/plain", message);
            digitalWrite(LED_BUILTIN, LOW);
        }

        void handleClient() {
            server.handleClient();
            delay(2);
        }


};

// const char *ssid = "SMART LOCK";
// const char *password = "Password";

// WebServerHandler webServer(ssid, password);

// void setup() {
//     webServer.begin();
// }

// void loop() {
//     webServer.handleClient();
// }
