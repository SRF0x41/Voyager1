
#include <Wire.h>
#include <Arduino.h>

typedef struct {
  // Valid commands 
  
}

void setup() {
    Serial.begin(115200);  // Start serial communication at 115200 baud rate
}

void loop() {
    if (Serial.available() > 0) {  // Check if data is available
        String receivedData = Serial.readString();  // Read incoming string
        Serial.print("Received: ");
        Serial.println(receivedData);
    }
}
