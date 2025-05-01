#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your WiFi credentials
const char* ssid = "xxxxxxx";  // change to your WiFi SSID
const char* password = "12345678";

// Your backend API endpoint
const char* serverName = "http://192.x.x.x:5000/api/data";  // change to your backend IP

// Deviation thresholds
const float VOLTAGE_THRESHOLD = 10.0;
const float CURRENT_THRESHOLD = 0.2;

// Interval settings
const unsigned long INTERVAL = 30000;  // 30 seconds
unsigned long lastSendTime = 0;

// Previous sensor values
float prevVoltage = 0;
float prevCurrent = 0;

// Mock sensor data (replace with real sensor functions later)
float readVoltage() {
  return random(210, 230);  // Simulate voltage fluctuation
}
float readCurrent() {
  return random(10, 50) / 100.0;  // Simulate current between 0.1 - 0.5 A
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void sendDataToServer(float voltage, float current, float power) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{";
    jsonData += "\"deviceId\":\"fan001\",";
    jsonData += "\"voltage\":" + String(voltage) + ",";
    jsonData += "\"current\":" + String(current) + ",";
    jsonData += "\"power\":" + String(power);
    jsonData += "}";

    int httpResponseCode = http.POST(jsonData);
    Serial.println("Sending data...");
    Serial.println(jsonData);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    http.end();
  } else {
    Serial.println("WiFi not connected!");
  }
}
int loopCounter = 0;

void loop() {
  float voltage;
  float current;

  // Simulate normal readings
  voltage = random(215, 230);
  current = random(20, 40) / 100.0; // 0.20 to 0.40 A

  // Inject deviation every 5 intervals (not every 5 seconds)
  if ((loopCounter > 0) && (loopCounter % 5 == 0)) {
    voltage = 190; // Sudden drop
    current = 0.10;
    Serial.println("⚠️ Injecting voltage/current deviation!");
  }

  float power = voltage * current;

  unsigned long currentTime = millis();

  bool isTimeToSend = (currentTime - lastSendTime >= INTERVAL);
  bool isDeviation = abs(voltage - prevVoltage) > VOLTAGE_THRESHOLD ||
                     abs(current - prevCurrent) > CURRENT_THRESHOLD;

  if (isTimeToSend || isDeviation) {
    sendDataToServer(voltage, current, power);
    lastSendTime = currentTime;  // Reset timer after deviation too

    prevVoltage = voltage;
    prevCurrent = current;

    loopCounter++;  // Only increase when we actually send
  }

  delay(1000);  // check every second
}
