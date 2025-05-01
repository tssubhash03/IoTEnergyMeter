#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your hotspot or WiFi credentials
const char* ssid = "Subhash";
const char* password = "12345678";

// Replace with your server IP and port (e.g., PC running Flask server)
const char* serverIP = "192.168.33.234";
const int port = 5000;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String getUrl = "http://" + String(serverIP) + ":" + String(port) + "/get-command";
    
    http.begin(getUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      Serial.println("📥 Command from server: " + payload);

      if (payload.length() > 0) {
        int inputVal = payload.toInt();
        int sensorValue = inputVal + 1;

        // Step 2: Send sensor data
        HTTPClient postHttp;
        String postUrl = "http://" + String(serverIP) + ":" + String(port) + "/esp-data";
        postHttp.begin(postUrl);
        postHttp.addHeader("Content-Type", "application/json");

        String jsonData = "{\"sensorValue\":" + String(sensorValue) + "}";
        int postCode = postHttp.POST(jsonData);

        if (postCode > 0) {
          Serial.println("✅ Data posted: " + jsonData);
        } else {
          Serial.printf("❌ Failed to POST data: %s\n", postHttp.errorToString(postCode).c_str());
        }

        postHttp.end();

        // Step 3: Clear command
        HTTPClient clearHttp;
        String clearUrl = "http://" + String(serverIP) + ":" + String(port) + "/clear-command";
        clearHttp.begin(clearUrl);
        clearHttp.addHeader("Content-Type", "text/plain");
        int clearCode = clearHttp.POST(""); // No body

        if (clearCode > 0) {
          Serial.println("✅ Command cleared");
        } else {
          Serial.printf("❌ Failed to clear command: %s\n", clearHttp.errorToString(clearCode).c_str());
        }

        clearHttp.end();
      } else {
        Serial.println("⚠️ Empty command received.");
      }
    } else {
      Serial.printf("❌ GET failed: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("❌ Not connected to WiFi!");
  }

  delay(5000); // Run every 5 seconds
}
