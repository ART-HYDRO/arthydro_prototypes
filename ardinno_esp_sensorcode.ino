//ardino uno code to read the data from the sensor and tranfer data to esp8266 using TX(transmet) pin and esp8266 RX(recive) pin

#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  Serial.begin(9600);        // Initialize Serial for communication with ESP8266
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Send a 10µs HIGH pulse to the TRIG pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the ECHO pin and calculate distance in centimeters
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Check if we are receiving any pulse duration
  if (duration > 0) {
    int distance = duration * 0.034 / 2; // Convert to cm
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Send the distance data via Serial to the ESP8266
    Serial.println(distance);
  } else {
    Serial.println("No echo received!");
  }

  delay(1000); // Wait for a second before the next reading
}





//esp8266 code to recive the data from ardinouno and display on website using rx(recive) pin of esp8266



#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Your_SSID";         // Replace with your WiFi SSID
const char* password = "Your_PASSWORD";  // Replace with your WiFi Password

ESP8266WebServer server(80);  // Web server on port 80
String distanceData = "Waiting for data...";

// Function to read data from Arduino via Serial
void readFromArduino() {
  while (Serial.available() > 0) {
    distanceData = Serial.readStringUntil('\n');  // Read data until newline
  }
}

// Function to handle HTTP requests to the root "/"
void handleRoot() {
  String html = "<html><body><h1>Distance Measurement</h1>";
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

  // Set up web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient(); // Handle web server client requests
  readFromArduino();     // Read latest data from the Arduino
}
//note to give the volatage suply from ardino to esp8266 connect ardino uno 5v to esp 5v and ardinouno GND to esp GND


