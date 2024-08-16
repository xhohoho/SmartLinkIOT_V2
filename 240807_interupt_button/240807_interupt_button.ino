#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Pejabat BKPT Sg.Buloh";
const char* password = "12345678ae";

const int switchPin = 26; // Define the switch input pin
const unsigned long debounceDelay = 50; // Debounce time in milliseconds

int counter = 0;

volatile bool switchState = false;
volatile unsigned long lastInterruptTime = 0;

void IRAM_ATTR handleSwitch() {

unsigned long currentTime = millis();


  
  // Check if the time since the last interrupt is greater than the debounce delay
  if ((currentTime - lastInterruptTime) > debounceDelay) {
    // switchState = !switchState; // Toggle the switch state

    if (switchPin == !switchState){
    counter++;
    lastInterruptTime = currentTime; // Update the last interrupt time
    switchState = switchPin;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(switchPin, INPUT); // Use INPUT_PULLUP for better noise handling
  attachInterrupt(digitalPinToInterrupt(switchPin), handleSwitch, RISING);

  // WiFi.begin(ssid, password);
  // Serial.println("Connecting");
  // while(WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to WiFi network with IP Address: ");
  // Serial.println(WiFi.localIP());
 
  // Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  // In the loop, you can use the switchState variable
  // For example, print the switch state to the serial monitor
  Serial.print("Switch state: ");
  // Serial.println(switchState ? "ON" : "OFF");
  Serial.println(counter);
  delay(200); // Slow down the loop for readability
}
