// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>

// #define OLED_RESET -1
// Adafruit_SSD1306 display(OLED_RESET);

int counter = 0;
int buttonState = 0;
int lastButtonState = 0;

//const int IR_PIN = 2;  // IR sensor input pin
const int PIN = 26;
const int interval = 15000;
int currentTime = 0;
int currentButtonState = 0;
int lastCounter = 0 ;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 5;

unsigned long previousMillis = 0;  // Variable to store previous time
unsigned int rpm = 0;  // Variable to store RPM value

void IRAM_ATTR IRinterrupt() {
  currentButtonState = digitalRead(PIN);
  Serial.println(millis());
  Serial.println((String)"Current Button State: " + currentButtonState);


  // if (currentButtonState != lastButtonState) {
  //   lastDebounceTime = millis();
  // }

  // if ((millis() - lastDebounceTime) > debounceDelay) {
  //   if (currentButtonState != buttonState) {
  //       buttonState = currentButtonState;
  //     if (buttonState == LOW) {

  //         counter++;

  //     }
  //   }
  // }

//   if (currentButtonState == LOW){

//   counter++;

//   // Serial.println(millis());
//   // Serial.println(counter);
// } 
  
  lastButtonState = currentButtonState;
  Serial.println(millis());
  Serial.println((String)"Last Button State: " + lastButtonState);
  Serial.println((String)"Counter: " + counter);



//kadang jadi kadang tak jadi, effect noise dgn motor.
// if (currentButtonState == LOW){

//   counter++;

//   // Serial.println(millis());
//   // Serial.println(counter);
// } 

}

void setup() {
  Serial.begin(9600);
  pinMode(PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN), IRinterrupt, CHANGE);
  currentTime = interval;
  Serial.println("RPM measurement system starting");
}

void loop() {


detachInterrupt(digitalPinToInterrupt(PIN));

// int rpm = (float(counter / 3 )) * 60; //divided by delay


// Serial.println(millis());
// Serial.println(counter);
// Serial.println("Measuring... last counter : ");
// Serial.println(lastCounter);

// if (millis() >  currentTime){
//   Serial.println(counter);
//   currentTime = currentTime + interval;
//   lastCounter = counter * 4;
//   counter = 0;
// }



attachInterrupt(digitalPinToInterrupt(PIN), IRinterrupt, FALLING);
delay(1000);// second delay
}
