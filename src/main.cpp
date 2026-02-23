//#define BLYNK_PRINT Serial


#include <Arduino.h>
#include <WiFi.h>
//#include <PubSubClient.h>
//#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
//#include <BlynkSimpleEsp32.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define LED 16

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


bool start = 0, stop = 0;

// Sensor
#define SENSOR 27
QueueHandle_t sensorQueue;
volatile int peo_cnt = 0;
volatile bool detected = false;
void sensor_task(void *parameter) {
  bool state = 0;
  while (true) {
    state = digitalRead(SENSOR);

    xQueueSend(sensorQueue, &state, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void calc_task(void *parameter) {
  bool state = 0;
  bool last_state = HIGH;
  while (true) {
    if (xQueueReceive(sensorQueue, &state, portMAX_DELAY)) {
      if (last_state == HIGH && state == LOW) {
        peo_cnt++;
        detected = true;
      }

      if (state == HIGH) {
        detected = false;
      }

      last_state = state;
    }
  }
}
void oled_task(void *parameter) {
  while (true) {
    OLED.clearDisplay();
    OLED.setCursor(0, 20);
    OLED.setTextColor(WHITE);
    OLED.setTextSize(2);
    Serial.print("People: ");
    Serial.println(peo_cnt);
    OLED.print("People:");
    OLED.println(peo_cnt);
    OLED.display();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Start!");

  pinMode(SENSOR, INPUT);
  Wire.begin(21, 22);


  if (!OLED.begin(SSD1306_SWITCHCAPVCC,0x3C)) {
    Serial.println("OLED failed!");
    while (true);
    return;
  }

  sensorQueue = xQueueCreate(10, sizeof(bool));

  xTaskCreate(sensor_task, "Sensor Task", 4096, NULL, 1, NULL);
  xTaskCreate(calc_task, "Calculator Task", 4096, NULL, 1, NULL);
  xTaskCreate(oled_task, "OLED Task", 4096, NULL, 1, NULL);
}
void loop() {
  // Empty
}