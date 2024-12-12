#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "LAPTOP-26IO2ILJ 1105";
const char* password = "55[03s7E";

const char* serverUrlGet = "https://comedero-production.up.railway.app/api/datos";
const char* serverUrlPost = "https://comedero-production.up.railway.app/api/actualizar-estado";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void obtenerYEnviarHorarios() {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    
    if (http.begin(client, serverUrlGet)) {
        http.addHeader("Accept", "application/json");
        int httpCode = http.GET();
        
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);
            
            if (!error && doc.is<JsonArray>()) {
                JsonArray array = doc.as<JsonArray>();
                
                // Limpiar buffer serial
                while(Serial.available()) Serial.read();
                
                // Enviar cantidad de horarios
                Serial.write('H');
                delay(10);
                Serial.write(array.size());
                delay(10);
                
                // Enviar cada horario
                for (JsonVariant v : array) {
                    Serial.write((uint8_t)v["hora"].as<int>());
                    delay(10);
                    Serial.write((uint8_t)v["minuto"].as<int>());
                    delay(10);
                    uint16_t gramos = v["gramos"].as<int>();
                    Serial.write((uint8_t)(gramos >> 8));
                    delay(10);
                    Serial.write((uint8_t)(gramos & 0xFF));
                    delay(10);
                }
                Serial.flush(); // Asegurar que todos los datos se envíen
            }
        }
        http.end();
    }
}

void enviarDatosAlServidor(uint16_t pesoPlato, uint16_t pesoDispensador) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;
        
        if (http.begin(client, serverUrlPost)) {
            http.addHeader("Content-Type", "application/json");
            
            StaticJsonDocument<200> doc;
            doc["cantidadPlato"] = pesoPlato;
            doc["cantidadDispensador"] = pesoDispensador;
            
            String jsonString;
            serializeJson(doc, jsonString);
            
            http.POST(jsonString);
            http.end();
        }
    }
}

void loop() {
    // Verificar si hay solicitud de la EDU CIAA
    if (Serial.available()) {
        char header = Serial.read();
        
        if (header == 'R') {
            // Solicitud de horarios recibida
            obtenerYEnviarHorarios();
        }
        else if (header == 'P') {
            // Esperar a recibir los 4 bytes de pesos
            while (Serial.available() < 4) delay(1);
            
            // Leer peso plato (2 bytes)
            uint16_t pesoPlato = (Serial.read() << 8) | Serial.read();
            
            // Leer peso dispensador (2 bytes)
            uint16_t pesoDispensador = (Serial.read() << 8) | Serial.read();
            
            enviarDatosAlServidor(pesoPlato, pesoDispensador);
        }
    }
    
    delay(10); // Pequeña pausa para no saturar el procesador
}