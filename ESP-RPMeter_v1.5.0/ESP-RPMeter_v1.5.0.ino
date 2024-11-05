/*
  File: ESP-RPMeter_v1.5.0.ino
  Purpose: To measure speed using sensors such as proximity or optical sensors.
  Author: Mohamad Khairul Akmal Bin Amran
  Date: 2024-11-05
  Description: This code calculates RPM using an interrupt by counting the rising or falling edges of pulse signals from the sensor, 
  then determining the RPM. 
  */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Define the GPIO pin connected to the pulse sensor
#define PULSE_SENSOR_PIN 26  // Change this to your actual GPIO pin

// Network credentials and version
const String version = "ESP-RPMeter_v1.5.0";
const char* ssid = " ";
const char* password = " ";

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variables for pulse counting and debounce
volatile unsigned long lastPulseTime = 0;
volatile unsigned int pulseCount = 0;
const unsigned long debounceDelay = 5;  // Debounce time in milliseconds
int currentButtonState = 0;
int buttonState = 0;
int finalCount = 0;

//////////////////////////////////////SUB AND FUNCTION /////////////////////////////////////////////////////////////
/**
 * @brief Attempt to connect to a WiFi network and, upon successful connection, display the IP address.
 * 
 * @param none none
 * @return none.
 */
void wifiSetup() {
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

/**
 * @brief Trigger an interrupt and apply debouncing to ensure the pulse signal is free from noise.
 * 
 * @param none none
 * @return none.
 */
void IRAM_ATTR pulseISR() {
  unsigned long currentTime = millis();
  currentButtonState = digitalRead(PULSE_SENSOR_PIN);

  // Debounce logic: only count the pulse if enough time has passed
  if ((currentTime - lastPulseTime) > debounceDelay) {
    if (currentButtonState != buttonState) {
      buttonState = currentButtonState;
      if (buttonState == LOW) {
        pulseCount++;
      }
    }
    // Serial.println(pulseCount);
    lastPulseTime = currentTime;
  }
}

/**
 * @brief Trigger an interrupt without applying debounce. 
 * 
 * @param none none
 * @return none.
 */
void IRAM_ATTR pulse() {

  currentButtonState = digitalRead(PULSE_SENSOR_PIN);
  if (currentButtonState == LOW) {
    pulseCount++;
  }
}

/**
 * @brief Display the pulse count on the serial monitor when the cutoff time is reached.
 * 
 * @param timeout set timeout in miliseconds
 * @return pulse count
 */
int pulseCountOnTimeout(int timeout) {
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();
  unsigned long cutOffTime = timeout;  // set cut off time


  if (currentTime - lastPrintTime >= cutOffTime) {
    //Serial.print("Pulse Count: ");
    //Serial.println(pulseCount);
    finalCount = pulseCount;

    // Reset the count after printing if needed
    pulseCount = 0;
    lastPrintTime = currentTime;
  }

  return finalCount;
}

/**
 * @brief Check the pulse state, used for testing to identify the current state of the pulse.
 * 
 * @param none none
 * @return state of the pulse
 */
int pulse_state() {
  int state = digitalRead(PULSE_SENSOR_PIN);
  //Serial.println(buttonState);
  return state;
}


////////////////////////////////////THIS IS MAIN PROGRAM //////////////////////////////////////////////
/**
 * @brief SETUP : Initialize the program for continuous loop execution by setting the baud rate, configuring pin modes, 
 *        setting up interrupts, and defining server responses.
 * 
 * @param none none
 * @return none
 */
void setup() {

  Serial.begin(115200);
  Serial.println(version);

  // Configure the pulse sensor pin as input
  pinMode(PULSE_SENSOR_PIN, INPUT_PULLUP);

  // Attach the interrupt to the pulse sensor pin
  attachInterrupt(digitalPinToInterrupt(PULSE_SENSOR_PIN), pulse, FALLING);

  wifiSetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    // String response = "Sensor Value: " + String(sensorValue);
    String response = "api_key=uktp&sensor=" + version + "&count=" + String(finalCount);
    request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();
}

/**
 * @brief MAIN LOOP
 * 
 * @param none none
 * @return none.
 */
void loop() {

  int display = pulseCountOnTimeout(1000);
  // int display = pulse_state();  // for testing only
  Serial.print("Version : ");
  Serial.println(version);
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.print("Pulse Count/state : ");
  Serial.println(display);

  delay(100);
}
