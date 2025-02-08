#include <Arduino.h>
void setup()
{
    Serial.begin(115200);
    delay(1000);
}

void loop()
{
    char serial_input[50];
   
    delay(500);
    Serial.println("ESP32 running...");

    if (Serial.available() > 0) {  // Check if data is available
        String input = Serial.readString();
        
    }
}