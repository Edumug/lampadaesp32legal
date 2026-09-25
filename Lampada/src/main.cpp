#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "moto g34 5G_1686"
#define WIFI_PASSWORD "edumug12"

#define DATABASE_URL "https://lampadaesp32-default-rtdb.firebaseio.com"

#define RELE 2

bool estadoAnterior = false;

void setup() {
    Serial.begin(115200);

    pinMode(RELE, OUTPUT);

    // Relé desligado inicialmente
    digitalWrite(RELE, HIGH);

    Serial.println();
    Serial.println("Iniciando ESP32...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Conectando ao WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi desconectado!");
        delay(1000);
        return;
    }

    HTTPClient http;

    String url = String(DATABASE_URL) + "/lampada.json";

    http.begin(url);

    int codigo = http.GET();

    if (codigo > 0) {
        String resposta = http.getString();
        resposta.trim();

        Serial.print("Firebase: ");
        Serial.println(resposta);

        // Firebase true = lâmpada ligada
        // Firebase false = lâmpada desligada
        if (resposta == "true") {
            digitalWrite(RELE, LOW);

            if (!estadoAnterior) {
                Serial.println("COMANDO RECEBIDO: LIGAR");
            }

            estadoAnterior = true;
        }
        else if (resposta == "false") {
            digitalWrite(RELE, HIGH);

            if (estadoAnterior) {
                Serial.println("COMANDO RECEBIDO: DESLIGAR");
            }

            estadoAnterior = false;
        }
        else if (resposta == "null") {
            Serial.println("Aguardando comando no Firebase...");
        }
    }
    else {
        Serial.print("Erro HTTP: ");
        Serial.println(codigo);
    }

    http.end();

    // Evita consultas excessivamente rápidas
    delay(1000);
}