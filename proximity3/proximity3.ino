#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Define the GPIO pin connected to the pulse sensor
#define PULSE_SENSOR_PIN 26  // Change this to your actual GPIO pin

// Replace with your network credentials
const char* ssid = "Cayrol";
const char* password = "blok-f1108";

IPAddress staticIP(192, 168, 169, 147);
IPAddress gateway(192, 168, 169, 27);
IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(192, 168, 1, 254);

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Variables for pulse counting and debounce
volatile unsigned long lastPulseTime = 0;
volatile unsigned int pulseCount = 0;
const unsigned long debounceDelay = 5;  // Debounce time in milliseconds
int currentButtonState = 0;
int buttonState = 0;
int finalCount = 0;

void wifiSetup(){
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

void IRAM_ATTR pulseISR() {
  unsigned long currentTime = millis();
  currentButtonState = digitalRead(PULSE_SENSOR_PIN);

  // Debounce logic: only count the pulse if enough time has passed
  if ((currentTime - lastPulseTime) > debounceDelay) {
    if (currentButtonState != buttonState){
      buttonState = currentButtonState;
      if (buttonState == LOW){
        pulseCount++;
      }
    }
    // Serial.println(pulseCount);
    lastPulseTime = currentTime;
  }
}

void IRAM_ATTR pulse() {

  currentButtonState = digitalRead(PULSE_SENSOR_PIN);
  if (currentButtonState == LOW){
        pulseCount++;
  }


}

void setup() {
  Serial.begin(9600);
  
  // Configure the pulse sensor pin as input
  pinMode(PULSE_SENSOR_PIN, INPUT_PULLUP);

  // Attach the interrupt to the pulse sensor pin
  attachInterrupt(digitalPinToInterrupt(PULSE_SENSOR_PIN), pulse, FALLING);


  wifiSetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // String response = "Sensor Value: " + String(sensorValue);
    String response = "api_key=uktp&sensor=COUNTESP32-147&count="+ String(finalCount);
    request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();
}

void loop() {
  // Example: Print the pulse count every second
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastPrintTime >= 10000) {
    Serial.print("Pulse Count: ");
    Serial.println(pulseCount);
    finalCount = pulseCount;

    // Reset the count after printing if needed
    pulseCount = 0;
    lastPrintTime = currentTime;
  }
  delay(100);
}
