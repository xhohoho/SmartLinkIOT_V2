#include <SoftwareSerial.h>
#include <DFRobot_WT61PC.h>

#define rxPin 10
#define txPin 11
#define FPSerial mySerial

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

DFRobot_WT61PC sensor(&FPSerial);

void setup()  {

    Serial.begin(9600);
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);
    sensor.modifyFrequency(FREQUENCY_10HZ);
}

void loop() {
    // if (mySerial.available() > 0) {
    //     // mySerial.read();
    //   //Serial.print(mySerial.read());
    // }
  if (sensor.available()) {
    Serial.print("Acc\t"); Serial.print(sensor.Acc.X); Serial.print("\t");
    Serial.print(sensor.Acc.Y); Serial.print("\t"); Serial.println(sensor.Acc.Z); //acceleration information of X,Y,Z
    Serial.print("Gyro\t"); Serial.print(sensor.Gyro.X); Serial.print("\t");
    Serial.print(sensor.Gyro.Y); Serial.print("\t"); Serial.println(sensor.Gyro.Z); //angular velocity information of X,Y,Z
    Serial.print("Angle\t"); Serial.print(sensor.Angle.X); Serial.print("\t");
    Serial.print(sensor.Angle.Y); Serial.print("\t"); Serial.println(sensor.Angle.Z); //angle information of X, Y, Z 
    Serial.println();
  }

  delay(1000);

}