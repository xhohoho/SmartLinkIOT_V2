#include <SoftwareSerial.h>

// Define the pins for RX and TX
#define RX_PIN 16
#define TX_PIN 17

// Create a SoftwareSerial object
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // Start hardware serial communication for debugging
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Start software serial communication
  mySerial.begin(9600);
  Serial.println("Software serial communication started");
}

void loop() {
  // Send data from the hardware serial port to the software serial port
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    mySerial.write(inChar);
  }

  // Send data from the software serial port to the hardware serial port
  if (mySerial.available()) {
    char inChar = (char)mySerial.read();
    Serial.write(inChar);
  }
}
