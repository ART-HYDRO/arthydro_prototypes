//ardino code connected with same connections with added voltage sensor s pin to ardino uno A1 and I pin to the ardino gnd 

// Define pins for the Ultrasonic Sensor
#define TRIG_PIN 9
#define ECHO_PIN 10

// Define pin for the Voltage Sensor
const int voltagePin = A1; // Analog pin connected to 'S' on the voltage sensor

void setup() {
  Serial.begin(9600);
  
  // Ultrasonic Sensor setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Read ultrasonic sensor
  int distance = getDistance();
  delay(50); // Allow time between readings to avoid interference

  // Read voltage sensor
  float voltage = getVoltage();
  delay(50); // Additional delay for stability

  // Output distance to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Output voltage to Serial Monitor
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  // Send the data via serial communication to ESP8266
  Serial.print("D:");
  Serial.print(distance);
  Serial.print(";");
  Serial.print("V:");
  Serial.print(voltage);
  Serial.println(";");
  
  delay(1000); // Delay for readability
}

// Function to get distance from Ultrasonic Sensor
int getDistance() {
  noInterrupts(); // Disable interrupts for accurate timing
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  interrupts(); // Re-enable interrupts after reading

  int distance = duration * 0.034 / 2; // Convert duration to distance in cm
  return distance;
}

// Function to get voltage from Voltage Sensor
float getVoltage() {
  int sensorValue = analogRead(voltagePin);
  float voltage = sensorValue * (5.0 / 1023.0) * 5.7; // Adjust scaling factor for your voltage divider
  return voltage;
}



//esp8266 code with added reading diplay of voltage sensor reading


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Galaxy M34 5G 288C";
const char* password = "123456789";


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



