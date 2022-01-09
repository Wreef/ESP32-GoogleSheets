![logo](https://i.ibb.co/YthtbLh/Giifff-mid.gif)
***
# Datalogger com Google Sheets: ESP32
Neste guia, iremos montar um datalogger usando uma planilha do Google como banco de dados.
***
## Contextualização
Iremos utilizar um sensor DHT para monitorar temperatura e umidade do ar. Os dados serão enviados, em formato de planilha, para o Google Drive. Os dados serão atualizados automaticamente, permitindo, além do armazenamento, a visualização dos dados em tempo real.

## Requerimentos
- Placa ESP32;
- Sensor DHT;
- Resistor de 10 kOhm;
- Jumpers para conexão de componentes.

## Biblioteca do Sensor DHT
Para utilizar o sensor DHT é necessário instalar bibliotecas.

Em sua Arduino IDE vá em: Sketch > Incluir Biblioteca > Gerenciar Bibliotecas

<p align="center">
  <img src="https://i.ibb.co/xC958xP/preparando-5.png" alt="5"/>
</p>

Pesquise por "DHT", escolha a opção da imagem e clique em "Instalar" (como eu já tinha instalado, não apareceu a opção "Instalar").

<p align="center">
  <img src="https://i.ibb.co/xMxNdq7/dht.png" alt="5"/>
</p>

Pesquise por "Adafruit Unified Sensor", escolha a opção da imagem e clique em "Instalar" (como eu já tinha instalado, não apareceu a opção "Instalar").

<p align="center">
  <img src="https://i.ibb.co/v35tF0h/dh2.png" alt="5"/>
</p>

## Google Sheets
Inicialmente é necessário criar uma planilha no Google Drive.

<p align="center">
  <img src="https://i.ibb.co/yk4G1P8/googlesheet1.png" alt="1"/>
</p>

Vá em: Extensões > Apps Script

<p align="center">
  <img src="https://i.ibb.co/PNZzZKV/googlesheet3.png" alt="1"/>
</p>

Mude para o Editor Legado.

<p align="center">
  <img src="https://i.ibb.co/DGfDg1M/googlesheet4.png" alt="1"/>
</p>

Dê um nome para o projeto.

<p align="center">
  <img src="https://i.ibb.co/mSqSspB/googlesheet5.png" alt="1"/>
</p>

Confirme.

<p align="center">
  <img src="https://i.ibb.co/XDW3wzw/googlesheet6.png" alt="1"/>
</p>

Vá em: Publicar > Implantar com aplicativo da web

<p align="center">
  <img src="https://i.ibb.co/Lhp2kyB/googlesheet7.png" alt="1"/>
</p>

Altere para "Anyone, even anonymous" e clique em "Deploy".

<p align="center">
  <img src="https://i.ibb.co/JKz8QM7/googlesheet8.png" alt="1"/>
</p>

Clique em "Analisar permissões" e acesse seu E-mail.

<p align="center">
  <img src="https://i.ibb.co/0JnyHjK/googlesheet9.png" alt="1"/>
</p>

Clique em "Avançado" e acesse seu projeto.

<p align="center">
  <img src="https://i.ibb.co/WG58WDf/googlesheet11.png" alt="1"/>
</p>

Clique em "Permitir".

<p align="center">
  <img src="https://i.ibb.co/28zN8NQ/googlesheet12.png" alt="1"/>
</p>

Copie e guarde o código do seu projeto. Iremos utilizar mais tarde.

<p align="center">
  <img src="https://i.ibb.co/xYs27L2/googlesheet13.png" alt="1"/>
</p>

## Código do ESP32

```cpp
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
```

## Entendendo o Código
Inicialmente importamos as bibliotecas necessárias.

```cpp
#include <WiFi.h> // Wifi
#include "HTTPClient.h" // Comunicação com a Internet
#include <DHT.h> // Sensor DHT
#include <Wire.h> // Sensor DHT
```

Em seguida é necessário definir o nome (WIFI_SSID) e a senha (WIFI_PASSWORD) da sua rede WiFi.

Cole o link do seu projeto na variável "ID_GOOGLE_SHEET".

A parte final do escopo é a definição do pino do que o sensor DHT estará conectado.

```cpp
#define WIFI_SSID "SUA REDE WI-FI" // Nome da sua rede Wi-Fi
#define WIFI_PASSWORD "SENHA  DA SUA REDE WI-FI" // Senha da sua rede Wi-Fi

#define ID_GOOGLE_SHEET "LINK DA SUA PLANILHA" // Link do seu projeto do Google Sheets

#define DHT_PIN 14 // Pino do Sensor DHT
DHT dht(DHT_PIN, DHT11); // Sensor DHT
```

Na função setup iremos conectar na rede WiFi e iniciar o sensor DHT e o Serial.

```cpp
void setup() {
  
  Serial.begin(115200); // Serial
  dht.begin(); // Sensor DHT
  delay(500);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // WiFi
  Serial.print("Conectando");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    delay(500);
  }

  Serial.println("Conectado:");
  Serial.println(WiFi.localIP());
  delay(500);
}
```

Na função loop iremos criar as variáveis para Temperatura (vfTemp), Umidade do ar (viUmi) e para o link do request (url).

Na parte final da função loop é realizado o request para enviar as informações medidas.

```cpp
void loop() {
  
  float vfTemp = dht.readTemperature(); // Temperatura
  int viUmi = dht.readHumidity(); // Umidade do Ar
  String url = String(ID_GOOGLE_SHEET) + "?" + "Temperatura=" + vfTemp + "&Umidade=" + viUmi; // Link do Request
  
  Serial.println("Realizando o envio...");
  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();  
  http.end();
  Serial.println("Concluído!");
  delay(15000);
}
```

## Preparando a IDE do Arduino para utilizar o ESP32.
Em sua Arduino IDE vá em: Arquivo > Preferências

<p align="center">
  <img src="https://i.ibb.co/pfNTZ7p/preparando-1.png" alt="1"/>
</p>

Em "URLs Adicionais para Gerenciadores de Placas" escreva: https://dl.espressif.com/dl/package_esp32_index.json

Dê "OK" para salvar.

<p align="center">
  <img src="https://i.ibb.co/qydsfk8/esp32-preg.png" alt="1"/>
</p>

Vá em: Ferramentas > Placa > ESP32 Arduino > DOIT ESP32 DEVKIT v1

<p align="center">
  <img src="https://i.ibb.co/jMtdJjk/esp32222.png" alt="1"/>
</p>

Agora é só enviar o código!

## Conclusão
Esse tipo de projeto é bastante útil quando o se utiliza o ESP32 em monitoramento. Além de sergratuito o Google Sheet suporta, aproximadamente, 330 mil linhas. Dependendo do intervalo de envio a planilha pode durar bastante.

# Obrigado(a)!
<p align="center">
  <img src="https://i.ibb.co/NsfrRKy/ezgif-com-gif-maker.gif" alt="12"/>
</p>
