#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
#include <DFRobot_WT61PC.h>


#define rxPin 16
#define txPin 17
#define FPSerial mySerial

// Replace with your network credentials
// const char* ssid = "Cayrol";
// const char* password = "blok-f1108";
const char* ssid = "Pejabat BKPT Sg.Buloh";
const char* password = "12345678ae";


IPAddress staticIP(192, 168, 169, 146);
IPAddress gateway(192, 168, 169, 27);
IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(192, 168, 1, 254);

// Create an AsyncWebServer object on port 80
AsyncWebServer server(80);

// Simulate a sensor value
int sensorValue = 123;

// Set up a new SoftwareSerial object
// SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);
SoftwareSerial mySerial(rxPin, txPin);

DFRobot_WT61PC sensor(&FPSerial);


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

void setup(){
  Serial.begin(115200);
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
    // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  sensor.modifyFrequency(FREQUENCY_200HZ);

  wifiSetup();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // String response = "Sensor Value: " + String(sensorValue);
    String response = "api_key=tPmAT5Ab3j7F9&sensor=DFROBOT&acc_x="+ String(sensor.Acc.X) +"&acc_y=" + String(sensor.Acc.Y) +"&acc_z="+ String(sensor.Acc.Z);
    request->send(200, "text/plain", response);
  });

  // Start server
  server.begin();
}

void loop(){
  // Simulate updating sensor value
 // Example: read from analog pin 34
  readSensor();
  // sensorValue = sensor.Acc.X;
  delay(1000);
}

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
