#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your WiFi and server details
const char* ssid = "Subhash";
const char* password = "12345678";
const char* serverURL = "http://192.x.x.x:5000/api/data"; // Replace with your backend IP

unsigned long lastSendTime = 0;
const long interval = 30000;  // 30 seconds

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("WiFi Connected");
}

void loop() {
  if (millis() - lastSendTime > interval) {
    lastSendTime = millis();

    // Simulate values
    float voltage = random(210, 231);    // 210V to 230V
    float current = random(10, 51) / 100.0; // 0.1A to 0.5A
    float power = voltage * current;

    Serial.println("Sending data...");
    Serial.printf("Voltage: %.2f, Current: %.2f, Power: %.2f\n", voltage, current, power);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverURL);
      http.addHeader("Content-Type", "application/json");

      String jsonPayload = String("{\"deviceId\":\"fan001\",\"voltage\":") + voltage +
                           ",\"current\":" + current +
                           ",\"power\":" + power + "}";

      int httpResponseCode = http.POST(jsonPayload);
      Serial.printf("HTTP Response code: %d\n", httpResponseCode);
      http.end();
    }
  }
}
