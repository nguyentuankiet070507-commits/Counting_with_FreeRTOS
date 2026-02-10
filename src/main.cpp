#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL60PzggMhf"
#define BLYNK_TEMPLATE_NAME "LED Control"
#define BLYNK_AUTH_TOKEN "YOUR_TOKEN"

#include <Arduino.h>
#include <WiFi.h>
//#include <PubSubClient.h>
//#include <ArduinoJson.h>
//#include <DHT.h>

#include <BlynkSimpleEsp32.h>

const char *ssid = "Kain";
const char *password = "tuankiet070507";
#define LED 16
// put function declarations here:

BLYNK_WRITE(V0) {
  int val = param.asInt();
  digitalWrite(LED, val ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start!");
  pinMode(LED, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  Blynk.run();
}