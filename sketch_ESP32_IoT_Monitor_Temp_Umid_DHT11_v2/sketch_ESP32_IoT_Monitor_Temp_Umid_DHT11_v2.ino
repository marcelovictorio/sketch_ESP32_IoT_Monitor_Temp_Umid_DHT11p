/*
* Código gerado pelo Chat GPT e refinado conforme as necessidades do projeto em 04/12/2025 
https://github.com/marcelovictorio @MarceloVictorio YouTube
🔷ESP32 IoT – Monitor de Temperatura e Umidade.
        ┌──────────────┐
        │   DHT11      │
        │ (Sensor)     │
        └──────┬───────┘
               │
               │ Dados (Temp / Umidade)
               ▼
        ┌──────────────────────┐
        │        ESP32         │
        │----------------------│
        │ • Leitura do sensor  │
        │ • Processamento      │
        │ • Servidor Web       │
        │ • Controle OLED      │
        └──────┬────────┬──────┘
               │        │
       I2C     │        │ Wi-Fi
               ▼        ▼
        ┌──────────┐   ┌───────────────────┐
        │   OLED   │   │ Navegador Web     │
        │ 128x64   │   │ (Celular / PC)    │
        │          │   │ Access Point (AP) │
        └──────────┘   └───────────────────┘

*/
/*************************************************************
 * ESP32 - Monitor Ambiental IoT
 * MODO: ACCESS POINT
 * SENSOR: DHT11
 * DISPLAY: OLED 128x64 SSD1306
 * FORMATAÇÃO: 2 inteiros + 2 casas decimais
 *************************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/* ==================== ACCESS POINT ==================== */
const char* ssid = "ESP32-DHT11";
const char* password = "12345678";

/* ==================== DHT ==================== */
#define DHTPIN  27
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);

/* ==================== OLED ==================== */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ==================== WEB SERVER ==================== */
WebServer server(80);

/* ==================== CONTROLE ==================== */
uint32_t delayMS;
unsigned long bootMillis;
bool showIP = true;

/* ======================================================
   FUNÇÃO: FORMATAÇÃO SEGURA (ANTI NaN)
   ====================================================== */
String formatValue(float value, const char* unit) {
  if (isnan(value)) {
    return "--.-- " + String(unit);
  } else {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%05.2f", value);
    return String(buffer) + " " + String(unit);
  }
}

/* ======================================================
   PÁGINA HTML
   ====================================================== */
void handleRoot() {

  sensors_event_t tEvent, hEvent;
  dht.temperature().getEvent(&tEvent);
  dht.humidity().getEvent(&hEvent);

  String tempStr = formatValue(tEvent.temperature, "°C");
  String humStr  = formatValue(hEvent.relative_humidity, "%");

  String html = "<!DOCTYPE html><html lang='pt-br'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 - Monitor Ambiental</title>";
  html += "<style>";
  html += "body{font-family:Arial;background:#111;color:#0f0;text-align:center;}";
  html += "h1{color:#0ff}";
  html += ".box{border:2px solid #0f0;border-radius:12px;padding:20px;margin:20px}";
  html += "</style></head><body>";

  html += "<h1>ESP32 - Monitor Ambiental</h1>";
  html += "<div class='box'>";
  html += "<p><b>Temperatura:</b> " + tempStr + "</p>";
  html += "<p><b>Umidade:</b> " + humStr + "</p>";
  html += "</div>";
  html += "<p>Atualização automática a cada 5 segundos</p>";

  html += "</body></html>";

  server.send(200, "text/html", html);
}

/* ======================================================
   SETUP
   ====================================================== */
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // DHT
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  // ACCESS POINT
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  IPAddress ip = WiFi.softAPIP();

  Serial.print("AP IP: ");
  Serial.println(ip);

  // OLED – mostra IP por 5s
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Modo Access Point");
  display.println(ssid);
  display.setCursor(0, 32);
  display.print("IP: ");
  display.println(ip);
  display.display();

  bootMillis = millis();

  // Web Server
  server.on("/", handleRoot);
  server.begin();
}

/* ======================================================
   LOOP
   ====================================================== */
void loop() {

  server.handleClient();

  // Exibe IP apenas nos primeiros 5 segundos
  if (showIP && millis() - bootMillis < 5000) {
    return;
  } else {
    showIP = false;
  }

  sensors_event_t tEvent, hEvent;
  dht.temperature().getEvent(&tEvent);
  dht.humidity().getEvent(&hEvent);

  String tempOLED = formatValue(tEvent.temperature, "C");
  String humOLED  = formatValue(hEvent.relative_humidity, "%");

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DHT11 - AP Mode");

  display.setTextSize(2);
  display.setCursor(0, 16);
  display.println(tempOLED);

  display.setCursor(0, 40);
  display.println(humOLED);

  display.display();

  delay(delayMS);
}
