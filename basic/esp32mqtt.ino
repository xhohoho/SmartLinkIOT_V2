#include "EspMQTTClient.h"
#include <WiFi.h>  // For ESP32 boards

#define MYLED 2  // Built-in LED Pin

EspMQTTClient client(
  "LGM Kilang Rheometer Wifi",                     // wifi SSID
  "12345678ae",                  // wifi password
  "broker.mqttdashboard.com",  // MQTT Broker server ip
  "",                          // MQTT Username: Can be omitted if not needed
  "",                          // MQTT Password: Can be omitted if not needed
  "espclientID",               // Client name that uniquely identify your device
  1883                         // The MQTT port, default to 1883. this line can be omitted
);

void setup() {
  Serial.begin(115200);
  Serial.println("Setting Up..");

  client.enableDebuggingMessages();  // Enable debugging messages sent to serial output
  pinMode(MYLED, OUTPUT);
}

// This function is called once everything is connected (Wifi and MQTT)
// You can add subscription here
void onConnectionEstablished() {
  Serial.println("Connection established..");

  // you can add topic subscription in this function
}

int led_state = HIGH;

void loop() {
  client.loop();

  delay(2000);

  led_state = !led_state;  // change state
  digitalWrite(MYLED, led_state);
  // Publish message
  client.publish("mynode/led1", String(led_state));
}
