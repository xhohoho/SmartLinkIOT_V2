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

void IRinterrupt() {
  currentButtonState = digitalRead(PIN);

  if (currentButtonState != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (currentButtonState != buttonState) {
        buttonState = currentButtonState;
      if (buttonState == LOW) {
          counter++;
      }
    }
  }  
  lastButtonState = currentButtonState;
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN), IRinterrupt, FALLING);
  currentTime = interval;
}

void loop() {


detachInterrupt(digitalPinToInterrupt(PIN));

int rpm = (float(counter / 3 )) * 60; //divided by delay


Serial.println(millis());
Serial.println(counter);
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
