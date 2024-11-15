// just small change in code of esp Unique Device ID: String device_id = "device001"; we will chage device id for each ddeivce so that it will be easy to setup in the database.

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FirebaseESP8266.h>

// Wi-Fi credentials
const char* ssid = "Galaxy M34 5G 288C";
const char* password = "123456789";

// Firebase credentials
#define DATABASE_URL "https://arthydro-ebcb8-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define DATABASE_SECRET "AIzaSyDqW4Y2YkqD7OniJzHLFj65R5iJxgGU_fc"

// Device-specific ID - This should be unique for each device
String device_id = "device001"; // Change this ID for each device

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

ESP8266WebServer server(80);  // Web server on port 80
String distanceData = "Waiting for data...";

// Function to read data from Arduino via Serial
void readFromArduino() {
  if (Serial.available() > 0) {
    String rawData = Serial.readStringUntil('\n');  // Read data until newline
    rawData.trim();  // Remove any whitespace or newline characters
    if (rawData.length() > 0) {
      distanceData = rawData;  // Store the raw data exactly as received
      sendToFirebase();
    }
  }
}

// Function to send data to Firebase
void sendToFirebase() {
  if (Firebase.ready()) {
    // Create a JSON object with device-specific data
    FirebaseJson json;
    json.set("distance", distanceData);
    json.set("timestamp/.sv", "timestamp");

    // Send data to the Firebase path for this specific device ID
    String firebasePath = "/devices/" + device_id + "/sensor_data";
    
    if (Firebase.setJSON(fbdo, firebasePath, json)) {
      Serial.println("Data sent to Firebase for device " + device_id + ": " + distanceData);
    } else {
      Serial.println("Failed to send data to Firebase");
      Serial.println("Reason: " + fbdo.errorReason());
    }
  }
}

// Function to handle HTTP requests to the root "/"
void handleRoot() {
  String html = "<html><body><h1>Distance Measurement for " + device_id + "</h1>";
  html += "<p>Distance: " + distanceData + " cm</p>";
  html += "<p>Page refreshes every second.</p>";
  html += "<script>setTimeout(function(){location.reload();}, 1000);</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);  // Initialize Serial for communication with Arduino
  
  // Initialize Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  // Set up web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient(); // Handle web server client requests
  readFromArduino();     // Read latest data from the Arduino
}
