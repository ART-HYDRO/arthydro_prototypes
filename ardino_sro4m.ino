//ardino uno with AJ-SRO4M sensor module code working

#define TRIG_PIN 9
#define ECHO_PIN 10

void setup() {
  Serial.begin(9600);
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
  } else {
    Serial.println("No echo received!");
  }

  delay(1000); // Wait for a second before the next reading
}
