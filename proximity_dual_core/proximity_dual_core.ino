/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

//Include section
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
// #include <SoftwareSerial.h>
// #include <DFRobot_WT61PC.h>

//define section
// #define rxPin 16
// #define txPin 17
// #define FPSerial mySerial

//task handler
TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
// const int led1 = 2;
// const int led2 = 4;

// Replace with your network credentials
const char* ssid = "Cayrol";
const char* password = "blok-f1108";
const char* version = "Dual Core Version 1.0";

//IP address
IPAddress staticIP(192, 168, 169, 146);
IPAddress gateway(192, 168, 169, 27);
IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(192, 168, 1, 254);

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Simulate a sensor value
int sensorValue = 123;

// Set up a new SoftwareSerial object and sensor
// SoftwareSerial mySerial(rxPin, txPin);
// DFRobot_WT61PC sensor(&FPSerial);

int inputPin = 26;
int counter = 0;
int buttonState = 0;
int lastButtonState = 0;

int currentButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 5;
unsigned long myTime;


/*********
  setup section
*********/


void setup() {
  Serial.begin(115200); 
  // pinMode(led1, OUTPUT);
  // pinMode(led2, OUTPUT);
  pinMode(inputPin, INPUT);

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    currentButtonState = digitalRead(inputPin);
    sensor();
  } 
}



void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  wifiSetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // String response = "Sensor Value: " + String(sensorValue);
    String response = "api_key=tPmAT5Ab3j7F9&sensor=proximity&count="+ String(counter);
    request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();


//loop
  for(;;){

   

  }
}

void loop() {
  Serial.println(counter);
  delay(1000);

}


/*********
  sub routine section
*********/


//setup wifi
void wifiSetup(){
    // Connect to Wi-Fi network with SSID and password
  Serial.println(version);
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

//setup read sensor
void sensor(){

//delay (2000);
   if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }

    if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState != buttonState) {
        buttonState = currentButtonState;
      if (buttonState == LOW) {
          myTime = millis();
          counter++;
          // Serial.println(myTime);
          // Serial.println(counter);
      }
    }
  }
  
  lastButtonState = currentButtonState;

}