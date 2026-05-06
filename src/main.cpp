#include <Arduino.h>

#define PHOTO_SENSOR_PIN 4
#define LED_PIN 16

const int ON_THRESHOLD = 900;
const int OFF_THRESHOLD = 1200;

uint16_t light_sensor_value[10];
bool is_led_on = false;
float buffered_value = -1;

void setup() {
  Serial.begin(115200);

  uint32_t startLog = millis();
  while (!Serial && (millis() - startLog < 3000)) {
    delay(10);
  }

  Serial.println("\n--- Система запущена ---");
  pinMode(PHOTO_SENSOR_PIN, ANALOG);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uint32_t average_light_value = 0;

  for (int i = 0; i < 10; i++) {
    light_sensor_value[i] = analogRead(PHOTO_SENSOR_PIN);
    average_light_value += light_sensor_value[i];
  }
  average_light_value /= 10;

  if (buffered_value < 0) {
    buffered_value = average_light_value;
  }

  if (buffered_value < average_light_value) {
    buffered_value += 10;
    if (buffered_value > average_light_value) buffered_value = average_light_value;
  } else if (buffered_value > average_light_value) {
    buffered_value -= 10;
    if (buffered_value < average_light_value) buffered_value = average_light_value;
  }

  Serial.printf("AVG: %u | BUF: %.1f | LED: %s\n", average_light_value, buffered_value, is_led_on ? "ON" : "OFF");

  if (!is_led_on && buffered_value < ON_THRESHOLD) {
    is_led_on = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("СТАН: ТЕМНО -> LED УВІМКНЕНО");
  } 
  else if (is_led_on && buffered_value > OFF_THRESHOLD) {
    is_led_on = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("СТАН: СВІТЛО -> LED ВИМКНЕНО");
  }

  Serial.println("-----------------------------------");
  delay(100);
}