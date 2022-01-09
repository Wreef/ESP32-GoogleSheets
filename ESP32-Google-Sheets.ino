#include <WiFi.h>
#include "HTTPClient.h"
#include <DHT.h>
#include <Wire.h>

#define WIFI_SSID "SUA REDE WI-FI"
#define WIFI_PASSWORD "SENHA  DA SUA REDE WI-FI"

#define ID_GOOGLE_SHEET "LINK DA SUA PLANILHA"

#define DHT_PIN 14
DHT dht(DHT_PIN, DHT11);


void setup() {
  
  Serial.begin(115200); 
  dht.begin();
  delay(500);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
  }

  Serial.println("Conectado:");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  
  float vfTemp = dht.readTemperature();
  int viUmi = dht.readHumidity();
  String url = String(ID_GOOGLE_SHEET) + "?" + "Temperatura=" + vfTemp + "&Umidade=" + viUmi;
  
  Serial.println("Realizando o envio...");
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();  
  http.end();
  Serial.println("Concluído!");
  delay(15000);
}
