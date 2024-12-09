/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

//Include section
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
#include <DFRobot_WT61PC.h>

//define section
#define rxPin 16
#define txPin 17
#define FPSerial mySerial

//task handler
TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int led1 = 2;
const int led2 = 4;

// Replace with your network credentials
const char* ssid = "Cayrol";
const char* password = "blok-f1108";

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
SoftwareSerial mySerial(rxPin, txPin);
DFRobot_WT61PC sensor(&FPSerial);


/*********
  setup section
*********/


void setup() {
  Serial.begin(115200); 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

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

  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  sensor.modifyFrequency(FREQUENCY_200HZ);

  for(;;){
    readSensor();
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code_old( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    Serial.print("Write from Core 1\t"); Serial.print(sensor.Acc.X); Serial.print("\n");
    delay(700);
  }
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  wifiSetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // String response = "Sensor Value: " + String(sensorValue);
    String response = "api_key=tPmAT5Ab3j7F9&sensor=DFROBOT&acc_x="+ String(sensor.Acc.X) +"&acc_y=" + String(sensor.Acc.Y) +"&acc_z="+ String(sensor.Acc.Z);
    request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();


//loop
  for(;;){
   
    // delay(700);
  }
}

void loop() {


}


/*********
  sub routine section
*********/


//setup wifi
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

//setup read sensor
void readSensor(){

    if (sensor.available()) {
    Serial.print("Acc\t"); Serial.print(sensor.Acc.X); Serial.print("\t");
    Serial.print(sensor.Acc.Y); Serial.print("\t"); Serial.println(sensor.Acc.Z); //acceleration information of X,Y,Z
    Serial.print("Gyro\t"); Serial.print(sensor.Gyro.X); Serial.print("\t");
    Serial.print(sensor.Gyro.Y); Serial.print("\t"); Serial.println(sensor.Gyro.Z); //angular velocity information of X,Y,Z
    Serial.print("Angle\t"); Serial.print(sensor.Angle.X); Serial.print("\t");
    Serial.print(sensor.Angle.Y); Serial.print("\t"); Serial.println(sensor.Angle.Z); //angle information of X, Y, Z 
    Serial.println();
  }
}