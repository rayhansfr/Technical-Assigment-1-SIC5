#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kazak";
const char* password = "kosong05";
const char* serverAddress = "http://192.168.0.102:5000/sensor";

const int DHT_PIN = 15;

DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
}

void loop() {
  
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  if (!isnan(data.temperature) && !isnan(data.humidity)) {
    HTTPClient http;

    http.begin(serverAddress);

    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"temperature\":" + String(data.temperature, 2) + ",\"humidity\":" + String(data.humidity, 1) + "}";
    Serial.println("Sending data: " + jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("data NaN");
  }
  delay(10000);
}
