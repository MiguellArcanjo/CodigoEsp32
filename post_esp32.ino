#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "brisa-3648202";
const char* password = "zlcvwjxv";
const int PINO_TRIG = 4; 
const int PINO_ECHO = 2; 

void setup() {
  Serial.begin(115200);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    digitalWrite(PINO_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PINO_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PINO_TRIG, LOW);

    long duracao = pulseIn(PINO_ECHO, HIGH);
    float distancia = (duracao * 0.0343) / 2;

    http.begin("https://apiesp32websocket.onrender.com/values"); 
    http.addHeader("Content-Type", "application/json");

    String postData;

    if (distancia < 15.00) {
      postData = "{\"comunicationValue\":true}";
    } else {
      postData = "{\"comunicationValue\":false}";
    }

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);
    } else {
      Serial.print("Erro na requisição HTTP: ");
      Serial.println(httpResponseCode);
    }

    Serial.print("Distância: ");
    Serial.print(distancia);
    Serial.println(" cm");

    http.end();
  }

  delay(1000); 
}
