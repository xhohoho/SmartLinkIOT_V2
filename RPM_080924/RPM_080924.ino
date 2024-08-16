#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH 128 // OLED display width, in pixels
#define OLED_HEIGHT 32

#define  OLED_ADDR   0x3C // A very common address for these displays.

#define OLED_RESET -1
//Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
//Adafruit_SSD1306  display(OLED_WIDTH, OLED_HEIGHT);

int inputPin = 26;
int counter = 0;
int buttonState = 0;
int lastButtonState = 0;

int currentButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 5;
unsigned long myTime;

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);

  // display.begin(SSD1306_SWITCHCAPVCC,  OLED_ADDR);
  // display.clearDisplay();
}

void loop() {
  currentButtonState = digitalRead(inputPin);

 
  // myTime= millis();
  // Serial.println(myTime);
  sensor();
  //

}

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
          Serial.println(myTime);
          Serial.println(counter);
      }
    }
  }
  
  lastButtonState = currentButtonState;

}