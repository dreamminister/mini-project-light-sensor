#include <Arduino.h>

#define PHOTO_SENSOR_PIN 4
#define LED_PIN 16

const int ON_THRESHOLD = 900;   // Темніше за це -> вмикаємо
const int OFF_THRESHOLD = 1300; // Світліше за це -> вимикаємо

uint16_t light_sensor_value[10];
bool is_led_on = false;

void setup() {
  Serial.begin(115200);

  uint32_t startLog = millis();
  while (!Serial && (millis() - startLog < 3000)) {
    delay(10);
  }

  Serial.println("\n--- Система з гістерезисом запущена ---");
  pinMode(PHOTO_SENSOR_PIN, ANALOG);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  uint32_t average_light_value = 0;

  for (int i = 0; i < 10; i++) {
    light_sensor_value[i] = analogRead(PHOTO_SENSOR_PIN);
    average_light_value += light_sensor_value[i];
    delay(10);
  }
  average_light_value /= 10;

  Serial.printf("AVG: %u | LED: %s\n", average_light_value, is_led_on ? "ON" : "OFF");

  if (!is_led_on && average_light_value < ON_THRESHOLD) {
    is_led_on = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("СТАН: ТЕМНО -> LED УВІМКНЕНО");
  } 
  else if (is_led_on && average_light_value > OFF_THRESHOLD) {
    is_led_on = false;
    digitalWrite(LED_PIN, LOW);
    Serial.println("СТАН: СВІТЛО -> LED ВИМКНЕНО");
  }

  Serial.println("-----------------------------------");
  delay(1000);
}