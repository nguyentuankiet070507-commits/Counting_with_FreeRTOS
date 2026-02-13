#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "RGB LED CONTROL"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#include <Arduino.h>
#include <WiFi.h>
//#include <PubSubClient.h>
//#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED 16

// LED
int led_pins[] = {14, 12, 13}; // r, g, b
const byte channels[] = {0, 1, 2};
const long freq = 5000;
const long resolution = 8;

// BUTTON
#define BUT_Y 18
#define BUT_N 19

//BLYNK
int red, green, blue;
char ssid[] = "Kain";
char pass[] = "tuankiet070507";
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
BLYNK_WRITE(V0){ red = param.asInt(); }
BLYNK_WRITE(V1){ green = param.asInt(); }
BLYNK_WRITE(V2){ blue = param.asInt(); }

bool start = 0, stop = 0;
void setup() {
  Serial.begin(115200);
  Serial.println("Start!");
  pinMode(BUT_Y, INPUT_PULLUP);
  pinMode(BUT_N, INPUT_PULLUP);

  for (long i = 0; i < 3; i++) {
    ledcSetup(channels[i], freq, resolution);
    ledcAttachPin(led_pins[i], channels[i]);
  }

  if (!OLED.begin(SSD1306_SWITCHCAPVCC,0x3C)) {
    Serial.println("OLED failed!");
    return;
  }

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);

  OLED.clearDisplay();
  OLED.setCursor(30, 20);
  OLED.setTextColor(WHITE);
  OLED.setTextSize(2);
  OLED.println("Start?");
  OLED.setCursor(0, 40);
  OLED.println("NO");
  OLED.setCursor(90, 40);
  OLED.println("YES");
  OLED.display();
}
void setColor(int r, int g, int b) {
  ledcWrite(channels[0], 255 - r);
  ledcWrite(channels[1], 255 - g);
  ledcWrite(channels[2], 255 - b);
}
void OLED_print(String s) {
  OLED.clearDisplay();
  OLED.setCursor(0, 20);
  OLED.setTextSize(2);
  OLED.setTextColor(WHITE);
  OLED.println(s);
  OLED.display();
}
void loop() {
  if (digitalRead(BUT_N) == LOW && !start && !stop) {
    Serial.println("OK!");
    OLED_print("OK!");
    delay(2000);
    stop = true;
    return;
  }

  if (digitalRead(BUT_Y) == LOW && !start) {
    Serial.println("Let's get started!");
    OLED_print("Let's get started!");
    delay(1000);
  

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    start = true;
  }

  if (start) {
    Blynk.run();
    setColor(red, green, blue);
  }

}