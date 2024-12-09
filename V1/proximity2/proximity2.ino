// Define the GPIO pin connected to the pulse sensor
#define PULSE_SENSOR_PIN 26  // Change this to your actual GPIO pin

// Variables for pulse counting and debounce
volatile unsigned long lastPulseTime = 0;
volatile unsigned int pulseCount = 0;
const unsigned long debounceDelay = 5;  // Debounce time in milliseconds
int currentButtonState = 0;
int buttonState = 0;

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
}

void loop() {
  // Example: Print the pulse count every second
  static unsigned long lastPrintTime = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastPrintTime >= 10000) {
    Serial.print("Pulse Count: ");
    Serial.println(pulseCount);

    // Reset the count after printing if needed
    pulseCount = 0;
    lastPrintTime = currentTime;
  }
  delay(100);
}
