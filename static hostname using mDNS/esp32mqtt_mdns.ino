#include "EspMQTTClient.h"
#include <WiFi.h>  // For ESP32 boards
#include "esp_timer.h"  // Include the ESP32 timer library
#include <ESPmDNS.h>    // Include the mDNS library

#define MYLED 2  // Built-in LED Pin

const char* ssid = "LGMPTL";
const char* password = "12345678ae";
const char* servername = "cmms.local";
const char* hostname = "esp32nodetest";
int i=0;

EspMQTTClient client(
  ssid,                     // WiFi SSID
  password,                                   // WiFi password
  servername,                                    // MQTT Broker server ip
  "",                                             // MQTT Username: Can be omitted if not needed
  "",                                             // MQTT Password: Can be omitted if not needed
  "espclientIDtest",                                   // Client name that uniquely identifies your device
  1883                                            // The MQTT port, default to 1883. This line can be omitted
);

int led_state = HIGH;  // Variable to store the current LED state

// RTC Timer handle
esp_timer_handle_t toggle_timer_handle;

// Timer callback function
void toggleLED(void* arg) {
  led_state = !led_state;  // Toggle the LED state
  digitalWrite(MYLED, led_state);  // Set the LED to the new state
  // Publish the LED state to MQTT
  i=i+1;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Setting Up..");

  client.enableDebuggingMessages();  // Enable debugging messages sent to serial output

  pinMode(MYLED, OUTPUT);

  // Initialize RTC timer to call `toggleLED` every 2 seconds (2000000 microseconds)
  esp_timer_create_args_t timer_args = {
    .callback = &toggleLED,  // Timer callback function
    .name = "LED_Toggle_Timer"
  };
  esp_timer_create(&timer_args, &toggle_timer_handle);

  // Start the timer with an interval of 2000 milliseconds (2 seconds)
  //esp_timer_start_periodic(toggle_timer_handle, 1000000);  // 2000000 microseconds = 2 seconds
  esp_timer_start_periodic(toggle_timer_handle, 500000);  // 2000000 microseconds = 2 seconds
  
}

// This function is called once everything is connected (WiFi and MQTT)
// You can add subscription here
void onConnectionEstablished() {
  Serial.println("Connection established..");
  if (!MDNS.begin(hostname)) {
      Serial.println("Error restarting mDNS after Wi-Fi reconnection");
      ESP.restart();  // Optionally restart ESP32 if mDNS fails
    }
  Serial.println("mDNS started");
}

void loop() {
  client.loop();  // Handle MQTT communication
}
