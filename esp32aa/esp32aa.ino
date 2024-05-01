#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Saumya";
const char* password = "saumya@@";
const char* APIA = "4626QS93Q1TYN7ER";
const char* APIB = "NT2E5L8VI7HO3LBC";
const char* server = "https://api.thingspeak.com/update"; 

void setup() {
  Serial.begin(115200);
  connectWiFi(); // Connect to WiFi
}

void loop() {
  if (Serial.available() >= 8) { // Ensure at least 8 characters are available (for three integers and two spaces)
    int countA, countB, lightLevel;

    // Read incoming data from Arduino
    lightLevel = Serial.parseInt();
    countA = Serial.parseInt();
    countB = Serial.parseInt();


    // Skip the remaining characters
    while(Serial.available() > 0){
      Serial.read();
    }

    Serial.print("Count for City A: ");
    Serial.println(countA);
    Serial.print("Count for City B: ");
    Serial.println(countB);
    Serial.print("Light Level for City A/B: ");
    Serial.println(lightLevel);

    // Send data to ThingSpeak
    sendDataToThingSpeak(APIA, countA, lightLevel);
    sendDataToThingSpeak(APIB, countB, lightLevel);
    delay(500);
  }
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void sendDataToThingSpeak(const char* apiKey, int dataField1, int dataField2) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(dataField1) + "&field2=" + String(dataField2);
    Serial.print("URL: ");
    Serial.print(url);
    Serial.print(" WiFi Status: ");
    Serial.println(WiFi.status());
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode == HTTP_CODE_OK) {
      Serial.println("Data sent successfully to ThingSpeak.");
    } else {
      Serial.print("Error sending data to ThingSpeak. HTTP Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected. Unable to send data to ThingSpeak.");
  }
}
