#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "moto g34 5G_1686"
#define WIFI_PASSWORD "edumug12"

#define DATABASE_URL "https://lampadaesp32-default-rtdb.firebaseio.com"

#define RELE 2

void setup() {
    Serial.begin(115200);

    pinMode(RELE, OUTPUT);

    // Relé desligado    
    digitalWrite(RELE, HIGH);

    Serial.println();
    Serial.println("Iniciando ESP32...");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Conectando ao WiFi");

    while (WiFi.status() != WL_CONNECTED) {
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
        return;
    }

    HTTPClient http;

    String url = String(DATABASE_URL) + "/lampada.json";

    Serial.print("Consultando: ");
    Serial.println(url);

    http.begin(url);

    int codigo = http.GET();

    if (codigo > 0) {

        Serial.print("HTTP: ");
        Serial.println(codigo);

        String resposta = http.getString();

        resposta.trim();

        Serial.print("Firebase: ");
        Serial.println(resposta);

        if (resposta == "false") {

            digitalWrite(RELE, LOW);

            Serial.println("💡 LAMPADA LIGADA");

        } 
        else if (resposta == "true") {

            digitalWrite(RELE, HIGH);

            Serial.println("⚫ LAMPADA DESLIGADA");

        }
        else if (resposta == "null") {

            Serial.println("Firebase ainda nao possui /lampada/ligada");

        }

    } 
    else {

        Serial.print("Erro HTTP: ");
        Serial.println(codigo);

    }

    http.end();

}